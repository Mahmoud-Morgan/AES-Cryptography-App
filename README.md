# AES-Cryptography-App
A console application is written in C programming language for encrypting/decrypting text files by Advanced Encryption Standard (AES).
using multiple processing functions for handling directory files. 

### Library
using [kokke/tiny-AES-c](https://github.com/kokke/tiny-AES-c) implementation of the AES ECB, CTR and CBC encryption algorithms written in C.

### Prerequisites
C compiler. ex: gcc, The GNU Compiler Collection.

### Running Test
- compile crypto.c file 
- press "e" => encryption , "d" => decraption
- enter the key file name. ex: key.txt. // - file extention must be (.txt) - max name length = 100 - key length must = 16
- press "d" => for handling all files in directory, "f" => for handling a spasifc file. 
- enter the file.txt name. ex: test.txt , or directory name.
- Note: - file extention must be (.txt) - max name length(file/directory) = 100.

##### handling a spasifc file output:
"fileName_operationName.txt"
- encryption => test.txt_encrypted.txt.
- decraption => test.txt_edecrypted.txt.

##### handling a directory files output:
- foreach file => "directoryName_fileName_operationName.txt"


