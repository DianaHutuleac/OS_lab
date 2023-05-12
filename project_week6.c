#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>
#include <errno.h>

void menuRegularFile(){
    printf("\nMenu for a regular file:\n\n");

    char *row1[] = {"Option", "Description"};
    char *col1[] = {"-n", "-d", "-h", "-m", "-a", "-l"};
    char *col2[] = {"File Name", "File Dimension/Size", "Number of Hard Links", "Time of Last Modification", "Access Rights", "Create a Symbolic Link & Give the Link Name"};

    printf("%10s\t|\t%10s\t \n", row1[0], row1[1]);
    printf("-------------------------------------------------------------------\n");
    size_t len = sizeof col1 / sizeof col1[0];
    for (int j = 0; j < len; ++j) {
        printf("%8s\t|\t%9s\t\n", col1[j], col2[j]);
    }
}

void menuDirectory(){
    printf("\nMenu for a Directory:\n\n");

    char *row1[] = {"Option", "Description"};
    char *col1[] = {"-n", "-d", "-a", "-c"};
    char *col2[] = {"Directory Name", "Directory Dimension/Size", "Access Rights", "Total number of .c files in the directory"};

    printf("%10s\t|\t%10s\t \n", row1[0], row1[1]);
    printf("-------------------------------------------------------------------\n");
    size_t len = sizeof col1 / sizeof col1[0];
    for (int k = 0; k < len; ++k) {
        printf("%8s\t|\t%9s\t\n", col1[k], col2[k]);
    }
}

void menuSymbolicLink(){
    printf("\nMenu for a symbolic link:\n\n");
                        
    char *row1[] = {"Option", "Description"};
    char *col1[] = {"-n", "-l", "-d", "-t", "-a"};
    char *col2[] = {"Link Name", "Delete link", "Size of the link", "Size of the target", "Access Rights"};

    printf("%10s\t|\t%10s\t \n", row1[0], row1[1]);
    printf("-------------------------------------------------------------------\n");
    size_t len = sizeof col1 / sizeof col1[0];
    for (int l = 0; l < len; ++l) {
        printf("%8s\t|\t%9s\t\n", col1[l], col2[l]);
    }
}

char* optRegFile(){
    char *pattern = "^-[ndhmal]+$";
    printf("\nChoose one or more options using the syntax: \"-\" followed by one or more letters from the options menu.\n");
    char* optionForReg = (char*) malloc(32 * sizeof(char));

    // Use a do-while loop to ensure valid input
    int validInput;
    do {
        // Prompt for input and read it with scanf
        printf("\nOption(s): ");
        scanf("%s", optionForReg);

        // Compile the regular expression pattern and check the input against it
        regex_t regex;
        int ret = regcomp(&regex, pattern, REG_EXTENDED);
        if (ret != 0) {
            printf("\nError compiling regular expression.\n");
            exit(1);
        }
        ret = regexec(&regex, optionForReg, 0, NULL, 0);
        regfree(&regex);

        // If the input is valid, exit the loop
        if (ret == 0) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter \"-\" & one or more letters from the options menu.\n");
            menuRegularFile();
            validInput = 0;
        }
    } while (!validInput);
    return optionForReg;
}

char* optDir(){
    char *pattern = "^-[ndac]+$";
    printf("\nChoose one or more options using the syntax: \"-\" followed by one or more letters from the options menu.\n");
    char* optionForDir = (char*) malloc(32 * sizeof(char));

    // Use a do-while loop to ensure valid input
    int validInput;
    do {
        // Prompt for input and read it with scanf
        printf("\nOption(s): ");
        scanf("%s", optionForDir);

        // Compile the regular expression pattern and check the input against it
        regex_t regex;
        int ret = regcomp(&regex, pattern, REG_EXTENDED);
        if (ret != 0) {
            printf("\nError compiling regular expression\n");
            exit(1);
        }
        ret = regexec(&regex, optionForDir, 0, NULL, 0);
        regfree(&regex);

        // If the input is valid, exit the loop
        if (ret == 0) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter \"-\" & one or more letters from the options menu.\n");
            menuDirectory();
            validInput = 0;
        }
    } while (!validInput);
    return optionForDir;
}

