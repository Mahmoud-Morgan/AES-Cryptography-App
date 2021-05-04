#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include "aes.h"
#include "aes.c"

static char checkEncryptOrDecrypt;
static FILE *inputFile;

#if defined(AES256)
const uint8_t len = 32;
#elif defined(AES192)
const uint8_t len = 24;
#elif defined(AES128)
const uint8_t len = 16;
#endif
static const int keyLength = 16; // 128bit key
static uint8_t encryptionKey[keyLength];
static unsigned long long int chartsNumber;


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

void getInputFile()
{
    char filename[100];
    bool checkInput = false;
    do
    {
        printf("Enter the filename to open for reading (max name length = 100) \n");
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
}

void countCharts()
{
    chartsNumber = 0;
    char c;
    c = fgetc(inputFile);
    while (c != EOF)
    {
        chartsNumber++;
        c = fgetc(inputFile);
    }
    rewind(inputFile);
    printf("number of charts = %llu  \n", chartsNumber);
}

FILE *getEncryptionKeyFile()
{
    char filename[100];
    FILE *keyFile;
    bool checkInput = false;
    do
    {
        printf("Enter the filename(max name length = 100)of the encryption Key (key length must = 16) \n");
        scanf("%s", filename);
        // Open one file for reading
        keyFile = fopen(filename, "r");
        if (keyFile == NULL)
        {
            printf("Cannot open file %s \n", filename);
        }
        else
        {
            checkInput = true;
        }
    } while (checkInput == false);
    return keyFile;
}

void getEncryptionKey(FILE *keyFile) // need length validation
{
    char content;
    for (int i = 0; i < keyLength; i++)
    {
        content = fgetc(keyFile);
        encryptionKey[i] = content;
    }
}

void printuintToChar(uint8_t *codedText, unsigned long long int  codedTextSize)
{
    char content;
    unsigned long long int i;
    for ( i = 0; i < codedTextSize; i++)
    {
        content = (char)codedText[i];
        printf("%c", content);
    }
}

void printUint(uint8_t *codedText, unsigned long long int  codedTextSize)
{
    unsigned long long int  i;
    for ( i = 0; i < codedTextSize; i++)
    {
        printf("%x", codedText[i]);
    }
}

void encryption()
{
    uint8_t textToEncrypt[chartsNumber];
    char content;
    unsigned long long int j = 0;   
    for (j = 0; j < chartsNumber; j++)
    {
        content = fgetc(inputFile);
        textToEncrypt[j] = content;
    }
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, encryptionKey);
    printf("encryptedtext:\n");
    AES_ECB_encrypt(&ctx, textToEncrypt);
    unsigned long long int  codedTextSize = sizeof(textToEncrypt) / sizeof(textToEncrypt[0]);
    printf("size of encrypted array = %llu \n",codedTextSize);
    printf("\n \n \n");
    printUint(textToEncrypt, codedTextSize);
    printf("\n \n \n");
}

void decryption()
{
    uint8_t textToDecrypt[chartsNumber / 2];
    unsigned long long int  i = 0;
    uint8_t contant;
    while (!feof(inputFile))
    {
        if (fscanf(inputFile, "%2hhx", &contant) == 1)
        {
            textToDecrypt[i] = contant;
            i++;
        }
    }
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, encryptionKey);
    AES_ECB_decrypt(&ctx, textToDecrypt);
    unsigned long long int  codedTextSize = sizeof(textToDecrypt) / sizeof(textToDecrypt[0]);
    printuintToChar(textToDecrypt, codedTextSize);
    printf("\n");
}

int main()
{
    prpuosActionEncryptOrDecrypt();

    getInputFile();
    countCharts();

    FILE *KeyFile = getEncryptionKeyFile(); // 128bit key
    getEncryptionKey(KeyFile);

    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        encryption();
        break;
    case 'd':
        decryption();
        break;
    }
    printf("\n process ended successfully \n");
    return 0;
}