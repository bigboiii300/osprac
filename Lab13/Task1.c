#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <limits.h>

int createFile(char* name){
    FILE* fd = fopen(name, "a");

    if(fd == NULL){
        printf("Can't open file!\n");
       return 1;
    }
    
    fputs("abcdefghijklmnopqrstuvwxyz", fd);

    fclose(fd);

    return 0;
}

char* getFileName(int i, int withTemp){
    char* filename = malloc(sizeof("aaa"));

    if(withTemp == 0){
        sprintf(filename, "dir_name/%03d", i);
    }
    else{
        sprintf(filename, "%03d", i);
    }

    return filename;
}


void mFree(char* f1, char* f2){
    free(f1);
    free(f2);
}

int main(int argc, char* argv[]){
    char* directory_name = "dir_name";

    printf("Directory: %s\n", directory_name);
    
    DIR *dir = opendir(directory_name);

    if(dir != NULL){
       system("rm -r dir_name");
    }

    system("mkdir dir_name");

    char* firstName = getFileName(0, 0);
    if(createFile(firstName)){
        free(firstName);
        printf("Smth wrong with file!\n");
        exit(-1);
    }
    free(firstName);


    int counter = 1;
    while(1){
        char* prevFile = getFileName(counter - 1, 1);
        char* nextFile = getFileName(counter, 0);

        if(symlink(prevFile, nextFile)) {
            mFree(prevFile, nextFile);
            printf("Smth happened!\n");
            exit(-1);
        }
        FILE* newFile = fopen(nextFile, "b");
        if(newFile == NULL){
            printf("%d\n", counter - 1);
            mFree(prevFile, nextFile);
            exit(-1);
         }
        fclose(newFile);
        mFree(prevFile, nextFile);
        counter++;
    }
  
    if (closedir(dir) != 0) {
        printf("Smth happened!\n");
        return 1;
    }

    return 0;
}
