Mute 
=========

[![](https://secure.travis-ci.org/joy2fun/mute.png)](https://travis-ci.org/joy2fun/mute)

 - 可以屏蔽 `PHP` 的 `function` 或 `method` 调用
 - 不可以屏蔽 `Closure` 以及内建调用
 - 被屏蔽的调用返回值始终为 `NULL`

# 安装

环境要求：
 - PHP 7.0 +

```sh
phpize
./configure --with-php-config=/path/to/php-config
make && make install
```

在 `php.ini` 中添加：

```ini
[mute]
extension=mute.so
; 多个设置可由 `,` 或空格分隔
; static 方法和非 static 方法调用均采用 myObject::myMethod 的格式
mute.calls=myFunction, myClass::myMethod
```

