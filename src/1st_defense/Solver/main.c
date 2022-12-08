#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"work_on_file.h"
#include"solver.h"

int main(int argc, char *argv[]){
    if(argc ==1 || argc>3){
        printf("Argument error.\n");
        return 1;
    }
    char input[1024];
    read_file(argv[1],input);

    int width = 9;
    if (argc == 3)
        width = 16;

    int* tab[width];
    for(int i =0; i<width; i++)
        tab[i] = malloc(width*sizeof(int));
    decode(input,tab,width);
    solve(tab, width);

    char result[1024];
    toString(tab,result,width);
    write_file(argv[1],result);
    for(int i =0; i<width; i++)
        free(tab[i]);
    return 0;
}
