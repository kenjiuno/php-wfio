--TEST--
wfio mkdir,rmdir
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "mkdir ";
$f = mkdir("wfio://123abcABC");
echo !!$f;
echo "\n";

echo "file_put_contents ";
echo file_put_contents("wfio://123abcABC/file.bin", "abc123");
echo "\n";

echo "file_get_contents ";
echo file_get_contents("wfio://123abcABC/file.bin");
echo "\n";

echo "unlink ";
echo unlink("wfio://123abcABC/file.bin");
echo "\n";

echo "rmdir ";
$f = rmdir("wfio://123abcABC");
echo !!$f;
echo "\n";

?>
--EXPECT--
mkdir 1
file_put_contents 6
file_get_contents abc123
unlink 1
rmdir 1
