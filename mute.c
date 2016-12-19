/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2016 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Chiao F <php@html.js.cn>                                     |
   +----------------------------------------------------------------------+
   */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mute.h"

ZEND_DECLARE_MODULE_GLOBALS(mute);

static HashTable *ht_mute_calls;

static void (*orig_execute_ex)(zend_execute_data *execute_data);
static zend_string * mute_get_call_name(zend_execute_data *ex);
static void mute_calls_init(void);
static void mute_call_add(char *call, size_t len);

static void mute_execute_ex(zend_execute_data *execute_data) /* {{{ */
{
	zend_string *call = mute_get_call_name(execute_data);

	if (call) {
		if (zend_hash_str_exists(ht_mute_calls, ZSTR_VAL(call), ZSTR_LEN(call))) {
			zend_string_free(call);
			goto skip;
		}
		zend_string_free(call);
	}

	zend_try {
		orig_execute_ex(execute_data);
	} zend_catch {
		zend_bailout();
	} zend_end_try();

skip:
	call = NULL;

} /* }}} */

static zend_string * mute_get_call_name(zend_execute_data *ex) /* {{{ */
{
	zend_function *zf = ex->func;
	zend_string *zs = NULL;
	size_t len = 0;
	size_t c_len = 0;

	if (zf->common.function_name) {
		len = zf->common.function_name->len;
		if (zf->common.scope) {
			c_len = zf->common.scope->name->len;
		}
	}

	if (len) {
		if (zf->common.function_name->val[0] == '{') { /* special */
			return zs;
		}
		zs = zend_string_init("", c_len ? len + c_len + 2 : len, 0);
		char *s = (char *) & ZSTR_VAL(zs);
		if (c_len) {
			s = strncat(s, zf->common.scope->name->val, c_len);
			s = strncat(s, "::", 2);
		}
		s = strncat(s, zf->common.function_name->val, len);
	}

	return zs;
} /* }}} */

static void mute_calls_init(void) /* {{{ */
{
	/* init persistent table */
	ht_mute_calls = (HashTable *) pemalloc(sizeof(HashTable), 1);
	zend_hash_init(ht_mute_calls, 0, NULL, ZVAL_PTR_DTOR, 1);
	GC_FLAGS(ht_mute_calls) |= IS_ARRAY_IMMUTABLE;
	GC_REFCOUNT(ht_mute_calls) = 2;

	/* parse ini settings */
	char *s = NULL;
	char *e = MUTE_G(calls);

	while (*e) {
		switch (*e) {
			case ' ':
			case ',':
				if (s) {
					*e = 0;
					mute_call_add(s, e-s);
					s = NULL;
				}
				break;
			default:
				if (!s) {
					s = e;
				}
		}
		e++;
	}

	if (s) {
		mute_call_add(s, e-s);
	}

}/* }}} */

static void mute_call_add(char *call, size_t len) /* {{{ */
{
	zend_string *key;
	zend_ulong idx;

	key = zend_string_init(call, len, 0);
	if (ZEND_HANDLE_NUMERIC(key, idx)) {
		/* skip numeric string */
		/* TODO call name validation */
	} else {
		zend_hash_str_add_empty_element(ht_mute_calls, call, len);
	}
}/* }}} */

static void mute_calls_free() /* {{{ */
{
	zend_string *key;
	if (((ht_mute_calls)->u.flags & HASH_FLAG_INITIALIZED)) {
		ZEND_HASH_FOREACH_STR_KEY(ht_mute_calls, key) {
			if (key) {
				free(key);
			}
		} ZEND_HASH_FOREACH_END();
		free(HT_GET_DATA_ADDR(ht_mute_calls));
	}
	free(ht_mute_calls);
} /* }}} */

/* {{{ PHP_INI */
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("mute.calls", "", PHP_INI_SYSTEM, OnUpdateString, calls, zend_mute_globals, mute_globals)
PHP_INI_END()
/* }}} */

PHP_MINIT_FUNCTION(mute) /* {{{ */
{
	REGISTER_INI_ENTRIES();
	mute_calls_init();
	orig_execute_ex = zend_execute_ex;
	zend_execute_ex = mute_execute_ex;
	return SUCCESS;
} /* }}} */

PHP_MSHUTDOWN_FUNCTION(mute) /* {{{ */
{
	UNREGISTER_INI_ENTRIES();
	mute_calls_free();
	return SUCCESS;
} /* }}} */

PHP_MINFO_FUNCTION(mute) /* {{{ */
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mute support", "enabled");
	php_info_print_table_row(2, "calls", MUTE_G(calls));
	php_info_print_table_end();
} /* }}} */

PHP_GINIT_FUNCTION(mute) /* {{{ */
{
	mute_globals->calls = NULL;
} /* }}} */

const zend_function_entry mute_functions[] = { /* {{{ */
	PHP_FE_END
};  /* }}} */

zend_module_entry mute_module_entry = { /* {{{ */
	STANDARD_MODULE_HEADER,
	"mute",
	mute_functions,
	PHP_MINIT(mute),
	PHP_MSHUTDOWN(mute),
	NULL,
	NULL,
	PHP_MINFO(mute),
	PHP_MUTE_VERSION,
	PHP_MODULE_GLOBALS(mute),
	PHP_GINIT(mute),
	NULL,
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
}; /* }}} */

#ifdef COMPILE_DL_MUTE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(mute)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim: noet sw=4 ts=4 fdm=marker
 */
