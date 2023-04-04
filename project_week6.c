#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

                printf("\n");
                printf("Choose one or more options\n");

                char option[52];

                scanf("%s", option);

                printf("%s\n", option);

                if(strstr(option, "n") || strstr(option, "-n")){
                    printf("\nThe file name is %s\n", argv[i]);
                }

                if(strstr(option, "d") || strstr(option, "-d")){
                    off_t size = info.st_size;
                    printf("\nThe file dimension/size is %ld bytes\n", size);
                }

                if(strstr(option, "h") || strstr(option, "-h")){
                    nlink_t noOfHardLinks = info.st_nlink;
                    printf("\nThe number of hard links is %ld \n", noOfHardLinks);
                }

                if(strstr(option, "m") || strstr(option, "-m")){
                    printf("\nTime of last modification is: %s", ctime(&info.st_mtime));
                }
                
                if(strstr(option, "a") || strstr(option, "-a")){
                    printf("\nAccess rights: \n");
                    printf((S_ISDIR(info.st_mode)) ? "d" : "-");
                    printf((info.st_mode & S_IRUSR) ? "r" : "-");
                    printf((info.st_mode & S_IWUSR) ? "w" : "-");
                    printf((info.st_mode & S_IXUSR) ? "x" : "-");
                    printf((info.st_mode & S_IRGRP) ? "r" : "-");
                    printf((info.st_mode & S_IWGRP) ? "w" : "-");
                    printf((info.st_mode & S_IXGRP) ? "x" : "-");
                    printf((info.st_mode & S_IROTH) ? "r" : "-");
                    printf((info.st_mode & S_IWOTH) ? "w" : "-");
                    printf((info.st_mode & S_IXOTH) ? "x" : "-");
                    printf("\n\n");
                }

                if(strstr(option, "l") || strstr(option, "-l")){
                    char linkName[52];
                    printf("\nIntroduce the link name: ");
                    scanf("%s", linkName);
                    symlink(argv[i], linkName);
                }
                
            }
            else if(S_ISDIR(m)){
                printf("argument %d is dir\n", i);
            }
            else if(S_ISLNK(m)){
                printf("argument %d is link\n", i);

                printf("\n");
                printf("Menu for a symbolic link:\n");
                printf("-n (link name)\n");
                printf("-l (delete link)\n");
                printf("-d (size of the link)\n");
                printf("-t (size of the target)\n");
                printf("-a (access rights)\n");

                printf("\n");
                printf("Choose one or more options (Note that if the -l or l option is given, the other following options won't be performed!!)\n");

                char option[52];

                scanf("%s", option);

                printf("%s\n", option);

                if(strstr(option, "n") || strstr(option, "-n")){
                    printf("\nThe link name is %s\n", argv[i]);
                }

                if(strstr(option, "l") || strstr(option, "-l")){
                    if(unlink(argv[i]) != 0){
                        perror("unlink() error\n");
                    }else{
                        printf("The link %s was deleted\n", argv[i]);
                        //exit(0);
                    }                    
                }

                if(strstr(option, "d") || strstr(option, "-d")){
                    off_t size = info.st_size;
                    printf("\nThe size of the link is %ld bytes\n", size);
                }

                if(strstr(option, "t") || strstr(option, "-t")){
                    char target_path[256];
                    ssize_t len = readlink(argv[i], target_path, sizeof(target_path)-1);

                    if (len != -1) {
                        target_path[len] = '\0';
                        if (lstat(target_path, &info) == 0) {
                            if (S_ISREG(info.st_mode)) {
                                printf("Target file size: %lld bytes\n", (long long) info.st_size);
                            } else {
                                printf("Target is not a regular file.\n");
                            }
                        } else {
                            perror("lstat");
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        perror("readlink");
                        exit(EXIT_FAILURE);
                    }
                }
                
                if(strstr(option, "a") || strstr(option, "-a")){
                    printf("\nAccess rights: \n");
                    printf((S_ISDIR(info.st_mode)) ? "d" : "-");
                    printf((info.st_mode & S_IRUSR) ? "r" : "-");
                    printf((info.st_mode & S_IWUSR) ? "w" : "-");
                    printf((info.st_mode & S_IXUSR) ? "x" : "-");
                    printf((info.st_mode & S_IRGRP) ? "r" : "-");
                    printf((info.st_mode & S_IWGRP) ? "w" : "-");
                    printf((info.st_mode & S_IXGRP) ? "x" : "-");
                    printf((info.st_mode & S_IROTH) ? "r" : "-");
                    printf((info.st_mode & S_IWOTH) ? "w" : "-");
                    printf((info.st_mode & S_IXOTH) ? "x" : "-");
                    printf("\n\n");
                }
            }
        }
    }
}