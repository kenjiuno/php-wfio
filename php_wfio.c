/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: kenji uno <user@kkdf2.sakura.ne.jp>                          |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_filestat.h"
#include "php_wfio.h"
#include "readdirw.h"
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

struct php_wfio_stream_data_t	{
	FILE *pf;
};

/* If you declare any globals in php_wfio.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(wfio)
*/

/* True global resources - no need for thread safety here */
static int le_wfio;

ZEND_BEGIN_ARG_INFO(arginfo_wfio_fopen8, 0)
	ZEND_ARG_INFO(0, file)
	ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wfio_getcwd8, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wfio_path2utf8, 0)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wfio_path_from_utf8, 0)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wfio_path_combine, 0)
	ZEND_ARG_INFO(0, dir)
	ZEND_ARG_INFO(0, file)
ZEND_END_ARG_INFO()

/* {{{ wfio_functions[]
 *
 * Every user visible function must have an entry in wfio_functions[].
 */
const zend_function_entry wfio_functions[] = {
	PHP_FE(wfio_fopen8, arginfo_wfio_fopen8)
	PHP_FE(wfio_getcwd8, arginfo_wfio_getcwd8)
	PHP_FE(wfio_path2utf8, arginfo_wfio_path2utf8)
	PHP_FE(wfio_path_from_utf8, arginfo_wfio_path_from_utf8)
	PHP_FE(wfio_path_combine, arginfo_wfio_path_combine)
	PHP_FE_END	/* Must be the last line in wfio_functions[] */
};
/* }}} */

/* {{{ wfio_module_entry
 */
zend_module_entry wfio_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"wfio",
	wfio_functions,
	PHP_MINIT(wfio),
	PHP_MSHUTDOWN(wfio),
	PHP_RINIT(wfio),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(wfio),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(wfio),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WFIO
