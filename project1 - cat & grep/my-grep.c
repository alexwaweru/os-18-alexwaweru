#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char** argv )
{
    // If no file is passed exit with error status 0
    if (argc < 3)
    {
        exit(1);
    }

    // Read the files and print them out on the console in order
    for (int index = 2; index < argc; index++)
    {
        FILE* file_pointer = fopen(argv[index], "r");
        if(!file_pointer) 
        {
            perror("cannot open file\n");
            return EXIT_FAILURE;
        }

        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, file_pointer)) != -1) 
        {
            if (strstr(line, argv[1]))
            {
                printf("%s",line);
            }
        }
        
        fclose(file_pointer);  
        if (line)
        {
            free(line);
        }     
    }

    exit(0);
}