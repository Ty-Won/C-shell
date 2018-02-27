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


int shell_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid=fork();
        //if child
        if(pid==0){
            if(execvp(args[0],args)== -1){
                perror("C-Shell");
            }
            exit(EXIT_FAILURE);
        }
        else if(pid<0){
            perror ("C-Shell");
        }
        else{//Parent Process
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status)&& !WIFSIGNALED(status));

        }
        return 1;
}

int shell_cd(char ** args);
int shell_help(char **args);
int shell_exit(char **args);

char * builtInStr[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit
};

int shell_num_builtins(){
    return sizeof(builtInStr)/sizeof(char *);
}

int shell_cd ( char **args){
    if(args[1]==NULL) {
        printf("C-Shell expected argument to \"CD\"\n");
    }else{
        if(chdir(args[1])!=0){
            perror("C-shell:Not child process");
        }
    }
    return 1;
}

// int shell_help(char **args){
//     int 
// }

int shell_exit(char **args){
    return 0;
}

int shell_execute (char **args){
    int execIndex;
    if(args[0]==NULL){
        return 1;
    }

    for (execIndex=0; execIndex<shell_num_builtins();execIndex++){
        printf("    %s\n", builtInStr[execIndex]);
    }
}