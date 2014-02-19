dnl $Id$
dnl config.m4 for extension wfio

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(wfio, for wfio support,
dnl Make sure that the comment is aligned:
dnl [  --with-wfio             Include wfio support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(wfio, whether to enable wfio support,
dnl Make sure that the comment is aligned:
dnl [  --enable-wfio           Enable wfio support])

if test "$PHP_WFIO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-wfio -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/wfio.h"  # you most likely want to change this
  dnl if test -r $PHP_WFIO/$SEARCH_FOR; then # path given as parameter
  dnl   WFIO_DIR=$PHP_WFIO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for wfio files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       WFIO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$WFIO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the wfio distribution])
  dnl fi

  dnl # --with-wfio -> add include path
  dnl PHP_ADD_INCLUDE($WFIO_DIR/include)

  dnl # --with-wfio -> check for lib and symbol presence
  dnl LIBNAME=wfio # you may want to change this
  dnl LIBSYMBOL=wfio # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $WFIO_DIR/lib, WFIO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_WFIOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong wfio lib version or lib not found])
  dnl ],[
  dnl   -L$WFIO_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(WFIO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(wfio, wfio.c, $ext_shared)
fi
