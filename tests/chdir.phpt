--TEST--
Whether wfio follows php5's chdir.
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "mkdir ";
$f = mkdir("test-subdir");
echo !!$f;
echo "\n";

echo "chdir ";
$f = chdir("test-subdir");
echo !!$f;
echo "\n";

echo "touch ";
$f = touch("touch.bin");
echo !!$f;
echo "\n";

echo "wfio_fopen8 ";
$f = wfio_fopen8("touch.bin", "rb");
echo !!$f;
echo "\n";

echo "fclose ";
echo !!fclose($f);
echo "\n";

echo "mkdir ";
$f = mkdir("wfio-should-see");
echo !!$f;
echo "\n";

echo "scandir ";
$f = scandir("wfio://wfio-should-see");
echo var_dump($f);
echo "\n";

echo "rmdir ";
$f = rmdir("wfio-should-see");
echo !!$f;
echo "\n";

echo "unlink ";
$f = unlink("touch.bin");
echo !!$f;
echo "\n";

echo "chdir ";
$f = chdir("..");
echo !!$f;
echo "\n";

echo "rmdir ";
$f = rmdir("test-subdir");
echo !!$f;
echo "\n";

?>
--EXPECT--
mkdir 1
chdir 1
touch 1
wfio_fopen8 1
fclose 1
mkdir 1
scandir array(2) {
  [0]=>
  string(1) "."
  [1]=>
  string(2) ".."
}

rmdir 1
unlink 1
chdir 1
rmdir 1
