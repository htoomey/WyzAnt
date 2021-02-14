//
// Program: Bank Transactions file IO with error handling
// Written by: Harold Toomey
// Filename: bank.c
//

// Include Files
#include <stdlib.h>		// exit(), system()
#include <stdio.h>		// printf(), sprintf(), fprintf(), scanf()
#include <time.h>		// time(), localtime(), asctime()
#include <string.h>		// strncpy()
#include <ctype.h>		// toupper()
#include <errno.h>		// errno() errno codes: http://www.cplusplus.com/reference/system_error/errc/
#include <stdbool.h>	// Bool data type, true, false

// Function Prototypes
void WriteLogEntry(char transactionTypeCode, double transactionAmount, double startingBalance, double endingBalance);
void OpenAccountFiles(int accountNumber);
void CloseAccountFiles(int accountNumber);
bool AccountExists(int accountNumber);
void NewAccount(int accountNumber);
void Deposit(int accountNumber);
void Withdraw(int accountNumber);
double Inquiry(int accountNumber);
void Fraud(int accountNumber);
void CloseAccount(int accountNumber);
void ChangeAccount(void);
void QuitProgram(bool *quit);

// Defines
#define DEBUG 0			// Set to 1 to see debug printouts, otherwise set to 0

// Global Variables
FILE *AFilehandle = NULL;
FILE *LFilehandle = NULL;
bool AccountFilesExist = false;
bool PromptForAccountNumber = true;



// *********************************************************************************************
// Main - Program always starts here.
// **********************************************************************************************************

int main (int argc, char *argv[])
{
  int accountNumber;
  char AFilename[13];		// e.g. A123456.txt
  char LFilename[13];
  char transactionTypeCode;
  double transactionAmount;
  double startingBalance;
  double endingBalance;
  int numCharsWritten;
  bool quit = false;

  do
  {
    // Display account number prompt when a new account needs to be created
    if (PromptForAccountNumber == true)
    {
      // Have the customer login by keyboard entry their account number, six digits e.g. 123456.
      printf("\n***********************************\n");
      printf("* Eric's Bank Transaction Program *\n"); //**//
      printf("***********************************\n\n");
      printf("  Customer Login Screen:\n");
      printf("  Enter your 6-digit account number: ");
      scanf("%d", &accountNumber);
      printf("\n");

      // Check if account number is 6 digits?
      if (accountNumber < 0 || accountNumber > 999999) {
          printf("ERROR: The account number entered (%d) is not 6-digits long.  Goodbye!\n", accountNumber);
          exit(-1);
      }
      PromptForAccountNumber = false;
    }

	  // Transaction type codes:
	  // "F"=Fraud  "N"=New Account  "W"=Withdraw  "D"=Deposit  "I"=Inquiry  "Q"=Quit "C"=Close
	  // Prompt for transaction type
	  printf("\nEnter Transaction Type:\n");
	  printf("\t\"N\" = New Account\n");
	  printf("\t\"D\" = Deposit\n");
	  printf("\t\"W\" = Withdraw\n");
	  printf("\t\"I\" = Inquiry\n");
	  printf("\t\"F\" = Fraud\n");
	  printf("\t\"C\" = Close Account\n");
 	  printf("\t\"A\" = Change Account\n");
	  printf("\t\"Q\" = Quit\n       > ");
	  getchar();		// Clear the buffer after dirty scanf() function call
	  transactionTypeCode = toupper(getchar());		// Uppercase for ease
  	  printf("\n");

	  // Check for valid transaction input, then execute if valid
	  switch (transactionTypeCode) {
		case 'N': NewAccount(accountNumber); break;
        case 'D': Deposit(accountNumber); break;
		case 'W': Withdraw(accountNumber); break;
		case 'I': Inquiry(accountNumber); break;
		case 'F': Fraud(accountNumber); break;
		case 'C': CloseAccount(accountNumber); break;
		case 'A': ChangeAccount(); break;
		case 'Q': QuitProgram(&quit); break;
		default: printf("\nInvalid transaction code entered (%c).  Try again.\n", transactionTypeCode); break;
	  }
  }	while (quit != true);		// Loop until keyboard entry transaction type = 'Q' for quit.

  // Clean up before exiting
  void CloseAccountFiles(int accountNumber);

  printf("\nGoodbye!\n");
  return 0;
}


// *********************************************************************************************
// Writes Log entries.
//
// File "L123456.txt" will log one line starting with time stamp, transaction type code, 
// transaction amount, starting balance, ending balance.
// e.g. Sun Nov 29 2020 21:11:13 N 0.0 0.0 0.0
// **********************************************************************************************************

