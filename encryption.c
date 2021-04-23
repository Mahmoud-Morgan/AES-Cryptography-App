#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

FILE *getInputFile()
{
    FILE *inputFile;
    char filename[100];
    bool checkInput = false;
    do
    {
        printf("Enter the filename to open for reading \n");
        scanf("%s", filename);
        // Open one file for reading
        inputFile = fopen(filename, "r");
        if (inputFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }
        else
        {
            checkInput = true;
        }
    } while (checkInput == false);
    return inputFile;
}

FILE *getOutputFile()
{
    FILE *outputFile;
    char filename[100];
    bool checkInput = false;

    do
    {
        printf("Enter the filename to open for writing \n");
        scanf("%s", filename);
        // Open one file for reading
        outputFile = fopen(filename, "w");
        if (outputFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }
        else
        {
            checkInput = true;
        }
    } while (checkInput == false);
    return outputFile;
}

int getEncryptionKey()
{
    int encryptionKey;
    printf("please enter the integer encryption Key \n");
    scanf("%d", &encryptionKey);
    return encryptionKey;
}

void encrypt(FILE *inputFile, FILE *outputFile, int encryptionKey)
{
    // Read contents from file
    char content;
    c = fgetc(inputFile);
    char encryptedChart;
    while (content != EOF)
    {
        encryptedChart = content + encryptionKey; //encryption
        fputc(encryptedChart, outputFile);
        c = fgetc(inputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

void decrypt(FILE *inputFile, FILE *outputFile, int encryptionKey)
{
    // Read contents from file
    char content;
    c = fgetc(inputFile);
    char encryptedChart;
    while (content != EOF)
    {
        encryptedChart = content - encryptionKey; //decryption
        fputc(encryptedChart, outputFile);
        c = fgetc(inputFile);
    }
    fclose(inputFile);
    fclose(outputFile);
}

char prpuosActionEncryptOrDecrypt() //TODO need to check the duplication
{
    char checkEncryptOrDecrypt;
    bool checkInput = false;

    while (checkInput == false)
    {
        printf("pleas select the prpuos action \n ( e ) for Encrypt \n ( d ) for Decrypt \n");
        scanf("%c", &checkEncryptOrDecrypt);
        if (checkEncryptOrDecrypt == 'e' || checkEncryptOrDecrypt == 'd')
        {
            checkInput = true;
        }
    }
    return checkEncryptOrDecrypt;
}

int main()
{
    char checkEncryptOrDecrypt = prpuosActionEncryptOrDecrypt();
    int encryptionKey = getEncryptionKey();
    FILE *inputFile = getInputFile();
    FILE *outputFile = getOutputFile();

    // inputFile , outputFile , encryptionKey , selectedAlgorithm
    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        encrypt(inputFile,outputFile,encryptionKey);
        break;
    case 'd':
        decrypt(inputFile,outputFile,encryptionKey);
        break;
    }

    printf("\n process ended successfully \n");
    return 0;
}