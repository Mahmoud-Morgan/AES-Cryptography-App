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
static unsigned long int numberOfBlocks;


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
    // Extract characters from file
    // and store in character c
    while (c != EOF)
    {
        // Increment count for this character
        chartsNumber++;
        c = fgetc(inputFile);
    }
    rewind(inputFile);
}

void countNumberOfBlocks()
{
    if (fmod(chartsNumber, keyLength) == 0)
    {
        numberOfBlocks = (chartsNumber / keyLength);
    }
    else
    {
        numberOfBlocks = (chartsNumber / keyLength) + 1;
    }
}

static void phex(uint8_t *str)
{
    unsigned char i;
    for (i = 0; i < keyLength; ++i)
        printf("%.2x", str[i]);
    printf("\n");
}

uint8_t getPlainText(unsigned long long int *chartsNumber)
{
    uint8_t plainText[*chartsNumber];
    char content;
    unsigned long long int i = 0;
    for (i = 0; i < *chartsNumber; i++)
    {
        content = fgetc(inputFile);
        plainText[i] = content;
    }
    return *plainText;
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


void printuintToChar(uint8_t *codedText, int codedTextSize)
{
    char content;
    for (int i = 0; i < codedTextSize; i++)
    {
        content = (char)codedText[i];
        printf("%c", content);
    }
}



int main()
{
    prpuosActionEncryptOrDecrypt();

    getInputFile();
    countCharts();
    countNumberOfBlocks();
   
    FILE *KeyFile = getEncryptionKeyFile(); // 128bit key
    getEncryptionKey(KeyFile);

    uint8_t textToEncrypt[chartsNumber];
    char content;
    unsigned long long int j = 0;
    for (j = 0; j < chartsNumber; j++)
    {
        content = fgetc(inputFile);
        textToEncrypt[j] = content;
    }

    uint8_t textToDecrypt[chartsNumber/2];
    // for (j = 0; j < chartsNumber/2; j++)
    // {
    //     fscanf("%2"SCNu8,textToDecrypt[j]);
    //     j++;
    // }

    struct AES_ctx ctx;
    AES_init_ctx(&ctx, encryptionKey);

    switch (checkEncryptOrDecrypt)
    {
    case 'e':
        printf("encryptedtext:\n");
        for (uint8_t i = (uint8_t)0; i < (uint8_t)numberOfBlocks; ++i)
        {
            AES_ECB_encrypt(&ctx, textToEncrypt + i * (uint8_t)keyLength);
        }
        for (uint8_t i = (uint8_t)0; i < (uint8_t)numberOfBlocks; ++i)
        {
            phex( textToEncrypt + i * (uint8_t)keyLength);
        }
       // printuintToChar(textToEncrypt, chartsNumber);
        break;
    case 'd':
        printf("decryptedtext:\n");
        for (uint8_t i = (uint8_t)0; i < (uint8_t)numberOfBlocks; ++i)
        {
            AES_ECB_decrypt(&ctx, textToDecrypt + i * (uint8_t)keyLength);
        }
        printf(" original  \n");
        printuintToChar(textToDecrypt, chartsNumber);
        break;
    }

         printf("decryptedtext:\n");
        for (uint8_t i = (uint8_t)0; i < (uint8_t)numberOfBlocks; ++i)
        {
            AES_ECB_decrypt(&ctx, textToEncrypt + i * (uint8_t)keyLength);
        }
        printf(" original  \n");
        printuintToChar(textToEncrypt, chartsNumber);

    printf("\n");

    printf("\n process ended successfully \n");
    return 0;
}