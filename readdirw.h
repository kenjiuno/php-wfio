#ifndef READDIRW_H
#define READDIRW_H


/*
 * Structures and types used to implement opendirw/readdirw/closedirw
 * on Windows NT.
 */

#include <windows.h>

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <direct.h>

/* struct dirent - same as Unix */

struct direntw {
	long d_ino;					/* inode (always 1 in WIN32) */
	off_t d_off;				/* offset to this dirent */
	unsigned short d_reclen;	/* length of d_name */
	wchar_t d_name[_MAX_FNAME + 1];	/* filename (null terminated) */
};


/* typedef DIR - not the same as Unix */
typedef struct {
	HANDLE handle;				/* _findfirst/_findnext handle */
	short offset;				/* offset into directory */
	short finished;				/* 1 if there are not more files */
	WIN32_FIND_DATAW fileinfo;	/* from _findfirst/_findnext */
	wchar_t *dir;				/* the dir we are reading */
	struct direntw dent;		/* the dirent to return */
} DIRW;

/* Function prototypes */
DIRW *opendirw(const wchar_t *);
struct direntw *readdir8(DIRW *);
int readdirw_r(DIRW *, struct direntw *, struct direntw **);
int closedirw(DIRW *);
int rewinddirw(DIRW *);

#endif /* READDIRW_H */
