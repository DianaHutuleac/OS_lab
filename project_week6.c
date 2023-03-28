#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){


    if(argc < 2){
        printf("not enough arguments! provide multiple arguments representing different paths\n");
        exit(EXIT_FAILURE);
    }
    else{
        for(int i = 1; i < argc; i++){

            struct stat info;
            lstat(argv[i], &info);

            mode_t m;

            m = info.st_mode;

            if(S_ISREG(m)){
                printf("argument %d is reg\n", i);

                printf("\n");
                printf("Menu for a regular file:\n");
                printf("-n (file name)\n");
                printf("-d (file dim/size)\n");
                printf("-h (number of hard links)\n");
                printf("-m (time of last modification)\n");
                printf("-a (access rights)\n");
                printf("-l (create a sym link give: link name)\n");
            }
            else if(S_ISDIR(m)){
                printf("argument %d is dir\n", i);
            }
            else if(S_ISLNK(m)){
                printf("argument %d is link\n", i);
            }
        }
    }
}