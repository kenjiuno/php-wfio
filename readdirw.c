#include <malloc.h>
#include <string.h>
#include <errno.h>

#include "php.h"
#include "readdirw.h"
/**********************************************************************
 * Implement dirent-style 
 *   opendirw
 *   readdirw
 *   rewinddirw
 *   closedirw on Win32
 *
 * Functions will require Windows NT due to Unicode support.
 * 
 * Windows 95/98/Me users don't require this,
 * because their OS doesn't have Unicode file support.
 **********************************************************************/

DIRW *opendirw(const wchar_t *dir)
{
	DIRW *dp;
	wchar_t *filespec;
	HANDLE handle;
	int index;
	wchar_t resolved_path_buff[MAXPATHLEN];

	wcsncpy(resolved_path_buff, dir, MAXPATHLEN);

	filespec = (wchar_t *)malloc(sizeof(wchar_t) * (wcslen(resolved_path_buff) + 2 + 1));
	if (filespec == NULL) {
		return NULL;
	}
	wcscpy(filespec, resolved_path_buff);
	index = wcslen(filespec) - 1;
	if (index >= 0 && (filespec[index] == L'/' || (filespec[index] == L'\\')))
		filespec[index] = L'\0';
	wcscat(filespec, L"\\*");

	dp = (DIRW *) malloc(sizeof(DIRW));
	if (dp == NULL) {
		return NULL;
	}
	dp->offset = 0;
	dp->finished = 0;

	if ((handle = FindFirstFileW(filespec, &(dp->fileinfo))) == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		if (err == ERROR_NO_MORE_FILES || err == ERROR_FILE_NOT_FOUND) {
			dp->finished = 1;
		} else {
			free(dp);
			free(filespec);
			return NULL;
		}
	}
	dp->dir = wcsdup(resolved_path_buff);
	dp->handle = handle;
	free(filespec);

	return dp;
}

struct direntw *readdirw(DIRW *dp)
{
	if (!dp || dp->finished)
		return NULL;

	if (dp->offset != 0) {
		if (FindNextFileW(dp->handle, &(dp->fileinfo)) == 0) {
			dp->finished = 1;
			return NULL;
		}
	}
	dp->offset++;

	wcsncpy(dp->dent.d_name, dp->fileinfo.cFileName, _MAX_FNAME+1);
	dp->dent.d_ino = 1;
	dp->dent.d_reclen = wcslen(dp->dent.d_name);
	dp->dent.d_off = dp->offset;

	return &(dp->dent);
}

int readdirw_r(DIRW *dp, struct direntw *entry, struct direntw **result)
{
	if (!dp || dp->finished) {
		*result = NULL;
		return 0;
	}

	if (dp->offset != 0) {
		if (FindNextFileW(dp->handle, &(dp->fileinfo)) == 0) {
			dp->finished = 1;
			*result = NULL;
			return 0;
		}
	}
	dp->offset++;

	wcsncpy(dp->dent.d_name, dp->fileinfo.cFileName, _MAX_FNAME+1);
	dp->dent.d_ino = 1;
	dp->dent.d_reclen = wcslen(dp->dent.d_name);
	dp->dent.d_off = dp->offset;

	memcpy(entry, &dp->dent, sizeof(*entry));

	*result = &dp->dent;

	return 0;
}

int closedirw(DIRW *dp)
{
	if (!dp)
		return 0;
	/* It is valid to scan an empty directory but we have an invalid
	   handle in this case (no first file found). */
	if (dp->handle != INVALID_HANDLE_VALUE) {
		FindClose(dp->handle);
	}
	if (dp->dir)
		free(dp->dir);
	if (dp)
		free(dp);

	return 0;
}

int rewinddirw(DIRW *dp)
{
	/* Re-set to the beginning */
	wchar_t *filespec;
	HANDLE handle;
	int index;

	FindClose(dp->handle);

	dp->offset = 0;
	dp->finished = 0;

	filespec = (wchar_t *)malloc(sizeof(wchar_t) * (wcslen(dp->dir) + 2 + 1));
	if (filespec == NULL) {
		return -1;
	}

	wcscpy(filespec, dp->dir);
	index = wcslen(filespec) - 1;
	if (index >= 0 && (filespec[index] == L'/' || (filespec[index] == L'\\')))
		filespec[index] = L'\0';
	wcscat(filespec, L"/*");

	if ((handle = FindFirstFileW(filespec, &(dp->fileinfo))) == INVALID_HANDLE_VALUE) {
		dp->finished = 1;
	}
	
	dp->handle = handle;
	free(filespec);

	return 0;
}
