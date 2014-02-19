php-wfio
========
Unicode(UTF-8 only) filename support for PHP 5.4 on Windows

Install
-------
Get binary: https://github.com/kenjiuno/php-wfio/releases

Put php_wfio.dll into php\ext folder

Edit your php.ini

    [PHP]
    extension_dir = "ext"  ; Uncomment this line
    extension=php_wfio.dll ; Add this line

Run php.exe -m

    H:\php-5.4.25>php.exe -m
    [PHP Modules]
    ...
    wfio
    ...

    [Zend Modules]


Code Samples
------------
About file access:

    $file = wfio_fopen8("多国語.txt", "rb"); // in UTF-8
    ....
    fclose($file);

Or you can use stream wrapper:

    $file = fopen("wfio://多国語.txt", "rb"); // in UTF-8
    ....
    fclose($file);

dirstream is also supported:

    $d = opendir("wfio://C:\\Windows");
    ....
    closedir($d);

"wfio://" supports:

    fopen("wfio://多国語.txt")
    fwrite
    fread
    stat("wfio://filename")
    fclose
    
    opendir("wfio://C:\\Windows")
    readdir
    closedir
    
    rename("wfio://oldname", "wfio://newname")
    copy("wfio://source", "wfio://dest")
    unlink("wfio://filename")
    mkdir("wfio://pathname")
    rmdir("wfio://dirname")
    
    scandir("wfio://directory")
    file_get_contents("wfio://filename")
    file_put_contents("wfio://filename")

No chdir support
----------------

* "chdir" is not supported! So, keep your fullpath always like "wfio://C:\\dir1\\dir2\\dir3\\filename".
* Use wfio_getcwd8 for getting current directory
* Use wfio_path_combine to hold your subdir in fullpath format.
