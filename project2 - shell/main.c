/***************************************************************************//**
  @file         main.c
  @author       Waweru, Alex Njoroge
  @date         Sunday,  30 September 2018
  @brief        Wish (Waweru Interactive SHell)
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


/**
   @brief Dynamically read a line from the standard input.
   @return string read from the standard input.
 */
char *wish_read_line()
{
    unsigned int maximum_length = 128;
    unsigned int current_size = 0;

    char *string_pointer = malloc(maximum_length);
    current_size = maximum_length;
    
    if(string_pointer != NULL)
    {
        int c = EOF;
        unsigned int i =0;

        // Accept user input until hit enter or end of file
        while (( c = getchar() ) != '\n' && c != EOF)
        {
            string_pointer[i++]=(char)c;

            // If i reached maximize_size then realloc size
            if(i == current_size)
            {
                current_size = i+maximum_length;
                string_pointer = realloc(string_pointer, current_size);
            }
        }

        // Add an implicit NULL terminator
        string_pointer[i] = '\0';

        // Free string pointer
        free(string_pointer);
        string_pointer = NULL;
    }
}


/**
   @brief Split string from the standsrd input.
   @param String read from the standard input.
   @return An array of tokens with an implicit null terminator.
 */
char **wish_tokenize_line(char *line)
{
    int string_len = strlen(line);
    int start_index = 0, end_index = 0, position = 0, bufsize = 32;
    char **tokens;
    char *token;

    // Initialize the deliminiters
    const char *delimiters[4];
    delimiters[0] = "\t";
    delimiters[1] = "\r";
    delimiters[2] = "\n";
    delimiters[3] = "\a";

    // For the case when the string is of length 0 (empty string)
    if (end_index = string_len)
    {
        return tokens;
    }

    for (end_index = 0; end_index <= string_len; end_index++)
    {
        // Check current contains a delimiter
        int is_a_token = 0;
        for (int i=0; i < 4; i++)
        {
            if ((str(end_index) == delimiters[i]) || (end_index == string_len))
            {
                is_a_token = 1;
                break;
            }
        }

        // If you encounter a deliminiter or end of line
        if (is_a_token)
        {
            token = (char *)malloc(((end_index - start_index)+1) * sizeof(char));
            tokens[position] = token;
            // Increment the start index so the next word starts after the deliminiter
            start_index = end_index + 1;
            // Increment the position
            position++;

            // Allocate memory to the tokens array dynamically 
            if (position >= bufsize) {
                bufsize += 32;
                tokens = realloc(tokens, bufsize * sizeof(char*));
                if (!tokens) {
                    fprintf(stderr, "wish: Memory allocation error\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Add an implicit null terminator
    tokens[position] = NULL;
    return tokens;
}



 

