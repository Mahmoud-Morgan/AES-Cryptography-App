#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


FILE * getInputFile()
{
    FILE *inputFile;
    char filename[100];
    bool checkInput = false;
    
    do{
        printf("Enter the filename to open for reading \n");
        scanf("%s", filename);
        // Open one file for reading
        inputFile = fopen(filename, "r");
        if (inputFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }else{
            checkInput = true;
        }
    }while (checkInput == false);
    return inputFile;
}


FILE * getOutputFile()
{
    FILE *outputFile;
    char filename[100];
    bool checkInput = false;
    
    do{
        printf("Enter the filename to open for writing \n");
        scanf("%s", filename);
        // Open one file for reading
        outputFile = fopen(filename, "w");
        if (outputFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }else{
            checkInput = true;
        }
    }while (checkInput == false);
    return outputFile;
}

void getOuputFile()
{
    //
}

void encrypt()
{
    //
}

void decrypt()
{
    //
}

char prpuosActionEncryptOrDecrypt()//TODO need to check the duplication
{
    char checkEncryptOrDecrypt;
    bool checkInput = false;
    
    while (checkInput == false){
       printf("pleas select the prpuos action \n ( e ) for Encrypt \n ( d ) for Decrypt \n");
       scanf("%c",&checkEncryptOrDecrypt);  
        if(checkEncryptOrDecrypt == 'e' || checkEncryptOrDecrypt == 'd'){
             checkInput = true;
        }
    } 
    return checkEncryptOrDecrypt;
}

int main()
{

    char checkEncryptOrDecrypt = prpuosActionEncryptOrDecrypt();
    
    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        /* code */
        break;
     case 'd':
        /* code */
        break;
    }


    FILE *inputFile = getInputFile();
    FILE *outputFile = getOutputFile();

   int encryptionKey;
   printf("please enter encryption Key \n");
    scanf( "%d" ,&encryptionKey);

    // Read contents from file
    char c;
    c = fgetc(inputFile);
    char encryptedChart;
    while (c != EOF)
    {
        encryptedChart = c + encryptionKey; //encryption 

        fputc(encryptedChart, outputFile);
        c = fgetc(inputFile);
    }
  
    printf("\n Contents copied \n");
  
    fclose(inputFile);
    fclose(outputFile);

   return 0;
}