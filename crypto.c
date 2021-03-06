#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include <libgen.h>
//#include <pthread.h>
#include <dirent.h>
#include "aes.h"
#include "aes.c"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static char checkEncryptOrDecrypt;
static char checkDirectoryOrFile;
static char inputFileName[100];
static char directoryName[100];

#if defined(AES256)
static const uint8_t keyLength = 32;
#elif defined(AES192)
static const uint8_t keyLength = 24;
#elif defined(AES128)
static const int keyLength = 16; // 128bit key
#endif

static uint8_t encryptionKey[16];

void prpuosActionEncryptOrDecrypt()
{
    bool checkInput = false;
    do
    {
        printf("pleas select the prpuos action \n ( e ) for Encrypt \n ( d ) for Decrypt \n");
        scanf(" %s", &checkEncryptOrDecrypt);
        printf("entered chart = %c \n", checkEncryptOrDecrypt);
        if (checkEncryptOrDecrypt == 'e' || checkEncryptOrDecrypt == 'd')
        {
            checkInput = true;
        }
    } while (checkInput == false);

    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        printf("encryption process is selected ... \n\n");
        break;
    case 'd':
        printf("decryption process is selected ... \n\n");
        break;
    }
}

FILE *getInputFile()
{
    FILE *inputFile;
    bool checkInput = false;
    do
    {
        printf("Enter the filename to open for reading (max name length = 100) \n");
        scanf("%s", inputFileName);
        // Open one file for reading
        inputFile = fopen(inputFileName, "r");
        if (inputFile == NULL)
        {
            printf("Cannot open file %s \n", inputFileName);
        }
        else
        {
            checkInput = true;
        }
    } while (checkInput == false);
    return inputFile;
}

unsigned long long int countChars(FILE *inputFile)
{
    unsigned long long int charsNumber = 0;
    char c;
    c = fgetc(inputFile);
    while (c != EOF)
    {
        charsNumber++;
        c = fgetc(inputFile);
    }
    rewind(inputFile);
    printf("number of chars = %llu  \n", charsNumber);
    return charsNumber;
}

FILE *getEncryptionKeyFile()
{
    char filename[100];
    FILE *keyFile;
    bool checkInput = false;
    do
    {
        printf("Enter the filename of the encryption Key (max name length = 100 / key length must = 16) \n");
        scanf("%s", filename);
        // Open one file for reading
        keyFile = fopen(filename, "r");
        if (keyFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }
        else
        {
            printf(" %s file is opened as a key file \n\n", filename);
            checkInput = true;
        }
    } while (checkInput == false);
    return keyFile;
}

void getEncryptionKey()
{
    bool checkInput = false;
    do
    {
        FILE *keyFile = getEncryptionKeyFile();
        char content;
        int i = 0;
        content = fgetc(keyFile);
        while (content != EOF)
        {
            encryptionKey[i] = content;
            content = fgetc(keyFile);
            i++;
            if (i > keyLength)
            {
                break;
            }
        }
        if (i > keyLength)
        {
            printf("long key, pleas enter valid key file with length = %d \n", keyLength);
        }
        else if (i <= keyLength - 1)
        {
            printf("short key, pleas enter valid key file with length = %d \n", keyLength);
        }
        else
        {
            checkInput = true;
        }
    } while (checkInput == false);
}

void printuintToChar(uint8_t *codedText, unsigned long long int codedTextSize)
{
    char content;
    unsigned long long int i;
    for (i = 0; i < codedTextSize; i++)
    {
        content = (char)codedText[i];
        printf("%c", content);
    }
}

void printUint(uint8_t *codedText, unsigned long long int codedTextSize)
{
    unsigned long long int i;
    for (i = 0; i < codedTextSize; i++)
    {
        printf("%x", codedText[i]);
    }
}

void exportEncryptedFile(uint8_t *codedText, unsigned long long int codedTextSize)
{
    char concatenatName[] = "_encrypted.txt";
    int directoryNameLength = strlen(directoryName);
    int inputFileNameLength = strlen(inputFileName);
    int concatenatNameLength = strlen(concatenatName);
    int newFileNameLength = directoryNameLength + inputFileNameLength + concatenatNameLength +1;
    char newFileName[newFileNameLength];

    if (directoryNameLength > 0)
    {
        strcpy(newFileName, directoryName);
        strcat(newFileName, "_");
        strcat(newFileName, inputFileName);
        strcat(newFileName, concatenatName);
    }
    else
    {
        strcpy(newFileName, inputFileName);
        strcat(newFileName, concatenatName);
    }

    FILE *outputFile = fopen(newFileName, "w+");
    unsigned long long int i;
    for (i = 0; i < codedTextSize; i++)
    {
        fprintf(outputFile, "%x", codedText[i]);
    }
    fclose(outputFile);

    printf("\n File created and saved successfully. \n");
}

void exportDecryptedFile(uint8_t *codedText, unsigned long long int codedTextSize)
{
    char concatenatName[] = "_decrypted.txt";
    int directoryNameLength = strlen(directoryName);
    int inputFileNameLength = strlen(inputFileName);
    int concatenatNameLength = strlen(concatenatName);
    int newFileNameLength = directoryNameLength + inputFileNameLength + concatenatNameLength +1;
    char newFileName[newFileNameLength];


    if (directoryNameLength > 0)
    {
        strcpy(newFileName, directoryName);
        strcat(newFileName, "_");
        strcat(newFileName, inputFileName);
        strcat(newFileName, concatenatName);
    }
    else
    {
        strcpy(newFileName, inputFileName);
        strcat(newFileName, concatenatName);
    }
   
    FILE *outputFile = fopen(newFileName, "w+");
    unsigned long long int i;
    for (i = 0; i < codedTextSize; i++)
    {
        fprintf(outputFile, "%c", (char)codedText[i]);
    }
    rewind(outputFile);
    fclose(outputFile);

    printf("\n File created and saved successfully. \n");
}

