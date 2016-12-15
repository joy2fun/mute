--TEST--
Check for muting callback
--SKIPIF--
<?php if (!extension_loaded("mute")) print "skip"; ?>
--INI--
mute.calls=a 
--FILE--
<?php 
function a(){
  echo "a() called.\n";
}

array_map("a", range(0, 9));
?>
--EXPECTF--

