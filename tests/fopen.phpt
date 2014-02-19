--TEST--
wfio fopen,fclose,unlink
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "wfio_fopen8 ";
$f = wfio_fopen8("wfio-fopen.bin", "wb");
echo !!$f;
echo "\n";

echo "fwrite ";
echo !!fwrite($f, "12345");
echo "\n";

echo "ftell 5 ";
echo ftell($f);
echo "\n";

echo "fseek 0,SEEK_SET ";
echo fseek($f, 0, SEEK_SET);
echo "\n";

echo "ftell 0 ";
echo ftell($f);
echo "\n";

echo "fseek 0,SEEK_END ";
echo fseek($f, 0, SEEK_END);
echo "\n";

echo "ftell 5 ";
echo ftell($f);
echo "\n";

echo "fseek -1,SEEK_CUR ";
echo fseek($f, -1, SEEK_CUR);
echo "\n";

echo "ftell 4 ";
echo ftell($f);
echo "\n";

echo "rewind ";
echo !!rewind($f);
echo "\n";

echo "ftell 0 ";
echo ftell($f);
echo "\n";

echo "fclose ";
echo !!fclose($f);
echo "\n";

echo "fopen ";
$f = fopen("wfio://wfio-fopen.bin", "rb");
echo !!$f;
echo "\n";

echo "fread ";
echo fread($f, 5);
echo "\n";

echo "fclose ";
echo !!fclose($f);
echo "\n";

echo "unlink ";
echo !!unlink("wfio://wfio-fopen.bin");
echo "\n";
?>
--EXPECT--
wfio_fopen8 1
fwrite 1
ftell 5 5
fseek 0,SEEK_SET 0
ftell 0 0
fseek 0,SEEK_END 0
ftell 5 5
fseek -1,SEEK_CUR 0
ftell 4 4
rewind 1
ftell 0 0
fclose 1
fopen 1
fread 12345
fclose 1
unlink 1