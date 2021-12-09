#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cp.h"

int main(int argc, char *argv[])
{
    int short cpy_status, file_status;

    if (argc == 1)
        printf("No source file given !\n");
    else if (argc == 2)
        printf("No destination operand given !\n");
    
    if (argc == 3)
    {
        if (strcmp(argv[1], argv[2]) == 0)
            printf("Source and destination file are the same !\n");

        else
        {
            struct cp_handler *cp = create_cp_handler();
            if (cp == NULL)
            {
                printf("[CRITICAL] Memory error !\n");
                return -1;
            }

            file_status = verify_files(cp, argv[1], argv[2]);
            if (file_status == -1)
            {
                
                printf("[CRITICAL] Failed to read source file !\n%s for \"%s\"\n",strerror(errno),argv[1]);
                destroy_cp_handler(cp);
                return -1;
            }
            else if (file_status == -2)
            {
                
                printf("[CRITICAL] Failed to write to the destination operand !\n%s for destination \"%s\"\n", strerror(errno), argv[2]);
                destroy_cp_handler(cp);
                return -1;
            }

            else
            {
                cpy_status = bin_copy(cp, 1);
                if (cpy_status == -2)
                {
                    printf("[WARNING] Bytes differ from source to destination !\n[WARNING] File created may be corrupted !\n");
                    return -1;
                }
                else if (cpy_status == -1)
                {
                    printf("[CRITICAL] Memory error !\n");
                    return -1;
                }
                destroy_cp_handler(cp);
            }

        }
    }
    else
        printf("Usage : <cp> <source file> <destination> \n");

}