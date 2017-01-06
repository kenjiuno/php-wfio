--TEST--
wfio file funcs like filemtime
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "file_put_contents ";
$f = file_put_contents("test_filemtime.bin", "123456");
echo !!$f;
echo "\n";

$fp1 = "test_filemtime.bin";
$fp2 = "wfio://test_filemtime.bin";

echo "fileatime ";
echo fileatime($fp1) - fileatime($fp2);
echo "\n";

echo "filectime ";
echo filectime($fp1) - filectime($fp2);
echo "\n";

echo "filegroup ";
echo filegroup($fp1) - filegroup($fp2);
echo "\n";

echo "fileinode ";
echo fileinode($fp1) - fileinode($fp2);
echo "\n";

echo "filemtime ";
echo filemtime($fp1) - filemtime($fp2);
echo "\n";

echo "fileowner ";
echo fileowner($fp1) - fileowner($fp2);
echo "\n";

echo "fileperms ";
echo fileperms($fp1) - fileperms($fp2);
echo "\n";

echo "filesize ";
echo filesize($fp1) - filesize($fp2);
echo "\n";

echo "filetype ";
echo strcmp(filetype($fp1), filetype($fp2));
echo "\n";

echo "unlink ";
$f = unlink("test_filemtime.bin");
echo !!$f;
echo "\n";

?>
--EXPECT--
file_put_contents 1
fileatime 0
filectime 0
filegroup 0
fileinode 0
filemtime 0
fileowner 0
fileperms 0
filesize 0
filetype 0
unlink 1
