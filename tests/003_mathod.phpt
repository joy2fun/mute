--TEST--
Check for muting method calls
--SKIPIF--
<?php if (!extension_loaded("mute")) print "skip"; ?>
--INI--
mute.calls=C::a,C::s
--FILE--
<?php 
class C {
  function a(){
    echo "called.\n";
  }
  static function s(){
    echo "s() called.\n";
  }
}
$c = new C;
$c->a();
C::s();
?>
--EXPECTF--

