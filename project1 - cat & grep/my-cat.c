#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char** argv )
{
    // If no file is passed exit with error status 0
    if (argc < 2)
    {
        exit(1);
    }

    // Read the files and print them out on the console in order
    for (int index = 1; index < argc; index++)
    {
        FILE* file_pointer = fopen(argv[index], "r");
        if(!file_pointer) 
        {
            perror("cannot open file\n");
            return EXIT_FAILURE;
        }

        int c;
        while((c = getc(file_pointer)) !=EOF)
        {
            putchar(c);
        } 
        printf("\n\n");

        fclose(file_pointer);       
    }

    exit(0);
}