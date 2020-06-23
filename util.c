#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include "util.h"


int print_error(const char *format, ...)
{
	int ret;
	va_list aptr;

	va_start(aptr, format);
	fprintf(stderr, "%s%sERROR:%s ", COL_BLD, COL_RED, COL_RST);
	ret = vfprintf(stderr, format, aptr);
	fprintf(stderr, "\n");
	va_end(aptr);

	return ret;
}

int print_error_errno(const char *format, ...)
{
	int ret;
	va_list aptr;

	va_start(aptr, format);
	fprintf(stderr, "%s%sERROR:%s ", COL_BLD, COL_RED, COL_RST);
	ret = vfprintf(stderr, format, aptr);
	fprintf(stderr, ": %s\n", strerror(errno));
	va_end(aptr);

	return ret;
}

int redirect_file(char *file, int fd_old)
{
    int fd = -1;
	if(file == NULL)
		return -1;

	if(fd_old == STDIN_FILENO)
		fd = open(file, O_RDONLY);
	else if(fd_old == STDOUT_FILENO)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
	{
		print_error("Invalid file descriptor given");
		return 1;
	}

	if(fd < 0)
	{
		print_error_errno("Cannot open file '%s'", file);
		return 1;
	}

	if(dup2(fd, fd_old) < 0)
	{
		print_error_errno("Cannot duplicate file descriptor %i", fd_old);
		return 1;
	}

	close(fd);
	return 0;
}

int redirect_pipe(int fd_new, int fd_old)
{
	if(dup2(fd_new, fd_old) < 0)
	{
		print_error("Cannot duplicate file descriptor %i", fd_new);
		return 1;
	}

	close(fd_new);
	return 0;
}

