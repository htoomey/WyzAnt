//  Programm: Vigenere Cipher Encryptio/Decryption
//  Written By: Harold Toomey
//  Date: 22 Feb 2021
//  Updated: 28 Feb 2021


#include <stdio.h>		// printf(), fopen(), fclose()
#include <stdlib.h>     // exit()
#include <ctype.h>		// tolower()
#include <string.h>

#define MAX_CHARS 512

// Function prototypes
void EncryptVigenereCipher(char *key, char *plaintext, char *ciphertext);
void DecryptVigenereCipher(char *key, char *plaintext, char *ciphertext);
void ReadTextfileToBuffer(char *textFilename, char *buffer);
void WriteBufferToTextfile(char *textFilename, char *buffer);
void ValidateBuffers(char *key, char *plaintext, char *ciphertext);
void PrintText(char *key, char *plaintext, char *ciphertext);


int main(int argc, char *argv[])
{
	// Buffers
	char key[MAX_CHARS];
	char plaintext[MAX_CHARS];
	char ciphertext[MAX_CHARS];

	// File Names
	char *keyFilename        = "key.txt";
	char *plaintextFilename  = "plaintext.txt";
	char *ciphertextFilename = "ciphertext.txt";

	// 1. Read the key text file into the key text buffer
	ReadTextfileToBuffer(keyFilename, key);

	// 2. Read the plain text file into the plain text buffer
	ReadTextfileToBuffer(plaintextFilename, plaintext);

	// 3. Lowercase all text, then make sure only alpha characters are in the buffers
	ValidateBuffers(key, plaintext, ciphertext);

	// 4. Encrypt the plain text into cipher text
	EncryptVigenereCipher(key, plaintext, ciphertext);

	// 5. Write the cipher text buffer to the cipher text file
	WriteBufferToTextfile(ciphertextFilename, ciphertext);

	// 6. Show key and text both before and after encryption
	printf("Encrypted Text:\n");
	PrintText(key, plaintext, ciphertext);

	// 7. Dercrypt the cipher text and store it as plain text
	DecryptVigenereCipher(key, plaintext, ciphertext);

	// 8. Show key and text both before and after decryption
	printf("Decrypted Text:\n");
	PrintText(key, plaintext, ciphertext);

	return (0);	// Success
}


// ==================================================================================
// Encrypt plain text into cipher text using the key
// ==================================================================================
void EncryptVigenereCipher(char *key, char *plaintext, char *ciphertext)
{
	int i;
	int keyLen;
	int plaintextLen;

	// Store the buffer lengths for efficiency since they are constant
	keyLen = strlen(key);
	plaintextLen = strlen(plaintext);

	// Encrypt the plain text and store the cipher text as lowercase
	for (i = 0; i < plaintextLen; i++) {
		// THE ENCRYPTION ALGORITHM
	 	ciphertext[i] = (((plaintext[i] - 'a') + (key[i % keyLen] - 'a')) % 26) + 'a';
	}
	ciphertext[i] = '\0';
}


// ==================================================================================
// Decrypt cipher text into plain text using the key
// ==================================================================================
void DecryptVigenereCipher(char *key, char *plaintext, char *ciphertext) {
	int i;
	int keyLen;
	int ciphertextLen;

	// Store the buffer lengths for efficiency since they are constant
	keyLen = strlen(key);
	ciphertextLen = strlen(ciphertext);

	// Decrypt the cipher text and store the plain text as lowercase
	for (i = 0; i < ciphertextLen; i++) {
		// THE DECRYPTION ALGORITHM
	 	plaintext[i] = (((ciphertext[i] - 'a') - (key[i % keyLen] - 'a') + 26) % 26) + 'a';
	}
	plaintext[i] = '\0';
}


// ==================================================================================
// Read a text file into a text buffer
// ==================================================================================
void ReadTextfileToBuffer(char *textFilename, char *buffer) {
	int i;
	char charBuffer;
	FILE *fp;

	// Open the text file for read
	fp = fopen(textFilename, "r");
	if (fp == NULL)
	{
		printf("ERROR: Text file \"%s\" could not be opened for read.\n", textFilename);
		exit(1);
	}

	// Read in the text and store it into the buffer
	// We can use fgets() instead of fgetc(), however it stops at '\n' or EOF.
	//fgets(buffer, MAX_CHARS, fp);	
	for (i = 0; (charBuffer = fgetc(fp)) != EOF; i++) {
		buffer[i] = charBuffer;
	}
	buffer[i] = '\0';		// NULL terminate the text string

	fclose(fp);			// Close the text file
}


// ==================================================================================
// Write a text buffer to a text file
// ==================================================================================
void WriteBufferToTextfile(char *textFilename, char *buffer) {
	FILE *fp;

	// Open the text file for write
	fp = fopen(textFilename, "w");
	if (fp == NULL)
	{
		printf("ERROR: Text file \"%s\" could not be opened for write.\n", textFilename);
		exit(5);
	}

	fputs(buffer, fp);	// Write the text buffer text to the text file

	fclose(fp);			// Close the text file
}


// ==================================================================================
// Lowercase all text, then make sure only alpha characters are in the buffers
// ==================================================================================
void ValidateBuffers(char *key, char *plaintext, char *ciphertext) {
	int bufferLen;
	int i;

	// Validate key text buffer
	bufferLen = strlen(key);
	for (i = 0; i < bufferLen; i++) {
		key[i] = tolower(key[i]);	// Lowercase the key text string for convenience
		if ((key[i] < 'a') || (key[i] > 'z')) {	// Check for alpha characters ONLY
			printf("ERROR: Non-alpha character '%c' detected in key text file.\n", key[i]);
			exit(2);
		}
	}

	// Validate plain text buffer
	bufferLen = strlen(plaintext);
	for (i = 0; i < bufferLen; i++) {
		plaintext[i] = tolower(plaintext[i]);	// Lowercase the plain text string for convenience
		if ((plaintext[i] < 'a') || (plaintext[i] > 'z')) {	// Check for alpha characters ONLY
			printf("ERROR: Non-alpha character '%c' detected in plain text file.\n", plaintext[i]);
			exit(3);
		}
	}

	// Validate cipher text buffer
	bufferLen = strlen(ciphertext);
	for (i = 0; i < bufferLen; i++) {
		ciphertext[i] = tolower(ciphertext[i]);	// Lowercase the cipher text string for convenience
		if ((ciphertext[i] < 'a') || (ciphertext[i] > 'z')) {	// Check for alpha characters ONLY
			printf("ERROR: Non-alpha character '%c' detected in cipher text file.\n", ciphertext[i]);
			exit(4);
		}
	}
}


// ==================================================================================
// Print the key, plain text, and cipher text to the console
// ==================================================================================
void PrintText(char *key, char *plaintext, char *ciphertext) {
	printf("\tKey:         \"%s\"\n", key);
	printf("\tPlain Text:  \"%s\"\n", plaintext);
	printf("\tCipher Text: \"%s\"\n", ciphertext);
	printf("\n");
}

// ==================================================================================
