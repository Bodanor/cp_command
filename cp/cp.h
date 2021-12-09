#ifndef __CP_H__
#define __CP_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <syscall.h>

struct file_dt{
    FILE *file_descriptor;
    char *file_name;
    long long size;
};

struct cp_handler{
    struct file_dt *source;
    struct file_dt *destination;
    char *binary_buffer;
    long long total_size;
};


/* 
 * return pointer to struct cp_handler
 * return NULL if malloc error
*/


struct cp_handler *create_cp_handler(void);


void destroy_cp_handler(struct cp_handler *cp);
/*
 * return -1 : if error I/O on source file
 * return -2 : if error I/O on destination file
 * return 0 : if everything went well !
*/
int verify_files(struct cp_handler *cp, char *source, char *destination);

/*
 * return -1 : If malloc error
 * return -2 : If bytes copied are is different from the source file descriptor
 * return 0 : If bytes are the same from the source file descriptor
 * 
*/ 
int bin_copy(struct cp_handler *cp, int progress);

#endif