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
#include <dirent.h>
#include <sys/stat.h>


/**
   @brief Dynamically read a line from the standard input.
   @return string read from the standard input.
 */
char *wish_read_line()
{
    int maximum_length = 128;
    int current_size = 0;
    char *string_pointer = malloc(sizeof(char) * maximum_length);
    current_size = maximum_length;
    // Check for successful memory allocation
    if(!string_pointer){
        printf("\033[1;31m");
        fprintf(stderr, "Memory Allocation Error!\n");
        printf("\033[0m");
        exit(EXIT_FAILURE);
    }

    int c;
    int index = 0;
    // Accept user input until hit enter or end of file
    while(1){
        c = getchar();
        if(c == EOF || c == '\n'){
            string_pointer[index] = '\0';
            return string_pointer;
        } else {
            string_pointer[index] = c;
        }
        index++;
        // If index reached maximize_size then reallocate size dynamically
        if(index == current_size){
            current_size = index+maximum_length;
            string_pointer = realloc(string_pointer, current_size);
            // Check for proper memory allocation
            if (!string_pointer){
                printf("\033[1;31m");
                fprintf(stderr, "Memory Allocation Error!\n");
                printf("\033[0m");
                exit(EXIT_FAILURE);
            }
        }
    }
}


/**
   @brief Scan string from the standard input and split using given delimiter.
   @param String read from the standard input.
   @return An array of tokens with an implicit null terminator.
 */
int scan_string(char *string, int string_len, int *start_index, int *last_index, const char delimiter) {
    if (*last_index == string_len)
        return -1;
    for (*last_index = *start_index; *last_index <= string_len; *last_index+=1) {
        if ((string[*last_index] == delimiter) || (*last_index == string_len)) {
            return *last_index - *start_index;
        }
    }
    return -2;
}


/**
   @brief Split string from the standsrd input.
   @param String read from the standard input.
   @return An array of tokens with an implicit null terminator.
 */
char **wish_tokenize_line(char *line)
{
    int string_len = strlen(line);
    int token_len = 0, start_index = 0, end_index = 0;
    char *token;
    char **tokens = malloc(16 * sizeof(char));
    int position = 0;
    int bufsize = 16;
    while (scan_string(line, string_len, &start_index, &end_index, ' ') >= 0 ) {
        int ss = 0, ee = 0;
        while (scan_string(line + start_index, end_index-start_index, &ss, &ee, '\t') >= 0 ) {
            token = (char *) malloc((ee-ss + 1) * sizeof(char));
            char *dst = stpncpy(token, line + start_index + ss, ee-ss);
            token[ee-ss]='\0';
            tokens[position] = token;
            position++;
            ss = ee + 1;
            // Reallocate memory size for the tokens pointer
            if (position == bufsize){
                bufsize = bufsize + 64;
                tokens =realloc(tokens, bufsize);
            }
            // Check for correct memory allocation
            if (!tokens){
                printf("\033[1;31m");
                fprintf(stderr, "memory reallocation error!\n");
                printf("\033[0m");
                exit(EXIT_FAILURE);
            }
        }
        start_index = end_index + 1;
    }
    return tokens;
}

// BUILT-IN COMMANDS

/*
  Builtin commands function declarations:
 */
int wish_cd(char **args);
int wish_help(char **args);
int wish_exit(char **args);
int wish_ls(char **args);
int wish_path(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit",
  "ls",
  "path"
};

int (*builtin_func[]) (char **) = {
  &wish_cd,
  &wish_help,
  &wish_exit,
  &wish_ls,
  &wish_path
};

int wish_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}


/**
   @brief Execute builtin exit to exit the Wish shell.
 */
 int wish_exit(char **args)
 {
     exit(0);
 }


 /**
  * @brief Execute builtin ls to  display files and folders in the current directory.
  * @return The exit status of the program, 1. 
  */
