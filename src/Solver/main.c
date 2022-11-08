#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"work_on_file.h"
#include"solver.h"

int main(int argc, char *argv[]){
     if(argc != 2){
	  printf("Argument error.\n");
	  return 1;
     }
     char input[255];
     read_file(argv[1],input);

     int tab[9][9];
     decode(input,tab);
     solve(tab);

     char result[255];
     toString(tab,result);
     write_file(argv[1],result);

     return 0;
}
