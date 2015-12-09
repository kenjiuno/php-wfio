--TEST--
wfio scandir crashes on absent
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 
echo "scandir";
@scandir("wfio://4f09352e-e3e3-40f8-895e-701349db8836");
echo "1\n";

echo "scandir";
@scandir("wfio://76b24348-170d-471e-b2d9-8086fc42d2ec");
echo "2\n";

echo "scandir";
@scandir("wfio://00bcb24a-fb58-4484-826a-01c65f4c56b7");
echo "3\n";

?>
--EXPECT--
scandir1
scandir2
scandir3
