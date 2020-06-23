#ifndef _UTIL_H_
#define _UTIL_H_


#define COL_RST  "\x1B[0m"
#define COL_BLD  "\x1B[1m"
#define COL_RED  "\x1B[31m"
#define COL_GRN  "\x1B[32m"
#define COL_YEL  "\x1B[33m"
#define COL_BLU  "\x1B[34m"
#define COL_MAG  "\x1B[35m"
#define COL_CYN  "\x1B[36m"
#define COL_WHT  "\x1B[37m"

/* open/fcntl.  */
#define O_ACCMODE	   0003
#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#ifndef O_CREAT
# define O_CREAT	   0100	/* Not fcntl.  */
#endif
#ifndef O_EXCL
# define O_EXCL		   0200	/* Not fcntl.  */
#endif
#ifndef O_NOCTTY
# define O_NOCTTY	   0400	/* Not fcntl.  */
#endif
#ifndef O_TRUNC
# define O_TRUNC	  01000	/* Not fcntl.  */
#endif
#ifndef O_APPEND
# define O_APPEND	  02000
#endif
#ifndef O_NONBLOCK
# define O_NONBLOCK	  04000
#endif
#ifndef O_NDELAY
# define O_NDELAY	O_NONBLOCK
#endif
#ifndef O_SYNC
# define O_SYNC	       04010000
#endif
#define O_FSYNC		O_SYNC
#ifndef O_ASYNC
# define O_ASYNC	 020000
#endif
#ifndef __O_LARGEFILE
# define __O_LARGEFILE	0100000
#endif

#ifndef __O_DIRECTORY
# define __O_DIRECTORY	0200000
#endif
#ifndef __O_NOFOLLOW
# define __O_NOFOLLOW	0400000
#endif
#ifndef __O_CLOEXEC
# define __O_CLOEXEC   02000000
#endif
#ifndef __O_DIRECT
# define __O_DIRECT	 040000
#endif
#ifndef __O_NOATIME
# define __O_NOATIME   01000000
#endif
#ifndef __O_PATH
# define __O_PATH     010000000
#endif
#ifndef __O_DSYNC
# define __O_DSYNC	 010000
#endif
#ifndef __O_TMPFILE
# define __O_TMPFILE   (020000000 | __O_DIRECTORY)
#endif


enum {
	PIPE_IN = 0,
	PIPE_OUT,
	PIPE_ARRAY
};



/**
 * Print an error message on stderr
 *
 * @param format: a string which contains format
 * @return the number of characters printed
 *
 **/
int print_error(const char *format, ...);

/**
 * Print an error message on stderr and add a string describing error number
 * used to print errno string text.
 *
 * @param format: a string which contains format
 * @return the number of characters printed
 *
 **/
int print_error_errno(const char *format, ...);


/**
 * Redirect a file into a file descriptor,
 * it checks the i/o stream to chose which mode to open the file
 *  it used to redirect the input/output (<,>) to/from a file
 * 
 * @param file:   a string which contains the filename or path
 * @param fd_new: a pointer into a new file descriptor
 * @param fd_old: the file descriptor to override
 * @return zero value on successful
 *
 **/
int redirect_file(char *file, int fd_old);

/**
 * Redirect a standard stream in a pipe
 * use to redirect output/input of linked pipes between process
 *
 * @param fd_new: a piped stream
 * @param fd_out: a standard stream
 * @return zero value on successful
 *
 **/
int redirect_pipe(int fd_new, int fd_old);


#endif
