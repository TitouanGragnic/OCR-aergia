#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>
void read_file(char file[], char input[]){
    /*
      Read file and put the content on input array.
    */

    FILE *fptr;
    int bufferLength = 1024;
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
    char* file = malloc((strlen(name)+strlen(".result")+1)*sizeof(char));
    strcpy(file,name);
    strcat(file,".result");

    FILE *fptr;
    fptr = fopen(file,"w");

    fprintf(fptr,"%s",result);
    free(file);
}

void decode(char input[], int** tab,int width)
{
    /*
      Decode string format to int matrix.
    */
    int i = 0;
    int j = 0;
    while(i<width*width)
    {
        if(input[j] == '.')
        {
            tab[i/width][i%width] = 0;
            i++;
        }
        else if (input[j]=='0')
        {
            tab[i/width][i%width] = 16;
            i++;
        }
        else if (input[j]>'0' && input[j]<= '9')
        {
            tab[i/width][i%width] = input[j]-'0';
            i++;
        }
        else if (input[j]>='A' && input[j]<='F')
        {
            tab[i/width][i%width] = input[j]-'A'+10;
            i++;
        }
        j++;
    }
}

void toString(int** tab,char result[], int width){
    /*
      Convert solver result to string format.
    */
    int i = 0;
    int j = 0;
    int k = 0;

    int square = sqrt(width);
    while(i<width)
    {
        j=0;
        while(j<width)
        {
            if(tab[i][j] == 16)
                result[k] = '0';
            if(tab[i][j]<=9)
                result[k] = '0'+tab[i][j];
            else
                result[k] = 'A'+tab[i][j]-10;
            k++;
            if((j+1)%square == 0)
            {
                result[k] = ' ';
                k++;
            }
            j++;
        }
        if((i+1)%square == 0)
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
