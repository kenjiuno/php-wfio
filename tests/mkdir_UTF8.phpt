--TEST--
wfio mkdir,rmdir utf-8 ver
--SKIPIF--
<?php if (!extension_loaded("wfio")) print "skip"; ?>
--FILE--
<?php 

function tryscan($yours) {
	foreach (scandir("wfio://.") as $found) {
		if ($yours == $found)
			return TRUE;
	}
	return FALSE;
}

// http://www.php.net/manual/en/intro-whatis.php

echo "mkdir English ";
$f = mkdir("wfio://PHP is a widely-used open source general-purpose scripting language that is especially suited for web development and can be embedded into HTML");
echo !!$f;
echo "\n";

echo "tryscan English ";
echo !!tryscan("PHP is a widely-used open source general-purpose scripting language that is especially suited for web development and can be embedded into HTML");
echo "\n";

echo "rmdir English ";
$f = rmdir("wfio://PHP is a widely-used open source general-purpose scripting language that is especially suited for web development and can be embedded into HTML");
echo !!$f;
echo "\n";

echo "\n"; //---

echo "mkdir Japanese ";
$f = mkdir("wfio://PHPは、広く使われているオープンソースの汎用スクリプト言語です。");
echo !!$f;
echo "\n";

echo "tryscan Japanese ";
echo !!tryscan("PHPは、広く使われているオープンソースの汎用スクリプト言語です。");
echo "\n";

echo "rmdir Japanese ";
$f = rmdir("wfio://PHPは、広く使われているオープンソースの汎用スクリプト言語です。");
echo !!$f;
echo "\n";

echo "\n"; //---

echo "mkdir ChineseSimplified ";
$f = mkdir("wfio://PHP是一种被广泛应用的开放源代码的多用途脚本语言");
echo !!$f;
echo "\n";

echo "tryscan ChineseSimplified ";
echo !!tryscan("PHP是一种被广泛应用的开放源代码的多用途脚本语言");
echo "\n";

echo "rmdir ChineseSimplified ";
$f = rmdir("wfio://PHP是一种被广泛应用的开放源代码的多用途脚本语言");
echo !!$f;
echo "\n";

echo "\n"; //---

echo "mkdir French ";
$f = mkdir("wfio://PHP est un langage de scripts généraliste et Open Source");
echo !!$f;
echo "\n";

echo "tryscan French ";
echo !!tryscan("PHP est un langage de scripts généraliste et Open Source");
echo "\n";

echo "rmdir French ";
$f = rmdir("wfio://PHP est un langage de scripts généraliste et Open Source");
echo !!$f;
echo "\n";

echo "\n"; //---

echo "mkdir BrazilianPortuguese ";
$f = mkdir("wfio://PHP é uma linguagem de script open source de uso geral");
echo !!$f;
echo "\n";

echo "tryscan BrazilianPortuguese ";
echo !!tryscan("PHP é uma linguagem de script open source de uso geral");
echo "\n";

echo "rmdir BrazilianPortuguese ";
$f = rmdir("wfio://PHP é uma linguagem de script open source de uso geral");
echo !!$f;
echo "\n";

echo "\n"; //---

echo "mkdir Russian ";
$f = mkdir("wfio://PHP это распространенный язык программирования общего назначения с открытым исходным кодом");
echo !!$f;
echo "\n";

echo "tryscan Russian ";
echo !!tryscan("PHP это распространенный язык программирования общего назначения с открытым исходным кодом");
echo "\n";

echo "rmdir Russian ";
$f = rmdir("wfio://PHP это распространенный язык программирования общего назначения с открытым исходным кодом");
echo !!$f;
echo "\n";

echo "\n"; //---

?>
--EXPECT--
mkdir English 1
tryscan English 1
rmdir English 1

mkdir Japanese 1
tryscan Japanese 1
rmdir Japanese 1

mkdir ChineseSimplified 1
tryscan ChineseSimplified 1
rmdir ChineseSimplified 1

mkdir French 1
tryscan French 1
rmdir French 1

mkdir BrazilianPortuguese 1
tryscan BrazilianPortuguese 1
rmdir BrazilianPortuguese 1

mkdir Russian 1
tryscan Russian 1
rmdir Russian 1
