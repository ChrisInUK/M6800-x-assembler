#if !defined C_FFWD_H
#define C_FFWD_H
// Added: CDC: 2018

#define	FILEMODE	0644	/* file creat mode */
#define	UPDATE		2	/* file open mode */
#define	ABS		0	/* absolute seek */


// Templates...

int fwdinit();
int fwdreinit();
int fwdmark();
int fwdnext();


#endif