void WriteLogEntry(char transactionTypeCode, double transactionAmount, double startingBalance, double endingBalance)
{
  time_t timeStamp;
  struct tm *timeInfo;
  char timeStampString[80];
  int numCharsWritten;
  
  if (LFilehandle == NULL) 		// Make sure the log file is open
  {
    errno = EBADF;		// EBADF = bad_file_descriptor
    // To identify file IO errors when they occur use fprintf stderr to print a message and errno
    fprintf(stderr, "ERROR %d: Log file not open for write in WriteLogEntry().\n", errno);
    // and then exit with EXIT_FAILURE;
    exit(EXIT_FAILURE);
  }
  
 // Grab the Time Stamp
  time(&timeStamp);						// Get current time
  timeInfo = localtime(&timeStamp);		// Convert to human readable components
  strncpy(timeStampString, asctime(timeInfo), sizeof(timeStampString));
  timeStampString[strlen(timeStampString)-1] = '\0';  	// Remove the \n character at end of string

  // Write a line to the Log File
  numCharsWritten = fprintf(LFilehandle, "%s %c %.2lf %.2lf %.2lf\n", timeStampString, transactionTypeCode, transactionAmount, startingBalance, endingBalance);
  if (numCharsWritten < 0)
  {
    errno = EIO;		// EIO = io_error
    fprintf(stderr, "ERROR %d: File IO error writing to the log file in WriteLogEntry().\n", errno);
    exit(EXIT_FAILURE);
  }
}


// **********************************************************************************************************
// Opens the Account and Log files, if they exist.
// **********************************************************************************************************

void OpenAccountFiles(int accountNumber)
{
  char AFilename[13];		// e.g. A123456.txt
  char LFilename[13];		// e.g. L123456.txt
  
  // Check if Account file already exists
  // if the account files do not exist yet, do not try to open them
  AccountFilesExist = AccountExists(accountNumber);
  if(!AccountFilesExist)
  {
#if DEBUG
    printf("Account %06d does not exist yet, so it cannot be opened.\n", accountNumber);
#endif
    return;
  }
    
  // Open Account file
  sprintf(AFilename, "A%06d.TXT", accountNumber);
  
  // r+ = read/update: Open a file for update (both for input and output). The file must exist.
  AFilehandle = fopen(AFilename, "r+");
  if (AFilehandle == NULL)		// Open the file in read/update mode
  {
    errno = EBADF;		// EBADF = bad_file_descriptor = 9
    fprintf(stderr, "ERROR %d: File IO error writing to the account file \"%s\" in OpenAccountFiles().\n", errno, AFilename);
    exit(EXIT_FAILURE);
  }
#if DEBUG
  printf("\tAccount file \"%s\" has been opened successfully.\n", AFilename);
#endif


  // Open Log file
  sprintf(LFilename, "L%06d.TXT", accountNumber);

  LFilehandle = fopen(LFilename, "a+");
  if (LFilehandle == NULL)		// Open the log file in append mode
  {
    errno = EBADF;		// EBADF = bad_file_descriptor
    fprintf(stderr, "ERROR %d: File IO error writing to the log file \"%s\"in OpenAccountFiles().\n", errno, LFilename);
    exit(EXIT_FAILURE);
  }

#if DEBUG
  printf("\tLog file \"%s\" has been opened successfully.\n", LFilename);
#endif
}


// **********************************************************************************************************
// Closes the Account and Log files, if they are open.
// **********************************************************************************************************

void CloseAccountFiles(int accountNumber)
{
  if (AFilehandle != NULL)
  {
    fclose (AFilehandle);
    AFilehandle = NULL;
  }
  
  if (LFilehandle != NULL)
  {
    fclose (LFilehandle);
    LFilehandle = NULL;
  }
}


// **********************************************************************************************************
// Determines if the Account file already exists.
// It assumes that if the Account files exists, then the Log file exists too.
// **********************************************************************************************************

bool AccountExists(int accountNumber)
{
  char AFilename[13];		// e.g. A123456.txt
  
#if DEBUG
  printf("Checking if the account already exists.\n");
#endif

  CloseAccountFiles(accountNumber);
    
  // Test if the Account file exists
  sprintf(AFilename, "A%06d.TXT", accountNumber);
  AFilehandle = fopen(AFilename, "r");	// r = read: Open file for input operations. The file must exist.
  if (AFilehandle == NULL)
  {
    AccountFilesExist = false;
  }
  else
  {
    AccountFilesExist = true;
  }
  
  // Since file exists, we need to close it.
  if (AFilehandle != NULL)
  {
    fclose (AFilehandle);
    AFilehandle = NULL;
  }
  
#if DEBUG
  if (AccountFilesExist == true)
  {
    printf("Account %06d already exists.\n", accountNumber);
  }
  else
  {
    printf("Account %06d does not already exist.\n", accountNumber);
  }
#endif
  
  return (AccountFilesExist);
}


