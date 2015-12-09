--TEST--
Whether wfio can detect correct workdir, when workdir contains non-ascii chars.
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "mkdir ";
$f = mkdir("ShiftJIS日本語フォルダ");
echo !!$f;
echo "\n";

echo "is_dir ";
$f = is_dir("ShiftJIS日本語フォルダ");
echo !!$f;
echo "\n";

echo "is_dir/ ";
$f = is_dir("ShiftJIS日本語フォルダ/");
echo !!$f;
echo "\n";

echo "is_dir wfio ";
$f = is_dir("wfio://\x53\x68\x69\x66\x74\x4a\x49\x53\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\xe3\x83\x95\xe3\x82\xa9\xe3\x83\xab\xe3\x83\x80");
echo !!$f;
echo "\n";

echo "is_dir/ wfio ";
$f = is_dir("wfio://\x53\x68\x69\x66\x74\x4a\x49\x53\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\xe3\x83\x95\xe3\x82\xa9\xe3\x83\xab\xe3\x83\x80/");
echo !!$f;
echo "\n";

echo "is_dir fail ";
$f = is_dir("wfio://\x53\x68\x69\x66\x74\x4a\x49\x53\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\xe3\x83\x95\xe3\x82\xa9\xe3\x83\xab\xe3\x83\x80\x31\x32\x33");
echo !$f;
echo "\n";

echo "chdir ";
$f = chdir("ShiftJIS日本語フォルダ");
echo !!$f;
echo "\n";

echo "touch ";
$f = touch("ShiftJIS日本語ファイル");
echo !!$f;
echo "\n";

echo "is_file ";
$f = is_file("ShiftJIS日本語ファイル");
echo !!$f;
echo "\n";

echo "is_file wfio ";
$f = is_file("wfio://\x53\x68\x69\x66\x74\x4A\x49\x53\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E\xE3\x83\x95\xE3\x82\xA1\xE3\x82\xA4\xE3\x83\xAB");
echo !!$f;
echo "\n";

echo "is_file wfio fail ";
$f = is_file("wfio://\x53\x68\x69\x66\x74\x4A\x49\x53\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E\xE3\x83\x95\xE3\x82\xA1\xE3\x82\xA4\xE3\x83\xAB\x31\x32\x33");
echo !$f;
echo "\n";

echo "file_exists ";
$f = file_exists("ShiftJIS日本語ファイル");
echo !!$f;
echo "\n";

echo "file_exists wfio ";
$f = file_exists("wfio://\x53\x68\x69\x66\x74\x4A\x49\x53\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E\xE3\x83\x95\xE3\x82\xA1\xE3\x82\xA4\xE3\x83\xAB");
echo !!$f;
echo "\n";

echo "file_exists wfio fail ";
$f = file_exists("wfio://\x53\x68\x69\x66\x74\x4A\x49\x53\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E\xE3\x83\x95\xE3\x82\xA1\xE3\x82\xA4\xE3\x83\xAB\x31\x32\x33");
echo !$f;
echo "\n";

echo "unlink ";
$f = unlink("ShiftJIS日本語ファイル");
echo !!$f;
echo "\n";

echo "chdir ";
$f = chdir("..");
echo !!$f;
echo "\n";

echo "rmdir ";
$f = rmdir("ShiftJIS日本語フォルダ");
echo !!$f;
echo "\n";

?>
--EXPECT--
mkdir 1
is_dir 1
is_dir/ 1
is_dir wfio 1
is_dir/ wfio 1
is_dir fail 1
chdir 1
touch 1
is_file 1
is_file wfio 1
is_file wfio fail 1
file_exists 1
file_exists wfio 1
file_exists wfio fail 1
unlink 1
chdir 1
rmdir 1