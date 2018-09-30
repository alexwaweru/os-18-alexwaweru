#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char** argv )
{
    // If no file is passed accept user input
    if (argc < 3)
    {
        unsigned int maximum_length = 128;
        unsigned int current_size = 0;
    
        char *string_pointer = malloc(maximum_length);
        current_size = maximum_length;

        printf("Enter a string to search from: ");
        
        if(string_pointer != NULL)
        {
            int c = EOF;
            unsigned int i =0;

            //accept user input until hit enter or end of file
            while (( c = getchar() ) != '\n' && c != EOF)
            {
                string_pointer[i++]=(char)c;

                //if i reached maximize_size then realloc size
                if(i == current_size)
                {
                    current_size = i+maximum_length;
                    string_pointer = realloc(string_pointer, current_size);
                }
            }

            string_pointer[i] = '\0';

            if (strstr(string_pointer, argv[1]))
            {
                printf("%s", string_pointer);
            }
            //free string pointer
            free(string_pointer);
            string_pointer = NULL;
        }
        return 0;
    }

    // If file is passed read the files and print them out on the console in order
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
                printf("\n");
            }
        }

        fclose(file_pointer);
          
        if (line)
        {
            free(line);
        }

        if (file_pointer) 
        {
            free(file_pointer);
        }
    }

    exit(0);
}