int wish_ls(char **args)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d){
        while ((dir = readdir(d)) != NULL) {
            struct stat path_stat;
            char *path = dir->d_name;
            stat(path, &path_stat);
            if(S_ISDIR(path_stat.st_mode)){
                printf("\033[1;34m");
                printf("%s\t", dir->d_name);
                printf("\033[0m");
            }
            if(S_ISREG(path_stat.st_mode)) {
                printf("\033[0m");
                printf("%s\t", dir->d_name);
            }
        }
        printf("\033[0m");
        printf("\n");
        closedir(d);
        return 1;
    }
}


/**
 * @brief Execute builtin path inorder to add path arguments to the search path
 */
int state = 1;
int wish_path(char **args)
{
    if (args[1] != NULL){
        setenv("$PATH", args[1], 0);
        return 1;
    } else {
        state = 0;
        printf("\033[1;31m");
        fprintf(stderr, "wish: path is now empty\n");
        printf("\033[0m");
    }
}


/**
 * @brief Executes the help command which gives guidance to the user
 */
int wish_help(char **args)
{
    printf("\033[1;33m");
    printf("......................................................");
    printf("\nWelcome to Njoroge Waweru's WISH SHELL\n");
    printf("The following commands are built in:\n");
    printf("\t-cd\n \t-ls\n  \t-help\n \t-exit\n\n");
    printf("Type program names and arguments, and hit enter.\n\n");
    printf("Use the man command for information on other programs.\n");
    printf("......................................................\n\n");
    printf("\033[0m");
    return 1;
}


/**
 * @brief Executes the cd command which switches the current directory
 */  
int wish_cd(char **args)
{
    if (args[1] == NULL) {
        printf("\033[1;31m");
        fprintf(stderr, "wish: expected argument to \"cd\"\n");
        printf("\033[0m");
    } else {
        if (chdir(args[1]) != 0) {
            printf("\033[1;31m");
            perror("wish");
            printf("\033[0m");
        }
    }
    return 1;
}


/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int wish_launch(char **args)
{
  pid_t pid, wait_pid;
  int status;
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      printf("\033[1;31m");
      perror("wish");
      printf("\033[0m");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    printf("\033[1;31m");
    perror("wish");
    printf("\033[0m");
  } else {
    // Parent process
    do {
      wait_pid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 after every command is executed.
 */
int wish_execute_arguments(char **args)
{
  int i;
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
  for (i = 0; i < wish_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  if(state==1){
    return wish_launch(args);
  }
  printf("\033[1;31m");
  printf("wish: Check your path or command spelling!\n");
  printf("\033[0m");
  return 1;
  }


/**
   @brief Loop getting input and executing it.
 */
void wish_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("wish> ");
    line = wish_read_line();
    args = wish_tokenize_line(line);
    status = wish_execute_arguments(args);

    free(line);
    free(args);
  } while (status);
}


/**
 * @brief loop reading line, executing it and writing output on a file
 */
int wish_loop_batch(char *filename)
{
    FILE* file_pointer = fopen(filename, "r");
    FILE* output = fopen("output.txt", "w");

    if(!file_pointer)
        {
            printf("\033[1;31m");
            perror("cannot open file\n");
            printf("\033[0m");
            return EXIT_FAILURE;
        }

    char *line;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file_pointer)) != -1) 
    {
        char **args;
        int status;
        
        line[strcspn(line, "\n")] = 0;
        args = wish_tokenize_line(line);
        status = wish_execute_arguments(args);
    }
    fclose(file_pointer);
    fclose(output);
    return 0;
}


/**
   @brief The main entry point of the wish shell.
 */
int main(int argc, char **argv)
{
    if (argc == 1){
        wish_loop();
    }else if (argc == 2){
        int c = wish_loop_batch(argv[1]);
    }else{
        printf("\033[1;31m");
        printf("Arguments exceed 2!\n");
        printf("\033[0m");
    }
    return 0;
}