// **********************************************************************************************************
// implement new function -- create
// Transaction: New
//
// Description:
// Creates the Account and Log files for this account.
// Sets account balance to $0.00.
//
// NOTE:
// w+ = write: Create an empty file for output operations. If a file with the same name already exists, 
// its contents are discarded and the file is treated as a new empty file.
// **********************************************************************************************************

void NewAccount(int accountNumber)
{
  char AFilename[13];		// e.g. A123456.txt
  char LFilename[13];		// e.g. L123456.txt
  int numCharsWritten;
  
#if DEBUG
  printf("Creating a New account.\n");
#endif
  
  CloseAccountFiles(accountNumber);
  
  // Check if Account file already exists
  AccountFilesExist = AccountExists(accountNumber);
  if(AccountFilesExist)
  {
    printf("Account %06d already exists.\n", accountNumber);
    return;
  }
  
  // Create the files with the program as New Account if they do not exist.
    
  // #1 Create Account File using the account number.
  // Their two decimal float account balance will be associated with a text file name starting with "A"
  // followed by their account number and having extension .txt  example A123456.txt
  sprintf(AFilename, "A%06d.TXT", accountNumber);
  AFilehandle = fopen(AFilename, "w+");
  if (AFilehandle == NULL)		// Make sure the file was created or opened correctly
  {
    errno = EBADF;		// EBADF = bad_file_descriptor
    // To identify file IO errors when they occur use fprintf stderr to print a message and errno
    fprintf(stderr, "ERROR %d: File IO error writing to the account file \"%s\" in NewAccount().\n", errno, AFilename);
    // and then exit with EXIT_FAILURE;
    exit(EXIT_FAILURE);
  }
  
#if DEBUG
  printf("Account file \"%s\" has been created successfully.\n", AFilename);
#endif


  // #2 Create Log File using the account number.
  // Their cumulative transaction log file will be associated with a text file name starting with "L"
  // followed by their account number and having extension .txt  example L123456.txt
  sprintf(LFilename, "L%06d.TXT", accountNumber);

  LFilehandle = fopen(LFilename, "w+");
  if (LFilehandle == NULL)		// Make sure the file was created or opened correctly
  {
    errno = EBADF;		// EBADF = bad_file_descriptor
    // To identify file IO errors when they occur use fprintf stderr to print a message and errno
    fprintf(stderr, "ERROR %d: File IO error writing to the log file \"%s\" in NewAccount().\n", errno, LFilename);
    // and then exit with EXIT_FAILURE;
    exit(EXIT_FAILURE);
  }

#if DEBUG
  printf("\tLog file     \"%s\" has been created successfully.\n", LFilename);
#endif


  // Copy a zero balance into the newly creatd Account file.
  // e.g. "A123456.txt" will be set to 0.0
  numCharsWritten = fprintf(AFilehandle, "%3.2f\n", 0.0);
  if (numCharsWritten < 0)
  {
    errno = EIO;		// EIO = io_error
    fprintf(stderr, "ERROR %d: File IO error writing to the log file \"%s\" in NewAccount().\n", errno, LFilename);
    exit(EXIT_FAILURE);
  }

  // File "L123456.txt" will log one line starting with time stamp, transaction type code, transaction amount, starting balance, ending balance.
  // e.g. Sun Nov 29 2020 21:11:13 N 0.0 0.0 0.0
  WriteLogEntry('N', 0.0, 0.0, 0.0);
  
  CloseAccountFiles(accountNumber);
  
  printf("New account %06d successfully opened.\n", accountNumber);
}


// **********************************************************************************************************
// Makes a deposit into the Account.
// **********************************************************************************************************

