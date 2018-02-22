#include <stdlib.h>
#include <stdio.h>
#define LINE_BUFFSIZE 1024
#define EOF -1
#define TOKEN_BUFFSIZE 64
#define DELIMITER " \t\r\n\a"

int main(int argc, char **argv){
    shell_loop();
    return EXIT_SUCCESS;
}

/*
Shell Loop is the function that lets the shell run continuously while the user write their command
*/
void shell_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        line = read_line();
        args = arg_split(line);
        status = execute(args);

        free(line);
        free(args);
    } while(status);
}


/*
The read_line function takes the input from the user and allocates memory to contain the entire command
*/
char * read_line(void){
    int bufsize = LINE_BUFFSIZE;
    int pos = 0;
    char * buffer = malloc(sizeof(char)*bufsize);
    int symbol;//EOF is an integer

    if(!buffer) {
        printf("c-shell: Allocation memory error\n");
        exit(EXIT_FAILURE);
    }

    while(1){ //for continuous reading
        symbol = getchar();
        if (symbol== EOF || symbol=='\n'){
            buffer[pos]="\0";
            return buffer;
        }
        else{
            buffer[pos]=symbol;
        }
        symbol++;

        if (pos >= bufsize){
            bufsize+=LINE_BUFFSIZE;
            buffer = realloc(buffer,bufsize);

            if(!buffer){
                printf("c-shell allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
}

/*
The arg_split function takes the line from the read_line function and tokenizes the line into different
segments for later execution
*/
char ** arg_split(char * line){
    int bufferSize = TOKEN_BUFFSIZE;
    int position=0;
    int c;
    char * token;
    char ** tokens = malloc(bufferSize * sizeof(char*));

    token = strtok(line, DELIMITER);

    if(!tokens){
        printf("c-shell allocation error \n");
        exit(EXIT_FAILURE);
    }

    while(token != NULL){
        tokens[position]=token;
        position++;

        if(position >= bufferSize){
            bufferSize+=TOKEN_BUFFSIZE;
            tokens = realloc(tokens,bufferSize * sizeof(char*));
            if (!tokens){
                printf("C-Shell: Allocation error \n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL,DELIMITER); //get the pointer from the previous string execution where it left off
        
    }
    tokens[position] = NULL;
    return tokens;
}


int arg_launch(){
    
}

