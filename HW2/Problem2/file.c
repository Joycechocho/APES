#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void main(){
    //Print to Standard out an interesting string using printf
    printf("An Interesting String\n");

    //Create a file
    FILE *fp;

    //Open the file (for writing)
    fp = fopen("problem2.txt","w");

    //Modify the permission of the file to be read/write
    char *path = realpath("problem2.txt", NULL);
    chmod(path, S_IRWXU);    

    //Write a character to the file
    putc('A', fp);
    putc('\n', fp);
    
    //Close the file
    fclose(fp);

    //Open the file (in append mode)
    fp = fopen("problem2.txt", "a");

    //Dynamically allocate an array of memory
    int *p = malloc(3*sizeof(int));
    for(int i=0; i<3; i++){
        p[i]=4*i;
    }
    //Free the memory
    free(p);
    
    //Read and input string from the command line and write to the string
    char input;
    printf("Specify the input:\n");
    
    //Write the string to the file
    while((input=getchar())!='\n'){
        putc(input, fp);
    }

    //Flush file output
    fflush(stdin);

    //Close the file
    fclose(fp);

    //Open the file (for reading)
    fp = fopen("problem2.txt", "r");

    //Read a single character (getc)
    char singleC = getc(fp);
    
    //Read a string of characters (gets)
    char stringC[20];
    fgets(stringC, 20, fp);

    //Close the file
    fclose(fp);
}
