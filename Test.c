#include <stdio.h>


int main(void){

    FILE *fptr;

    fptr = fopen("message.txt", "r");

    if (fptr == NULL){
        printf("file not found");
        return 1;
    }

    char ch;
    int pos = 0;

    while ((ch = getc(fptr)) != EOF) {

        if (((pos++)+1)%4 == 0){

        printf("%c", ch);

        }
    }

    fclose(fptr);
    fptr = NULL;

    return 1;




}