void Deposit(int accountNumber)
{
  double transactionAmount;
  double startingBalance;
  double endingBalance;
  int numCharsWritten;
  char AFilename[13];		// e.g. A123456.txt
  char LFilename[13];		// e.g. L123456.txt
  
#if DEBUG
  printf("\nMaking a Deposit.\n");
#endif

  OpenAccountFiles(accountNumber);
  if (AFilehandle == NULL)
  {
    printf("Sorry, you need to create a new account (N) before you can make a Deposit.\n");
    return;
  }

  // Retrieve deposit amount
  printf("Enter amount to deposit: $");
  scanf("%lf", &transactionAmount);

  // Retrieve current balance
  if (fscanf(AFilehandle, "%lf", &startingBalance) <= 0)
  {
    errno = EIO;		// EIO = file_io_error
    sprintf(AFilename, "A%06d.TXT", accountNumber);
    fprintf(stderr, "ERROR %d: File IO error reading from the account file \"%s\" in Deposit().\n", errno, AFilename);
    exit(EXIT_FAILURE);
  }
  CloseAccountFiles(accountNumber);

  endingBalance = startingBalance + transactionAmount;
  
  // Update account balance by adding amount.
  OpenAccountFiles(accountNumber);
  numCharsWritten = fprintf(AFilehandle, "%.2lf\n", endingBalance);
  if (numCharsWritten < 0)
  {
    errno = EIO;		// EIO = io_error
    sprintf(LFilename, "L%06d.TXT", accountNumber);
    fprintf(stderr, "ERROR %d: File IO error writing to the account file \"%s\" in Deposit().\n", errno, LFilename);
    exit(EXIT_FAILURE);
  }

  // Make log entry.
  WriteLogEntry('D', transactionAmount, startingBalance, endingBalance); 
  CloseAccountFiles(accountNumber);
  
  printf("Deposit of $%.2lf to account %06d was successful.\n", transactionAmount, accountNumber);
}


// **********************************************************************************************************
// Makes a Withdrawl from the Account.
// **********************************************************************************************************

void Withdraw(int accountNumber)
{
  double transactionAmount;
  double transactionFee = 1.0;
  double startingBalance;
  double endingBalance;
  int numCharsWritten;
  char AFilename[13];		// e.g. A123456.txt
  char LFilename[13];		// e.g. L123456.txt
  char systemCommand[25];	// Holds the system call command string
  
#if DEBUG
  printf("\nMaking a Withdrawl.\n");
#endif  
  
  OpenAccountFiles(accountNumber);
  if (AFilehandle == NULL)
  {
    printf("Sorry, you need to create a new account (N) and make a deposit (D) before you can make a Withdrawl.\n");
    return;
  }
    
  // Retrieve withdrawl amount
  printf("Enter amount to withdraw: $");
  scanf("%lf", &transactionAmount);
  
  // Retrieve current balance
  if (fscanf(AFilehandle, "%lf", &startingBalance) <= 0)
  {
    errno = EIO;		// EIO = file_io_error
    sprintf(AFilename, "A%06d.TXT", accountNumber);
    fprintf(stderr, "ERROR %d: File IO error reading from the account file \"%s\" in Withdraw().\n", errno, AFilename);
    exit(EXIT_FAILURE);
  }
  CloseAccountFiles(accountNumber);


  // Case a): Fraud
  // Withdraw if (amount plus $1.00) is greater than balance,
  if (transactionAmount + transactionFee > startingBalance)
  {
    OpenAccountFiles(accountNumber);
    // Make log entry of a fraud attempt account locked 'insufficient funds'
    printf("Withdrawl of $%.2lf from account %06d was unsuccessful.\n", transactionAmount, accountNumber);
    printf("Account %06d locked due to insufficient funds.\n", accountNumber);
    WriteLogEntry('F', transactionAmount, startingBalance, startingBalance);
    numCharsWritten = fprintf(LFilehandle, "Fraud attempt, account locked, insufficient funds.\n");
    if (numCharsWritten < 0)
    {
      errno = EIO;		// EIO = io_error
      fprintf(stderr, "ERROR %d: File IO error writing to the log file in Withdraw().\n", errno);
      exit(EXIT_FAILURE);
    }
    CloseAccountFiles(accountNumber);
    
    // then lock account from change using system("attrib +R A123456.txt");
	sprintf(AFilename, "A%06d.TXT", accountNumber);
	sprintf(systemCommand, "attrib +R %s", AFilename);
	system(systemCommand);    
	
    // and then exit with EXIT_FAILURE.
    exit(EXIT_FAILURE);
  }


  // Case b): Withdrawl
  // Withdraw if (amount plus $1.00) is equal to or less than balance,
  else
  {
    // update acount balance by subtracting sum of (amount plus $1.00).
    endingBalance = startingBalance - (transactionAmount + transactionFee);
    
    OpenAccountFiles(accountNumber);
    numCharsWritten = fprintf(AFilehandle, "%.2lf\n", endingBalance);
    if (numCharsWritten < 0)
    {
      errno = EIO;		// EIO = io_error
      sprintf(LFilename, "L%06d.TXT", accountNumber);
      fprintf(stderr, "ERROR %d: File IO error writing to the account file \"%s\" in Withdraw().\n", errno, LFilename);
      exit(EXIT_FAILURE);
    }
  
    // Make log entry.
    WriteLogEntry('W', transactionAmount, startingBalance, endingBalance); 
    CloseAccountFiles(accountNumber);
    
    printf("Withdrawl of $%.2lf from account %06d was successful.\n", transactionAmount, accountNumber);
  }
}