char* optLink(){
    char *pattern = "^-[nldta]+$";
    printf("\nChoose one or more options using the syntax: \"-\" followed by one ore more letters from the options menu.\n"
            "(Note that if the -l or l option is given, the other following options won't be performed!!)\n");
    char* optionForLink = (char*) malloc(32 * sizeof(char));

    // Use a do-while loop to ensure valid input
    int validInput;
    do {
        // Prompt for input and read it with scanf
        printf("\nOption(s): ");
        scanf("%s", optionForLink);

        // Compile the regular expression pattern and check the input against it
        regex_t regex;
        int ret = regcomp(&regex, pattern, REG_EXTENDED);
        if (ret != 0) {
            printf("\nError compiling regular expression\n");
            exit(1);
        }
        ret = regexec(&regex, optionForLink, 0, NULL, 0);
        regfree(&regex);

        // If the input is valid, exit the loop
        if (ret == 0) {
            validInput = 1;
        } else {
            printf("Invalid input. Please enter \"-\" & one or more letters from the options menu.\n");
            menuSymbolicLink();
            validInput = 0;
        }
    } while (!validInput);
    return optionForLink;
}

// Function for printing the access rights
void printAccessRights(struct stat info){

    printf("\nAccess rights: \n");
    printf((S_ISDIR(info.st_mode)) ? "d" : ((S_ISLNK(info.st_mode)) ? "l" : "-"));
    printf((info.st_mode & S_IRUSR) ? "r" : "-");
    printf((info.st_mode & S_IWUSR) ? "w" : "-");
    printf((info.st_mode & S_IXUSR) ? "x" : "-");
    printf((info.st_mode & S_IRGRP) ? "r" : "-");
    printf((info.st_mode & S_IWGRP) ? "w" : "-");
    printf((info.st_mode & S_IXGRP) ? "x" : "-");
    printf((info.st_mode & S_IROTH) ? "r" : "-");
    printf((info.st_mode & S_IWOTH) ? "w" : "-");
    printf((info.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("\nUSER:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IRUSR) ? "YES" : "NO",
       (info.st_mode & S_IWUSR) ? "YES" : "NO",
       (info.st_mode & S_IXUSR) ? "YES" : "NO");

    printf("\nGROUP:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IRGRP) ? "YES" : "NO",
       (info.st_mode & S_IWGRP) ? "YES" : "NO",
       (info.st_mode & S_IXGRP) ? "YES" : "NO");

    printf("\nOTHERS:\n-read: %s\n-write: %s\n-execute: %s\n", 
       (info.st_mode & S_IROTH) ? "YES" : "NO",
       (info.st_mode & S_IWOTH) ? "YES" : "NO",
       (info.st_mode & S_IXOTH) ? "YES" : "NO");
}

void handleRegularFile(char *path, char *optionForReg){
    struct stat info;
    if(lstat(path, &info) == -1) {
        perror("\nError: ");
        exit(1);
    }

    // File Name
    if(strstr(optionForReg, "n")){
        printf("\nThe file name is: %s\n", path);
    }

    // File Dimension/Size
    if(strstr(optionForReg, "d")){
        printf("\nThe file dimension/size is: %ld bytes\n", info.st_size);
    }

    // Number of Hard Links
    if(strstr(optionForReg, "h")){
        printf("\nThe number of hard links is: %ld \n", info.st_nlink);
    }

    // Time of Last Modification
    if(strstr(optionForReg, "m")){
        printf("\nTime of last modification is: %s", ctime(&info.st_mtime));
    }

    // Access Rights            
    if(strstr(optionForReg, "a")){
        printAccessRights(info);   
    }

    // Create a Symbolic Link & Give the Link Name
    if(strstr(optionForReg, "l")){
        char linkName[52];
        printf("\nIntroduce the link name: ");
        scanf("%s", linkName);

        if (symlink(path, linkName) == -1) {
            perror("\nError creating symbolic link!\n");
        } else {
            printf("\nSymbolic link created: %s -> %s\n", linkName, path);
        }
    }
}

void handleDirectory(char* path, char* optionForDir){
    struct stat info;
    if(lstat(path, &info) == -1) {
        perror("\nError: ");
        exit(1);
    }

    DIR *dir;
    struct dirent *entry;              
    dir = opendir(path);
    if (dir == NULL) {
        perror("\nError opening directory: ");
        exit(1);
    }

    // Directory Name
    if(strstr(optionForDir, "n")){
        while((entry = readdir(dir)) != NULL){
            printf("\nThe directory name is: %s", entry->d_name);

        }
        printf("\n");               
    }
    closedir(dir);

    // Directory Dimension/Size
    if(strstr(optionForDir, "d")){
        printf("\nThe directory dimension/size is: %ld bytes\n", info.st_size);
    }

    // Access Rights
    if(strstr(optionForDir, "a")){
        printAccessRights(info);   
    }

    // Total number of .c files in the directory
    if(strstr(optionForDir, "c")){              
        dir = opendir(path);
        if (dir == NULL) {
            perror("\nError opening directory: ");
            exit(1);
        }

        int noOfCFiles = 0;
        while((entry = readdir(dir)) != NULL){

            char names[1024];
            snprintf(names, sizeof(names), "%s/%s", path, entry->d_name);
            if (lstat(names, &info) == -1) {
                perror("\nError getting file st: ");
                exit(1);
            }

            if(S_ISREG(info.st_mode) && strstr(entry->d_name, ".c")){
                noOfCFiles++;
            }
        }
                        
        printf("\nTotal number of .c files in the directory is: %d\n", noOfCFiles);
        closedir(dir);
    }
}
             

void handleLinks(char* path, char* optionForLink){
    struct stat info;
    if(lstat(path, &info) == -1) {
        perror("\nError: ");
        exit(1);
    }

    // Link Name
    if(strstr(optionForLink, "n")){
        printf("\nThe link name is: %s\n", path);
    }

    // Delete link
    if(strstr(optionForLink, "l")){
        if(unlink(path) != 0){
            perror("unlink() error\n");
        }else{
            printf("\nThe link %s was deleted.\n", path);
            exit(1);
        }                    
    }

    // Size of the link
    if(strstr(optionForLink, "d")){
        printf("\nThe size of the link is: %ld bytes\n", info.st_size);
    }

    // Size of the target
    if(strstr(optionForLink, "t")){
        char target_path[256];
        ssize_t len = readlink(path, target_path, sizeof(target_path)-1);

        struct stat targetstat;

        if (len != -1) {
            target_path[len] = '\0';
            if (lstat(target_path, &targetstat) == 0) {
                if (S_ISREG(targetstat.st_mode)) {
                    printf("\nThe size of the target file is: %lld bytes\n", (long long) targetstat.st_size);
                } else {
                    printf("\nTarget is not a regular file.\n");
                }
            } else {
                perror("lstat");
                return;
            }
        }
        else {
            perror("readlink");
            return;
        }
    }

    // Access Rights            
    if(strstr(optionForLink, "a")){
        printAccessRights(info);
    }
}


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Not enough arguments! Provide multiple arguments representing different paths.\n");
        exit(EXIT_FAILURE);
    }
    else{
        int i, status;
        pid_t pid;

        for(i = 1; i < argc; i++){
            char option[32] = "";
            char* reg;
            char* dir;
            char* link;
            struct stat info;
            if(lstat(argv[i], &info) == -1) {
                perror("\nError: ");
                exit(1);
            }
            if(S_ISREG(info.st_mode)){
                printf("\nArgument %d <%s> is a Regular File\n", i, argv[i]);
                menuRegularFile();
                reg = optRegFile();
                strcpy(option, reg);
                        
            } else if(S_ISDIR(info.st_mode)){
                printf("\nArgument %d <%s> is a Directory\n", i, argv[i]);
                menuDirectory();
                dir = optDir();
                strcpy(option, dir);

            } else if(S_ISLNK(info.st_mode)){
                printf("\nArgument %d <%s> is a Link\n", i, argv[i]);
                menuSymbolicLink();
                link = optLink();
                strcpy(option, link);
            }

            if((pid = fork()) < 0){
                perror("Error\n");
                exit(EXIT_FAILURE);
            }

            else if(pid == 0){
                // Create first child process
                pid_t getID = getpid();
                printf("\nPID for Handling Menu: %d\n", getID);

                if(S_ISREG(info.st_mode)){
                    handleRegularFile(argv[i], option);
                        
                } else if(S_ISDIR(info.st_mode)){
                    handleDirectory(argv[i], option);

                } else if(S_ISLNK(info.st_mode)){
                    handleLinks(argv[i], option);

                }
                exit(EXIT_SUCCESS);
            }

            // Parent process here
            char *ext = strrchr(argv[i], '.');
            int pfd[2];

            if (pipe(pfd) < 0) {
                perror("\nPipe creation error\n");
                exit(EXIT_FAILURE);
            }

            if((pid = fork()) < 0){
                perror("Error\n");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                // Create second child process

                if(S_ISREG(info.st_mode)){
                    // Close read end of the pipe
                    close(pfd[0]);
                    if (ext != NULL && strcmp(ext, ".c") == 0) {
                        // Redirect stdout to the write end of the pipe
                        dup2(pfd[1], 1);

                        // Execute the script
                        if (execlp("./compileScript.sh", "./compileScript.sh", argv[i], NULL) == -1) {
                            perror(strerror(errno));
                            exit(errno);
                        }
                    } else{
                        int noOfLines = 0;
                        char c;
                        FILE *fp = fopen(argv[i], "r");
                        if (fp == NULL) {
                            perror("\nError opening file.\n");
                            exit(1);
                        }

                        while ((c = getc(fp)) != EOF) {
                            if (c == '\n') {
                                noOfLines++;
                            }
                        }

                        printf("\nNumber of lines in the file: %d\n", noOfLines);
                        fclose(fp);                        
                    }
                    exit(EXIT_SUCCESS);
                } else if(S_ISDIR(info.st_mode)){

                    char fileName[100];
                    snprintf(fileName, sizeof(fileName), "%s/%s", argv[i], "file.txt");
                
                    if(execlp("touch", "touch", fileName, NULL) == -1){
                        perror(strerror(errno));
                        exit(errno);
                    }

                    exit(EXIT_SUCCESS);
                }
                if(S_ISLNK(info.st_mode)){

                    if(execlp("chmod", "chmod", "u=rwx,g=rw,o=", argv[i], NULL) == -1){
                        perror(strerror(errno));
                        exit(errno);
                    }
                    exit(EXIT_SUCCESS);  
                }
            }
            // Computing the score in the parent process
            if(S_ISREG(info.st_mode)){
                if (ext != NULL && strcmp(ext, ".c") == 0) {
                    close(pfd[1]);

                    char string[255];
                    int noOfErrors = 0, noOfWarnings = 0;
                    FILE *stream;
                    stream = fdopen(pfd[0], "r");

                    while(fscanf(stream, "%s", string) != EOF) {
                        if(strstr(string, "error")) {
                            noOfErrors++;

                        } else if(strstr(string, "warning")){
                            noOfWarnings++;
                        }
                    }

                    printf("\nNumber of Errors: %d\nNumber of Warnings: %d\n", noOfErrors, noOfWarnings);
                    double score;
                    if(noOfErrors > 0) {
                        score = 1;
                    } else{
                        if(noOfWarnings == 0) {
                            score = 10;
                        } else if(noOfWarnings > 10) {
                            score = 2;
                        } else{
                            score = 2 + 8 * (10 - noOfWarnings) / 10.0;
                        }
                    }

                    char scoreString[10];
                    sprintf(scoreString, "%.2f", score);

                    char fileName[52];
                    sprintf(fileName, "%s:%s", argv[i], scoreString);

                    FILE *fp = fopen("grades.txt", "a");
                    if (fp == NULL) {
                        perror("\nError opening file grades.txt\n");
                        exit(1);
                    }
                    fprintf(fp, "%s\n", fileName);
                    fclose(fp);

                    printf("\nScore computed and saved in grades.txt.\n");
                    close(pfd[0]);
                }
            }

            for(int j = 0; j < 2; j++){
                int w = wait(&status);

                if(w == -1){
                    perror("\nWaitpid\n");
                    exit(EXIT_FAILURE);
                }
                if(WIFEXITED(status)){
                    printf("\nThe process with PID %d has ended with the exit code %d.\n ", w, WEXITSTATUS(status));
                } else{
                    perror("\nError at exiting.\n");
                    exit(-1);
                }
            }
        }       
    } 
}
