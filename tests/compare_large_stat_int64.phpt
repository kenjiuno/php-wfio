--TEST--
wfio stat,normal stat "CWD/path_to_largefile.ext" int64
--SKIPIF--
<?php
if (!extension_loaded("wfio")) print "skip";
if (!file_exists("path_to_largefile.ext")) print "skip";
if (PHP_INT_MAX == 2147483647) print "skip";
?>
--FILE--
<?php 
$res1 = stat("path_to_largefile.ext");
$res2 = stat("wfio://path_to_largefile.ext");

echo "mtime ";
echo $res1["mtime"] - $res2["mtime"];
echo "\n";

echo "size ";
echo $res1["size"] - $res2["size"];
echo "\n";

?>
--EXPECT--
mtime 0
size 4294967296
