--TEST--
wfio stat,normal stat
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "file_put_contents ";
$f = file_put_contents("test_stat.bin", "123");
echo !!$f;
echo "\n";

$res1 = stat("test_stat.bin");
$res2 = stat("wfio://test_stat.bin");

echo "mtime ";
echo $res1["mtime"] - $res2["mtime"];
echo "\n";

echo "size ";
echo $res1["size"] - $res2["size"];
echo "\n";

echo "unlink ";
$f = unlink("test_stat.bin");
echo !!$f;
echo "\n";

?>
--EXPECT--
file_put_contents 1
mtime 0
size 0
unlink 1
