#include "cp.h"

void *copy_status(void *args)
{
    struct cp_handler *cp = args;

    do
    {
        printf("Status : %.2lf %% copied\r" ,((double)cp->destination->size / (double)cp->total_size)*100);
        fflush(stdout);

    }while (cp->total_size != cp->destination->size);
    

}

/*
 * return -1 : If bytes differ from source to destination
 * return 0 : If all bytes have been successfully copied to the file
*/
int write_buff(struct cp_handler *cp, char *buffer)
{
    
    int byte_written = fwrite(buffer, 1 ,cp->source->size, cp->destination->file_descriptor);
    if (byte_written != cp->source->size)
        {
            return -1;
        }
    else
    {
        if (((int)(((double)cp->destination->size / (double)cp->total_size)*100) % 25) == 0)
        {
            int fd = fileno(cp->destination->file_descriptor);
            fdatasync(fd);
            
        }
        cp->destination->size += byte_written;
        return 0;
    }

}


struct cp_handler *create_cp_handler(void)
{
    struct cp_handler *cp = malloc(sizeof(struct cp_handler));
    if (cp == NULL)
        return NULL;
    cp->destination = malloc(sizeof(struct file_dt));
    if (cp->destination == NULL)
    {
        free(cp);
        return NULL;
    }
    cp->destination->file_descriptor = NULL;
    cp->source = malloc(sizeof(struct file_dt));
    if (cp->source == NULL)
    {
        free(cp->destination);
        free(cp);
        return NULL;
    }
    cp->source->file_descriptor = NULL;
    cp->binary_buffer = NULL;
    return cp;

}

int verify_files(struct cp_handler *cp, char *source, char *destination)
{
    assert(cp != NULL && source !=NULL && destination != NULL);
    FILE *fd_src, *fd_dst;
    fd_src = fopen(source, "rb");

    /*
     * I could use a single if statement with an || to test wheter one of the file descriptor has failed
     * But, because fopen with argument "wb", tends to overwrite existing file, I want to be sure
     * That it won't touch the destination file if the source file failed to be opened !
     * And that way, I know which one failed and return an appropriate error 
     */

    if (fd_src == NULL)
        return -1;
    
    fd_dst = fopen(destination,"wb");
    if (fd_dst == NULL)
        return -2;

    cp->destination->file_descriptor = fd_dst;
    cp->source->file_descriptor = fd_src;
    cp->destination->file_name = destination;
    cp->source->file_name = source;

    return 0;

}


int bin_copy(struct cp_handler *cp, int progress)
{
    assert(cp != NULL || cp->binary_buffer != NULL);

    pthread_t progress_thread;
    if (progress == 1)
    {
        fseek(cp->source->file_descriptor, 0L, SEEK_END);
        cp->total_size = ftell(cp->source->file_descriptor);
        fseek(cp->source->file_descriptor, 0L, SEEK_SET);
        
        if (pthread_create(&progress_thread, NULL, copy_status,(void *)cp) < 0)
        {
            printf("Could not create thread !\n");
            return -1;
        }
        
    }
    char buff[BUFSIZ];
    char *binary_buff = NULL;

    long long int i = 0;
    unsigned short j;
    int status = 0;

    while (j = fread(buff, 1, BUFSIZ, cp->source->file_descriptor))
    {   
        i+= j;
        binary_buff = (char*)malloc(sizeof(char) *j);
        if (binary_buff == NULL)
            return -1;
            
        memcpy(binary_buff, buff, j);
        cp->source->size = j;
        status = write_buff(cp, binary_buff);
        if (status == -1)
            return -2;
        free(binary_buff);

    }

    cp->source->size = i;
    pthread_join(progress_thread, NULL);
}

void destroy_cp_handler(struct cp_handler *cp)
{
    assert(cp != NULL);

    if (cp->source != NULL)
    {
        
        if (cp->source->file_descriptor != NULL)
        {
            fclose(cp->source->file_descriptor);
        }
        
        free(cp->source);

        
    
    }
    if (cp->destination != NULL)
    {
        
        if (cp->destination->file_descriptor != NULL)
        {
            fclose(cp->destination->file_descriptor);
        }
        free(cp->destination);
    }

    
    if (cp->binary_buffer != NULL)
        free(cp->binary_buffer);
    
    free(cp);
}
