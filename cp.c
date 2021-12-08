#include "cp.h"


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
     * I could use a single if statement with an || to dest wheter one of the file descriptor has failed
     * But, because fopen with argument "wb", tends to overwrite existing file, I want to be sure
     * That it won't touch the destination file if the source file failed to be opened !
     * And that way, I know which failed and return an appropriate error 
     */

    if (fd_src == NULL)
        return -1;
    
    fd_dst = fopen(destination, "wb");
    if (fd_dst == NULL)
        return -2;
    
    cp->destination->file_descriptor = fd_dst;
    cp->source->file_descriptor = fd_src;
    cp->destination->file_name = destination;
    cp->source->file_name = source;

    return 0;

}

int bin_read(struct cp_handler *cp)
{
    assert(cp != NULL);

    char buf[1000] = {0};
    int i = 0, j;
    char *binary_buff = (char*)malloc(sizeof(char)*STEPSIZE);
    if (binary_buff == NULL)
        return -1;

    while (j = fread(buf, 1, STEPSIZE, cp->source->file_descriptor))
    {
        i+= j;
        binary_buff = (char*)realloc(binary_buff, sizeof(char)*i);
        if (binary_buff == NULL)
            return -1;
        
        memcpy(binary_buff + (i-j), buf, j);
    }

    cp->binary_buffer = binary_buff;
    cp->source->size = i;
    
    return 0;
}

int bin_copy(struct cp_handler *cp)
{
    assert(cp != NULL || cp->binary_buffer != NULL);
    cp->destination->size = fwrite(cp->binary_buffer, 1, cp->source->size, cp->destination->file_descriptor);
    if (cp->destination->size != cp->source->size)
        {
            return -1;
        }
    else
        return 0;

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