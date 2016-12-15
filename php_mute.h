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

/* $Id$ */

#ifndef PHP_MUTE_H
#define PHP_MUTE_H

extern zend_module_entry mute_module_entry;
#define phpext_mute_ptr &mute_module_entry

#define PHP_MUTE_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_MUTE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MUTE_API __attribute__ ((visibility("default")))
#else
#	define PHP_MUTE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(mute)
	char *calls;
ZEND_END_MODULE_GLOBALS(mute)

#define MUTE_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(mute, v)

#if defined(ZTS) && defined(COMPILE_DL_MUTE)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

extern ZEND_DECLARE_MODULE_GLOBALS(mute);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim: noet sw=4 ts=4 fdm=marker
 */
