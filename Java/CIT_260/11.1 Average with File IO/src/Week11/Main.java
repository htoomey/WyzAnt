// ----------------------------
// Heather Toomey
// CIT-260
// Problem W11dot1
// July 5, 2020
// ----------------------------

import java.util.Scanner;
import java.lang.Exception;
import java.io.*;
//import java.io.File;
//import java.io.IOException;   // For creating a file
//import java.io.FileNotFoundException;
//import java.io.PrintWriter;
import java.lang.System;        // For System.exit()
import java.lang.NumberFormatException;
import java.util.NoSuchElementException;    // for file.nextLine()

//package Week11;


public class Main {

    public static void main(String[] args) throws Exception {

        final int NUM_DIGITS = 10;               // Set to 10 for assignment, less if debugging, min=1
        int[] intArray = new int[NUM_DIGITS];   // Create an array to hold 10 integers
        int number = -100;                      // A default result
        boolean parseIntError;                  // Used for looping logic
        String filename = "C:\\Users\\Harold\\IdeaProjects\\CIT_260\\11.1 Avarage with File IO\\out\\data.txt";   // Default file name

        // 1. Tell the user what the program does.
        System.out.println("This program gets ten numbers from the user, and\n" +
                "computes and displays the average.\n");

        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        // 2a. Asks the user to enter in 10 integer values.
        System.out.println("Please enter ten integer values.");

        for (int i = 0; i < NUM_DIGITS; i++) {
            do {
                System.out.print("Enter integer " + i + ": ");

                // 2b. If a non-integer value is entered, your program should display an
                // error message and re-prompt the user to enter that value.
                parseIntError = false;
                try {
                    number = Integer.parseInt(keyboard.next());
                }
                catch (NumberFormatException nfEx) {
                    System.out.println("Error: input must be an integer.");
                    //nfEx.printStackTrace();
                    parseIntError = true;
                }

                // 3. If integer entered, then store the user's input in an array of integers.
                if (!parseIntError) {
                    intArray[i] = number;
                }
            } while (parseIntError);
        }


        // 4a. Opens a file named data.txt for writing.
        File file = new File(filename);

        // 4b. If the file does not exist, your program should create it.
        boolean fileCreated = false;
        try {
            fileCreated = file.createNewFile();
        }
        catch (IOException ioEx) {
            System.out.println("Error: File \"" + filename + "\" cannot be created.");
            //ioEx.printStackTrace();
            System.exit(1);
        }

        // DEBUG CODE
        if (false) {
            if (fileCreated) {
                System.out.println("File \"" + filename + "\" has been created.");
            } else {
                System.out.println("File \"" + filename + "\" already exists.");
            }
        }

        // 4c. If the file cannot be opened, an error message is displayed and the program terminates.
       try {
            Scanner fileToWrite = new Scanner(file);
        }
        catch (FileNotFoundException fnfEx) {
            System.out.println("Error: File \"" + filename + "\" cannot be opened for read.");
            System.exit(2);
        }

        // We must cLear all file contents with each run before writing to it again!
        PrintWriter pw = new PrintWriter(file);
        pw.print("");
        pw.close();

        // 5a. Writes the ten integer values entered by the user to the file.
        pw = null;
        try {
            FileWriter fw = new FileWriter(file, true);
            pw = new PrintWriter(fw);

            for (int i = 0; i < intArray.length; i++) {
                // 5b. Write each integer value on a separate line.
                pw.println(intArray[i]);
            }
        }
        catch (IOException ioEx) {
            System.out.println("Error: File \"" + filename + "\" cannot be opened for write.");
            System.exit(4);
        }
        finally {
            // 6. Closes the file after we are done writing to it.
            if (pw != null) {
                pw.close();
            }
        }


        // 7a. Opens the file for reading.
        Scanner fileToRead = null;
        try {
            fileToRead = new Scanner(file);
        }

        // 7b. If the file cannot be opened, an error message is displayed
        // and the program terminates.
        catch (FileNotFoundException fnfEx) {
            System.out.println("Error: File \"" + filename + "\" cannot be opened for read.");
            System.exit(5);
        }

        // 8a. Reads the data back in from the file and sums the values as the data is read in.
        double sum = 0;
        int numCount = 0;
        while (fileToRead.hasNextLine()) {
            try {
                sum += Integer.parseInt(fileToRead.nextLine());
            }

            // 8b. An error message is displayed if there is an error reading the data,
            // and the program is terminated.
            catch (NoSuchElementException nseEx) {
                System.out.println("Error: File \"" + filename + "\" cannot be read.");
                System.exit(6);
            }
            numCount++;
        }

        // 9. Computes and displays the average value of the numbers in the file.
        // Show the average with two digits after the decimal point.
        double average = sum / numCount;
        System.out.println("The average of the input values is " + String.format("%.2f", average));

        // 6. Closes the file after we are done reading it.
        fileToRead.close();

        // 10. Displays a goodbye message.
        System.out.println("Goodbye ...");
    }
}
