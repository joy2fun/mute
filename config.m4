PHP_ARG_ENABLE(mute,
    whether to enable mute module,
    [ --enable-mute   Enable mute support ])

if test "$PHP_mute" != "no"; then
  PHP_NEW_EXTENSION(mute, mute.c, $ext_shared)
fi

