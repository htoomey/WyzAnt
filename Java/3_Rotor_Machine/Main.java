/*
 * NAME: Harold Toomey
 * DATE: 21 Sep 2020
 * FILE: Main.java
 * TASK: This Three-Rotor Machine encrypts/decrypts alphabetical text (no spaces), like the Enigma.
 * LANG: Java
 */

package Three_Rotor;

public class Main {

    public static void main(String[] args) {
        // Tells the user what the program does.
        System.out.print("This Three-Rotor Machine encrypts/decrypts alphabetical text (no spaces).\n");
        Rotor r = new Rotor();

        // Encrypt
        //r.setRotorBeginPositions(24, 26, 1);    // OPTIONAL: Default is slow = 24, medium = 26, fast = 1
        r.printRotors();    // Print out all rotors to show their encryption configuration
        r.getPlainText();   // Prompt for plain text to encrypt
        r.printPlainText();
        r.encryptString();
        r.printCypherText();

        // Decrypt
        // r.setRotorEndPositions(24, 26, 1);      // OPTIONAL: Default is where it ended after encrypting the text, less one turn
        r.getCypherText();  // Prompt for cypher text to decrypt
        r.printCypherText();
        r.decryptString();
        r.printPlainText();

        // Outputs a goodbye message.
        System.out.print("\nGoodbye\n\n");
    }

}