void encryption(FILE *inputFile)
{
    unsigned long long int charsNumber = countChars(inputFile);
    uint8_t textToEncrypt[charsNumber];
    char content;
    unsigned long long int j = 0;
    for (j = 0; j < charsNumber; j++)
    {
        content = fgetc(inputFile);
        textToEncrypt[j] = content;
    }
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, encryptionKey);
    printf("encryptedtext:\n");
    AES_ECB_encrypt(&ctx, textToEncrypt);
    unsigned long long int codedTextSize = sizeof(textToEncrypt) / sizeof(textToEncrypt[0]);
   // printUint(textToEncrypt, codedTextSize);
    exportEncryptedFile(textToEncrypt, codedTextSize);
    printf("\n");
}

void decryption(FILE *inputFile)
{
    unsigned long long int charsNumber = countChars(inputFile);
   
    uint8_t textToDecrypt[charsNumber / 2];
    unsigned long long int i = 0;
    uint8_t contant;
    while (!feof(inputFile))
    {
        if (fscanf(inputFile, "%2hhx", &contant) == 1)
        {
            textToDecrypt[i] = contant;
            i++;
            printf("i = %d\n",i);
        }else{
            printf("this file not encrypted by this app \n");
            exit(0);
        }
    }
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, encryptionKey);
    AES_ECB_decrypt(&ctx, textToDecrypt);
    unsigned long long int codedTextSize = sizeof(textToDecrypt) / sizeof(textToDecrypt[0]);
  //  printuintToChar(textToDecrypt, codedTextSize);
    exportDecryptedFile(textToDecrypt, codedTextSize);
    printf("\n");
}

void handleFileOrDirectory()
{
    bool checkInput = false;
    do
    {
        printf("for handling all files in directory pleas enter ( d )\nfor handling a spasifc file pleas enter ( f ) \n \n");
        scanf(" %s", &checkDirectoryOrFile);
        printf("entered chart = %c \n", checkDirectoryOrFile);
        if (checkDirectoryOrFile == 'd' || checkDirectoryOrFile == 'f')
        {
            checkInput = true;
        }
    } while (checkInput == false);

    switch (checkDirectoryOrFile)
    {
    case 'd':
        printf("the process will act on all files in the directory ... \n\n");
        break;
    case 'f':
        printf("the process will act on a spasifc file ... \n\n");
        break;
    }
}

void encryptOrDecryptSwitcher(FILE *inputFile)
{
    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        encryption(inputFile);
        break;
    case 'd':
        decryption(inputFile);
        break;
    }
}

void handlingFileProcess()
{
    FILE *inputFile = getInputFile();
    encryptOrDecryptSwitcher(inputFile);
    fclose(inputFile);
}

DIR *getDirectory()
{
    DIR *directory;
    bool checkInput = false;
    do
    {
        printf("Enter the directory extantion (max name length = 200) \n");
        scanf("%s", directoryName);
        directory = opendir(directoryName);
        if (directory == NULL)
        {
            printf("Cannot open directory %s \n", directoryName);
        }
        else
        {
            printf(" %s directory is opened \n", directoryName);
            checkInput = true;
        }
    } while (checkInput == false);
    return directory;
}

void handlingAllFilesInDirectoryProcess()
{
    DIR *directory = getDirectory();
    int numberOfFiles = 0;
    struct dirent *dir;
    FILE *inputFile;
    pid_t child_pid, w_pid;
    int status = 0;
    if (directory)
    {
        while ((dir = readdir(directory)) != NULL)
        {
            if (dir->d_type != DT_REG)
                continue;
            char *last = strrchr(dir->d_name, '.');
            if (last + 1 != NULL && strcmp(last + 1, "txt") == 0)
            {   numberOfFiles++;
                if ((child_pid = fork()) == 0)
                {
                    char filePath[100];
                    strcpy(inputFileName, dir->d_name);
                    strcpy(filePath, directoryName);
                    strcat(filePath, "/");
                    strcat(filePath, inputFileName);
                    printf("filePath: %s \n", filePath);
                    inputFile = fopen(filePath, "r");
                    if (inputFile == NULL)
                    {
                        printf("Cannot open file %s \n", inputFileName);
                    }
                    else
                    {rewind(inputFile);
                        encryptOrDecryptSwitcher(inputFile);
                    }
                    rewind(inputFile);
                    fclose(inputFile);
                    exit(0);
                }
            }
        }
    }
    while ((w_pid = wait(&status)) > 0);
    // {
    //     printf("w_pid = %d , status = %d \n",w_pid,status);
    // }
    rewinddir(directory);
    closedir(directory);
}

int main()
{
    prpuosActionEncryptOrDecrypt();
    getEncryptionKey();
    handleFileOrDirectory();
    switch (checkDirectoryOrFile)
    {
    case 'd':
        handlingAllFilesInDirectoryProcess();
        break;
    case 'f':
        handlingFileProcess();
        break;
    }
    printf("\n process ended successfully \n");
    return 0;
}