ZEND_GET_MODULE(wfio)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("wfio.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_wfio_globals, wfio_globals)
    STD_PHP_INI_ENTRY("wfio.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_wfio_globals, wfio_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_wfio_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_wfio_init_globals(zend_wfio_globals *wfio_globals)
{
	wfio_globals->global_value = 0;
	wfio_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(wfio)
{
	php_register_url_stream_wrapper("wfio", &php_stream_wfio_wrapper TSRMLS_CC);
	
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(wfio)
{
	php_unregister_url_stream_wrapper("wfio" TSRMLS_CC);
	
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(wfio)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(wfio)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wfio)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wfio support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


static int wfio_realpathw(wchar_t *relpath, size_t relpath_len TSRMLS_DC) {
	char *p;
	char workdir[MAXPATHLEN] = {0};
	wchar_t workdirw[MAX_PATH] = {0};
	wchar_t newpathw[MAX_PATH] = {0};

	if (relpath[0] == L'\\' || relpath[0] == L'/')
		return 0;

	p = VCWD_GETCWD(workdir, MAXPATHLEN);

	if (p != NULL) {
		MultiByteToWideChar(CP_UTF8, 0, workdir, MAXPATHLEN, workdirw, MAX_PATH);
		PathCombineW(newpathw, workdirw, relpath);
		wcsncpy(relpath, newpathw, relpath_len);
		return 0;
	} else {
		return -1;
	}
}

/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

static size_t php_wfiop_read(php_stream *stream, char *buf, size_t count TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;
	size_t read;
	
	read = fread(buf, 1, count, self->pf);
	
	if (feof(self->pf) != 0) {
		stream->eof = 1;
	}
	
	return (read < 0) ? 0 : read;
}

static size_t php_wfiop_write(php_stream *stream, const char *buf, size_t count TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;
	int wrote;

	wrote = fwrite((char *) buf, 1, count, self->pf);

	return (wrote < 0) ? 0 : wrote;
}

static int php_wfiop_seek(php_stream *stream, off_t offset, int whence, off_t *newoffs TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;

	assert(self != NULL);

	_fseeki64(self->pf, offset, whence);
	
	*newoffs = (off_t) _ftelli64(self->pf);

	return (*newoffs < 0) ? -1 : 0;
}

static int php_wfiop_stat(php_stream *stream, php_stream_statbuf *ssb TSRMLS_DC)
{
	// by php's fstat
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;

	if (!self) {
		return -1;
	}

	return _fstat(_fileno(self->pf), &ssb->sb);
}

static int php_wfiop_close(php_stream *stream, int close_handle TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;
	int ret = EOF;
	
	if (close_handle) {
		if (self->pf != NULL) {
			ret = fclose(self->pf);
			self->pf  = NULL;
		}
	}
	efree(self);

	return ret;
}

static int php_wfiop_flush(php_stream *stream TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self = (struct php_wfio_stream_data_t *) stream->abstract;

	return fflush(self->pf);
}

php_stream_ops php_stream_wfio_ops = {
	php_wfiop_write, php_wfiop_read,
	php_wfiop_close, php_wfiop_flush,
	"WFIO",
	php_wfiop_seek, 
	NULL, /* cast */
	php_wfiop_stat, /* stat */
	NULL  /* set_option */
};

php_stream *php_wfio_stream_opener(php_stream_wrapper *wrapper, char *path, char *mode, int options, 
                                  char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC)
{
	struct php_wfio_stream_data_t *self;
	php_stream *stream;
	
	wchar_t pathw[WFIO_MAX_PATH] = {0};
	wchar_t fp_pathw[WFIO_MAX_PATH] = {0};
	wchar_t modew[WFIO_MAX_PATH] = {0};
	
	if (strncasecmp("wfio://", path, 7) == 0) {
		path += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, path, -1, pathw, WFIO_MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, mode, -1, modew, WFIO_MAX_PATH);

	wfio_realpathw(pathw, WFIO_MAX_PATH TSRMLS_CC);
	
	self = emalloc(sizeof(*self));
	self->pf = _wfopen(pathw, modew);

	if (self->pf != NULL) {
		stream = php_stream_alloc_rel(&php_stream_wfio_ops, self, 0, mode);
		if (stream) {
			stream->flags |= PHP_STREAM_FLAG_NO_BUFFER;
			return stream;
		}

		fclose(self->pf);
	}

	efree(self);
	if (options & REPORT_ERRORS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "wfio_fopen8 failed");
	}

	return NULL;
}

static int php_wfio_url_stater(php_stream_wrapper *wrapper, char *url, int flags, php_stream_statbuf *ssb, php_stream_context *context TSRMLS_DC)
{
	wchar_t urlw[WFIO_MAX_PATH] = {0};
	struct _stat64 sb;
	
	if (strncmp(url, "wfio://", 7) == 0) {
		url += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, url, -1, urlw, WFIO_MAX_PATH);
	
	wfio_realpathw(urlw, WFIO_MAX_PATH TSRMLS_CC);
	
	if (_wstat64(urlw, &sb) != 0)
		return -1;

	ssb->sb.st_gid = sb.st_gid;
	ssb->sb.st_atime = sb.st_atime;
	ssb->sb.st_ctime = sb.st_ctime;
	ssb->sb.st_dev = sb.st_dev;
	ssb->sb.st_ino = sb.st_ino;
	ssb->sb.st_mode = sb.st_mode;
	ssb->sb.st_mtime = sb.st_mtime;
	ssb->sb.st_nlink = sb.st_nlink;
	ssb->sb.st_rdev = sb.st_rdev;
	ssb->sb.st_size = sb.st_size;
	ssb->sb.st_uid = sb.st_uid;
	
	return 0;
}

/* {{{ plain files opendir/readdir implementation */
static size_t php_wfio_dirstream_read(php_stream *stream, char *buf, size_t count TSRMLS_DC)
{
	DIRW *dir = (DIRW*)stream->abstract;
	/* avoid libc5 readdir problems */
	struct direntw entry;
	struct direntw *result = (struct direntw *)&entry;
	php_stream_dirent *ent = (php_stream_dirent*)buf;

	/* avoid problems if someone mis-uses the stream */
	if (count != sizeof(php_stream_dirent))
		return 0;

	if (readdirw_r(dir, &entry, &result) == 0 && result != NULL) {
		WideCharToMultiByte(CP_UTF8, 0, result->d_name, -1, ent->d_name, sizeof(ent->d_name), NULL, NULL);
		return sizeof(php_stream_dirent);
	}
	return 0;
}

static int php_wfio_dirstream_close(php_stream *stream, int close_handle TSRMLS_DC)
{
	return closedirw((DIRW *)stream->abstract);
}

static int php_wfio_dirstream_rewind(php_stream *stream, off_t offset, int whence, off_t *newoffs TSRMLS_DC)
{
	rewinddirw((DIRW *)stream->abstract);
	return 0;
}

static php_stream_ops	php_wfio_dirstream_ops = {
	NULL, php_wfio_dirstream_read,
	php_wfio_dirstream_close, NULL,
	"wfio_dir",
	php_wfio_dirstream_rewind,
	NULL, /* cast */
	php_wfiop_stat, /* stat */
	NULL  /* set_option */
};

static php_stream *php_wfio_dir_opener(php_stream_wrapper *wrapper, char *path, char *mode,
		int options, char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC)
{
	DIRW *dir;
	php_stream *stream;
	wchar_t pathw[WFIO_MAX_PATH] = {0};

	if (strncmp(path, "wfio://", 7) == 0) {
		path += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, path, -1, pathw, WFIO_MAX_PATH);

	wfio_realpathw(pathw, WFIO_MAX_PATH TSRMLS_CC);

	dir = opendirw(pathw);

#ifdef PHP_WIN32
	if (!dir) {
		php_win32_docref2_from_error(GetLastError(), path, path TSRMLS_CC);
	}

	if (dir && dir->finished) {
		closedirw(dir);
		dir = NULL;
	}
#endif
	if (dir) {
		stream = php_stream_alloc(&php_wfio_dirstream_ops, dir, 0, mode);
		if (stream == NULL)
			closedirw(dir);
	}

	return stream;
}

static int php_wfio_unlink(php_stream_wrapper *wrapper, char *url, int options, php_stream_context *context TSRMLS_DC)
{
	int ret;
	wchar_t urlw[WFIO_MAX_PATH] = {0};

	if (strncmp(url, "wfio://", 7) == 0) {
		url += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, url, -1, urlw, WFIO_MAX_PATH);

	wfio_realpathw(urlw, WFIO_MAX_PATH TSRMLS_CC);

	ret = _wunlink(urlw);
	if (ret == -1) {
		if (options & REPORT_ERRORS) {
			php_error_docref1(NULL TSRMLS_CC, url, E_WARNING, "%s", strerror(errno));
		}
		return 0;
	}

	/* Clear stat cache (and realpath cache) */
	php_clear_stat_cache(1, NULL, 0 TSRMLS_CC);

	return 1;
}

static int php_wfio_rename(php_stream_wrapper *wrapper, char *url_from, char *url_to, int options, php_stream_context *context TSRMLS_DC)
{
	int ret;
	wchar_t urlw_from[WFIO_MAX_PATH] = {0};
	wchar_t urlw_to[WFIO_MAX_PATH] = {0};

	if (!url_from || !url_to) {
		return 0;
	}

	if (strncmp(url_from, "wfio://", 7) == 0) {
		url_from += 7;
	}

	if (strncmp(url_to, "wfio://", 7) == 0) {
		url_to += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, url_from, -1, urlw_from, WFIO_MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, url_to, -1, urlw_to, WFIO_MAX_PATH);

	wfio_realpathw(urlw_from, WFIO_MAX_PATH TSRMLS_CC);
	wfio_realpathw(urlw_to, WFIO_MAX_PATH TSRMLS_CC);

	ret = _wrename(urlw_from, urlw_to);

	if (ret == -1) {
		php_win32_docref2_from_error(GetLastError(), url_from, url_to TSRMLS_CC);
		return 0;
	}

	/* Clear stat cache (and realpath cache) */
	php_clear_stat_cache(1, NULL, 0 TSRMLS_CC);

	return 1;
}

static int php_wfio_mkdir(php_stream_wrapper *wrapper, char *dir, int mode, int options, php_stream_context *context TSRMLS_DC)
{
	int ret, recursive = options & PHP_STREAM_MKDIR_RECURSIVE;
	wchar_t dirw[WFIO_MAX_PATH] = {0};

	if (strncmp(dir, "wfio://", 7) == 0) {
		dir += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, dir, -1, dirw, WFIO_MAX_PATH);

	wfio_realpathw(dirw, WFIO_MAX_PATH TSRMLS_CC);

	ret = _wmkdir(dirw);
	if (ret < 0) {
		/* Failure */
		return 0;
	} else {
		/* Success */
		return 1;
	}
}

static int php_wfio_rmdir(php_stream_wrapper *wrapper, char *url, int options, php_stream_context *context TSRMLS_DC)
{
	int url_len = strlen(url);
	wchar_t urlw[WFIO_MAX_PATH] = {0};

	if (strncmp(url, "wfio://", 7) == 0) {
		url += 7;
	}

	MultiByteToWideChar(CP_UTF8, 0, url, -1, urlw, WFIO_MAX_PATH);

	wfio_realpathw(urlw, WFIO_MAX_PATH TSRMLS_CC);

	if (_wrmdir(urlw) < 0) {
		php_error_docref1(NULL TSRMLS_CC, url, E_WARNING, "%s", strerror(errno));
		return 0;
	}

	/* Clear stat cache (and realpath cache) */
	php_clear_stat_cache(1, NULL, 0 TSRMLS_CC);

	return 1;
}

PHP_FUNCTION(wfio_fopen8)
{
	char *fname = NULL;
	int fname_len = 0;
	char *fmode = NULL;
	int fmode_len = 0;
	php_stream *stream = NULL;
	int flags = REPORT_ERRORS;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &fname, &fname_len, &fmode, &fmode_len) == FAILURE) {
		return;
	}
	
	stream = php_wfio_stream_opener(NULL, fname, fmode, flags, NULL, NULL STREAMS_CC TSRMLS_CC);

	if (!stream) {
		RETURN_FALSE;
	}
	php_stream_to_zval(stream, return_value);
}

/* {{{ proto mixed getcwd(void)
   Gets the current directory */
PHP_FUNCTION(wfio_getcwd8)
{
	wchar_t pathw[WFIO_MAX_PATH] = {0};
	char path[MAXPATHLEN] = {0};
	char buffer[MAXPATHLEN] = {0};
	char *p;
	
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	p = VCWD_GETCWD(buffer, MAXPATHLEN);
	if (p != NULL) {
		MultiByteToWideChar(CP_ACP, 0, buffer, MAXPATHLEN, pathw, WFIO_MAX_PATH);
		WideCharToMultiByte(CP_UTF8, 0, pathw, -1, path, MAXPATHLEN, NULL, NULL);
		RETURN_STRING(path, 1);
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

PHP_FUNCTION(wfio_path2utf8)
{
	char *path = NULL;
	int path_len = 0;
	wchar_t pathw[WFIO_MAX_PATH] = {0};
	char fp_path[WFIO_MAX_PATH] = {0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	MultiByteToWideChar(CP_ACP, 0, path, path_len, pathw, WFIO_MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, pathw, -1, fp_path, MAXPATHLEN, NULL, NULL);

	RETURN_STRING(fp_path, 1);
}

PHP_FUNCTION(wfio_path_from_utf8)
{
	char *path = NULL;
	int path_len = 0;
	wchar_t pathw[WFIO_MAX_PATH] = {0};
	char fp_path[WFIO_MAX_PATH] = {0};
	BOOL fUsedDefaultChar = FALSE;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
		return;
	}

	MultiByteToWideChar(CP_UTF8, 0, path, path_len, pathw, WFIO_MAX_PATH);
	WideCharToMultiByte(CP_ACP, 0, pathw, -1, fp_path, MAXPATHLEN, NULL, &fUsedDefaultChar);

	if (fUsedDefaultChar) {
		RETURN_FALSE;
	}
	RETURN_STRING(fp_path, 1);
}

PHP_FUNCTION(wfio_path_combine)
{
	char *dir = NULL;
	int dir_len = 0;
	char *file = NULL;
	int file_len = 0;
	wchar_t dirw[WFIO_MAX_PATH] = {0};
	wchar_t filew[WFIO_MAX_PATH] = {0};
	wchar_t destw[WFIO_MAX_PATH] = {0};
	char dest[WFIO_MAX_PATH] = {0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &dir, &dir_len, &file, &file_len) == FAILURE) {
		return;
	}

	MultiByteToWideChar(CP_UTF8, 0, dir, dir_len, dirw, WFIO_MAX_PATH);
	MultiByteToWideChar(CP_UTF8, 0, file, file_len, filew, WFIO_MAX_PATH);
	
	if (file[0] == L'\\' || file[0] == L'/') {
		wcsncpy(destw, filew, WFIO_MAX_PATH);
	}
	else if (!PathCombineW(destw, dirw, filew)) {
		RETURN_FALSE;
	}

	WideCharToMultiByte(CP_UTF8, 0, destw, -1, dest, MAXPATHLEN, NULL, NULL);

	RETURN_STRING(dest, 1);
}

static php_stream_wrapper_ops wfio_stream_wops = {
	php_wfio_stream_opener,
	NULL, /* close */
	NULL, /* stat */
	php_wfio_url_stater, /* stat_url */
	php_wfio_dir_opener, /* opendir */
	"WFIO",
	php_wfio_unlink, /* unlink */
	php_wfio_rename, /* rename */
	php_wfio_mkdir, /* mkdir */
	php_wfio_rmdir, /* rmdir */
};

php_stream_wrapper php_stream_wfio_wrapper =	{
	&wfio_stream_wops,
	NULL,
	0, /* is_url */
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
