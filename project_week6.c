#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>


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


void processRegularFile(char *path){

    char *pattern = "^-[ndhmal]+$";

    printf("\nChoose one or more options using the syntax: \"-\" followed by one or more letters from the options menu.\n");

    char optionForReg[32];

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
            validInput = 0;
        }
    } while (!validInput);


    struct stat info;

    if(lstat(path, &info) == -1) {
        perror("\nError: ");
        exit(1);
    }

    // File Name
    if(strstr(optionForReg, "n") || strstr(optionForReg, "-n")){
        printf("\nThe file name is: %s\n", path);
    }

    // File Dimension/Size
    if(strstr(optionForReg, "d") || strstr(optionForReg, "-d")){
        off_t size = info.st_size;
        printf("\nThe file dimension/size is: %ld bytes\n", size);
    }

    // Number of Hard Links
    if(strstr(optionForReg, "h") || strstr(optionForReg, "-h")){
        nlink_t noOfHardLinks = info.st_nlink;
        printf("\nThe number of hard links is: %ld \n", noOfHardLinks);
    }

    // Time of Last Modification
    if(strstr(optionForReg, "m") || strstr(optionForReg, "-m")){
        printf("\nTime of last modification is: %s", ctime(&info.st_mtime));
    }

    // Access Rights            
    if(strstr(optionForReg, "a") || strstr(optionForReg, "-a")){
        printAccessRights(info);   
    }

    // Create a Symbolic Link & Give the Link Name
    if(strstr(optionForReg, "l") || strstr(optionForReg, "-l")){
        char linkName[52];
        printf("\nIntroduce the link name: ");
        scanf("%s", linkName);

        if (symlink(path, linkName) == -1) {
            perror("\nError creating symbolic link!\n");
        } else {
            printf("\nSymbolic link created: %s -> %s\n", linkName, path);
        }

    }

    pid_t pid;

    if(strstr(path, ".c")){
        pid = fork();

        if(pid < 0){
            perror("\nFork failed.\n");
            return;
        }
        else if(pid == 0){

            pid_t getID = getpid();
            printf("\nPID for .C Files: %d\n", getID);

            execlp("./compileScript.sh", "./compileScript.sh", path, NULL);
            perror("\nExec failed.\n");
            return;
        }
        else{
            int status;
            
            int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);

            if(w == -1){
                perror("\nWaitpid\n");
                exit(EXIT_FAILURE);
            }
            if(WIFEXITED(status)){
                printf("\nExited, status = %d, PID: %d \n ", WEXITSTATUS(status), w);
            }
            else{
                perror("\nError at exiting.\n");
                exit(-1);
            }
        }
    }
}


void processDirectory(char* path){

    char *pattern = "^-[ndac]+$";

    printf("\nChoose one or more options using the syntax: \"-\" followed by one or more letters from the options menu.\n");

    char optionForDir[32];

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
            validInput = 0;
        }
    } while (!validInput);

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
    if(strstr(optionForDir, "n") || strstr(optionForDir, "-n")){

        while((entry = readdir(dir)) != NULL){
            printf("\nThe directory name is: %s", entry->d_name);

        }
        printf("\n");
                        
    }

    closedir(dir);

    // Directory Dimension/Size
    if(strstr(optionForDir, "d") || strstr(optionForDir, "-d")){
        off_t sizeDir = info.st_size;
        printf("\nThe directory dimension/size is: %ld bytes\n", sizeDir);
    }

    // Access Rights
    if(strstr(optionForDir, "a") || strstr(optionForDir, "-a")){
        printAccessRights(info);   
    }

    // Total number of .c files in the directory
    if(strstr(optionForDir, "c") || strstr(optionForDir, "-c")){
                        
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

    pid_t pid = fork();

    if(pid < 0){
        perror("\nFork failed.\n");
        return;
    }
    else if(pid == 0){

        pid_t getID = getpid();
        printf("\nPID for Directories: %d\n", getID);

        char fileName[100];
        snprintf(fileName, sizeof(fileName), "%s/%s", path, "file.txt");

        FILE *f = fopen(fileName, "w");

        if(f == NULL){
            perror("\nError creating file.\n");
            return;
        }
        fclose(f);

        printf("\n-------------------------------------------------------------------\n");
        printf("\nThe provided argument is a directory. In addition, a corresponding txt file will be created:\n");
        printf("\nFile created: %s\n", fileName);
        printf("\n-------------------------------------------------------------------\n");
        exit(EXIT_SUCCESS);
    }
    else{
        int status;
        int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);

        if(w == -1){
            perror("\nWaitpid\n");
            exit(EXIT_FAILURE);
        }
        if(WIFEXITED(status)){
            printf("\nExited, status = %d, PID: %d \n ", WEXITSTATUS(status), w);
        }
        else{
            perror("\nError at exiting.\n");
            exit(-1);
        }
    }
}
             

void processLinks(char* path){

    char *pattern = "^-[nldta]+$";
    
    printf("\nChoose one or more options using the syntax: \"-\" followed by one ore more letters from the options menu.\n"
            "(Note that if the -l or l option is given, the other following options won't be performed!!)\n");

    char optionForLink[32];

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
            validInput = 0;
        }
    } while (!validInput);

    struct stat info;
    if(lstat(path, &info) == -1) {
        perror("\nError: ");
        exit(1);
    }

    // Link Name
    if(strstr(optionForLink, "n") || strstr(optionForLink, "-n")){
        printf("\nThe link name is: %s\n", path);
    }

    // Delete link
    if(strstr(optionForLink, "l") || strstr(optionForLink, "-l")){
        if(unlink(path) != 0){
            perror("unlink() error\n");
        }else{
            printf("\nThe link %s was deleted.\n", path);
            
        }                    
    }

    // Size of the link
    if(strstr(optionForLink, "d") || strstr(optionForLink, "-d")){
        off_t size = info.st_size;
        printf("\nThe size of the link is: %ld bytes\n", size);
    }

    // Size of the target
    if(strstr(optionForLink, "t") || strstr(optionForLink, "-t")){
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
    if(strstr(optionForLink, "a") || strstr(optionForLink, "-a")){
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

        for(i = 1; i < argc; i++){
            // Create a child process
            pid_t pid = fork();
 
            if(pid < 0){
                perror("Error\n");
                exit(EXIT_FAILURE);
            }

            else if(pid == 0){

                pid_t getID = getpid();
                printf("\nPID for Handling Menu: %d\n", getID);
                                    
                struct stat info;
                lstat(argv[i], &info);
                mode_t m;
                m = info.st_mode;

                if(S_ISREG(m)){

                    printf("\nArgument %d is a Regular File\n", i);

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

                    processRegularFile(argv[i]);
                        
                } 
                if(S_ISDIR(m)){

                    printf("\nArgument %d is a Directory\n", i);

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

                    processDirectory(argv[i]);

                }
                if(S_ISLNK(m)){

                    printf("\nArgument %d is a link\n", i);

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

                    processLinks(argv[i]);

                }

                exit(EXIT_SUCCESS);
            }
            else{

                int w = waitpid(pid, &status, WUNTRACED | WCONTINUED);

                if(w == -1){
                    perror("\nWaitpid\n");
                    exit(EXIT_FAILURE);
                }
                if(WIFEXITED(status)){
                    printf("\nExited, status = %d, PID: %d \n ", WEXITSTATUS(status), w);
                }
                else{
                    perror("\nError at exiting.\n");
                    exit(-1);
                }
            }     
        } 
    }
}