// **********************************************************************************************************
// Inquires the Account and shows its balance.
// **********************************************************************************************************

double Inquiry(int accountNumber)
{
  char AFilename[13];		// e.g. A123456.txt
  char systemCommand[25];	// Holds the system call command string
  char balanceString[20];	// String buffer for file reads
  double balance = 0.0;
  
#if DEBUG
  printf("\nMaking a balance Inquiry.\n");
#endif
   
  OpenAccountFiles(accountNumber);
  if (AFilehandle == NULL)
  {
    printf("Sorry, you need to create a new account (N) before you can make a balance Inquiry.\n");
    return(0.0);
  }
  CloseAccountFiles(accountNumber);

  // Unlock the account using system("attrib -R A123456.txt"); .
  sprintf(AFilename, "A%06d.TXT", accountNumber);
  sprintf(systemCommand, "attrib -R %s", AFilename);
  system(systemCommand);

  // Retrieve current balance
  OpenAccountFiles(accountNumber);
  if (fscanf(AFilehandle, "%lf", &balance) <= 0)
  {
    errno = EIO;		// EIO = file_io_error
    sprintf(AFilename, "A%06d.TXT", accountNumber);
    fprintf(stderr, "ERROR %d: File IO error reading from the account file \"%s\".\n", errno, AFilename);
    exit(EXIT_FAILURE);
  }

  WriteLogEntry('I', 0.0, balance, balance);
  CloseAccountFiles(accountNumber);
  
  // Use fprintf stderr print a message with account balance.
  fprintf(stderr, "Current balance of $%.2f in account %06d.\n", balance, accountNumber);
  
  return(balance);
}


// **********************************************************************************************************
// Reports Fraud.
// **********************************************************************************************************

void Fraud(int accountNumber)
{
#if DEBUG
  printf("\nChecking your account for Fraud.\n");
#endif

  OpenAccountFiles(accountNumber);
  if (AFilehandle == NULL)
  {
    printf("Sorry, you need to create a new account (N) before you can check for Fraud.\n");
    return;
  }
  
  // If Fraud was detected during a withdrawl, then the program would print a fraud error then exit.
  printf("No Fraud detected for account %06d.\n", accountNumber);
}


// **********************************************************************************************************
// Closes the Account by archiving the Account and Log files.
// **********************************************************************************************************

void CloseAccount(int accountNumber)
{
  char AFilenameOld[13];
  char AFilenameNew[13];
  char LFilenameOld[13];
  char LFilenameNew[13];
  double balance = 0.0;
    
#if DEBUG
  printf("\nClosing your account.\n");
#endif
        
  OpenAccountFiles(accountNumber);
  if (AFilehandle == NULL)
  {
    printf("Sorry, you need to create a new account (N) before you can Close it.\n");
    CloseAccountFiles(accountNumber);
    return;
  }
  CloseAccountFiles(accountNumber);
  
  // Make a log entry.
  balance = Inquiry(accountNumber);
  
  OpenAccountFiles(accountNumber);		// An Inguiry closes the account files.
  WriteLogEntry('C', 0.0, balance, balance);
  CloseAccountFiles(accountNumber);

  // Close the account by changing the filename to CA123456.txt  and  CL123456.txt
  // Use rename("A123456.txt", "CA123456.txt");
  sprintf(AFilenameOld, "A%06d.TXT", accountNumber);
  sprintf(AFilenameNew, "CA%06d.TXT", accountNumber);
  sprintf(LFilenameOld, "L%06d.TXT", accountNumber);
  sprintf(LFilenameNew, "CL%06d.TXT", accountNumber);
  rename(AFilenameOld, AFilenameNew);
  rename(LFilenameOld, LFilenameNew);
  
  printf("Account %06d successfully Closed.\n", accountNumber);
  AccountFilesExist = false;
  PromptForAccountNumber = true;
}


// **********************************************************************************************************
// Prompts for another Account number.
// **********************************************************************************************************

void ChangeAccount(void)
{
  PromptForAccountNumber = true;
}


// **********************************************************************************************************
// Loop until keyboard entry transaction type = 'Q' for quit.
// **********************************************************************************************************

void QuitProgram(bool *quit)
{
#if DEBUG
  printf("Quitting Bank Program.\n");
#endif

  *quit = true;
}

// **********************************************************************************************************
