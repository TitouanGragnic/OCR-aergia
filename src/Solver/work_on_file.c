#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void read_file(char file[], char input[]){
     /*
       Read file and put the content on input array.
     */

     FILE *fptr;
     int bufferLength = 255;
     char buffer[bufferLength];

     if ((fptr = fopen(file,"r")) == NULL)
     {
	  printf("Error! opening file");
	  exit(1);
     }

     int i = 0 ;
     while(fgets(buffer, bufferLength, fptr))
     {
	  int j=0;
	  while (buffer[j] != 0)
	  {
	       input[i] = buffer[j];
	       j++;
	       i++;
	  }
     }
     fclose(fptr);
}


void write_file(char name[],char result[])
{
     /*
       Write a file with content of the string solver result.
     */
     char file[strlen(name)+7];
     strcpy(file,name);
     strcat(file,".result");

     FILE *fptr;
     fptr = fopen(file,"w");

     fprintf(fptr,"%s",result);
}

void decode(char input[], int tab[][9])
{
     /*
       Decode string format to int matrix.
     */
     int i = 0;
     int j = 0;

     while(i<81)
     {
	  if(input[j] == '.')
	  {
	       tab[i/9][i%9] = 0;
	       i++;
	  }
	  else if (input[j]>='0' && input[j]<= '9')
	  {
	       tab[i/9][i%9] = input[j]-'0';
	       i++;
	  }
	  j++;
     }
}

void toString(int tab[9][9],char result[]){
     /*
       Convert solver result to string format.
     */
     int i = 0;
     int j = 0;
     int k = 0;

     while(i<9)
     {
	  j=0;
	  while(j<9)
	  {
	       result[k] = '0'+tab[i][j];
	       k++;
	       if((j+1)%3 == 0)
	       {
		    result[k] = ' ';
		    k++;
	       }
	       j++;
	  }
	  if((i+1)%3 == 0)
	  {
	       result[k] = '\n';
	       k++;
	  }
	  result[k] = '\n';
	  k++;
	  i++;
     }
     result[k] = '\0';
}
