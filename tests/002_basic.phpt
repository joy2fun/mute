--TEST--
Check for muting function call
--SKIPIF--
<?php if (!extension_loaded("mute")) print "skip"; ?>
--INI--
mute.calls=b 
--FILE--
<?php 
function a(){
  echo "a() called.\n";
}
function b(){
  echo "b() called.\n";
}
a();
b();
$v = b();
var_dump($v);
?>
--EXPECTF--
a() called.
NULL

