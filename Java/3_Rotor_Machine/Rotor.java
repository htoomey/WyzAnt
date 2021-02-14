/*
 * NAME: Harold Toomey
 * DATE: 21 Sep 2020
 * FILE: Rotor.java
 * TASK: This Three-Rotor Machine encrypts/decrypts alphabetical text (no spaces), like the Enigma.
 * LANG: Java
 */

/*
                           Direction of Motion
              |                     |                  |
              V                     V                  V

       |-----------|        |-----------|        |----------|
 >>> A | 24>>>  21 |        | 26     20 |        |1       8 | A
     B | 25  v   3 |        | 1       1 |        |2   >>>18 | B >>>   EXAMPLE: A in, B out
     C | 26  v  15 |        | 2       6 |        |3   ^  26 | C
     D | 1   v   1 |        | 3       4 |        |4   ^  17 | D
     E | 2   v  19 |        | 4      15 |        |5   ^  20 | E
     F | 3   v  10 |        | 5       3 |        |6   ^  22 | F
     G | 4   v  14 |        | 6      14 |        |7   ^  10 | G
     H | 5   v  26 |        | 7      12 |        |8   ^   3 | H
     I | 6   v  20 |        | 8      23 |        |9   ^  13 | I
     J | 7   v  8  |        | 9       5 |        |10  ^  11 | J
     K | 8   v  16 |        | 10     16 |        |11  ^   4 | K
     L | 9   v   7 |        | 11      2 |        |12  ^  23 | L
     M | 10  v  22 |        | 12     22 |        |13  ^   5 | M
     N | 11  v   4 |        | 13     19 |        |14  ^  24 | N
     O | 12  v  11 |        | 14     11 |        |15  ^   9 | O
     P | 13  v   5 |        | 15     18 |        |16  ^  12 | P
     Q | 14  v  17 |        | 16     25 |        |17  ^  15 | Q
     R | 15  v   9 |        | 17  >>>24 | >>>>>> |18>>>	 16 | R
     S | 16  v  12 |        | 18  ^  13 |        |19     19 | S
     T | 17  v  23 |        | 19  ^   7 |        |20      6 | T
     U | 18  v  18 |        | 20  ^  10 |        |21     25 | U
     V | 19  v   2 |        | 21  ^   8 |        |22     21 | V
     W | 20  v  25 |        | 22  ^  21 |        |23      2 | W
     X | 21  v   6 |        | 23  ^   9 |        |24      7 | X
     Y | 22  >>>24 |>>>>>>> | 24>>>  26 |        |25      1 | Y
     Z | 23     13 |        | 25     17 |        |26     14 | Z
       |-----------|        |-----------|        |----------|
        Slow Rotor           Medium Rotor         Fast Rotor

                         (a) Initial Setting

Figure 2.8 Three-Rotor Machine With Wiring Represented by Numbered Contacts

------------------------------------------------------------------------------------
TEST CASE #1: PASS
    Input:     aaaaaaaaaaaaaaaaaaaaaaaaaa
    Encrypted: BETXDNVQOXKIFZYWMPUJHGBSCR
    Decrypted: AAAAAAAAAAAAAAAAAAAAAAAAAA
------------------------------------------------------------------------------------
TEST CASE #2: PASS
    Input:     abcdefghijklmnopqrstuvwxyz
    Encrypted: BQDKWQOUGCJFZRKLYMZURJYTET
    Decrypted: ABCDEFGHIJKLMNOPQRSTUVWXYZ
------------------------------------------------------------------------------------
TEST CASE #3: PASS
    Input:     abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxy  (no Z)
    Encrypted: BQDKWQOUGCJFZRKLYMZURJYTETGMYJRANFBRSJEKWZLGPBPKMSZ
    Decrypted: ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXY
------------------------------------------------------------------------------------
 */

package Three_Rotor;
import java.util.Scanner;

public class Rotor {

    final boolean DEBUG = false;            // Set to true to see 5 letter debug output.  Default to false.
    final int NUM_ENGLISH_CHARS = 26;       // Set to # of Rotor character positions.  A..Z = 26.
    final int NUM_DEBUG_CHARS = 5;          // Set to 5 Rotor character positions if DEBUG mode
    final int NUM_SYMBOLS = DEBUG ? NUM_DEBUG_CHARS : NUM_ENGLISH_CHARS;

    /*
       Rotor Data Structure:
             [][0] = LEFT or [ Left Enter 1-NUM_SYMBOLS ]
             [][1] = RIGHT or [ Right Exit 1-NUM_SYMBOLS ]
             [][2] = JUMP_E or [ Positive Jump from L to R for encryption ]
             [][3] = JUMP_D or [ Positive Jump from R to L for decryption ]
     */
    int[][] slowRotor   = new int[NUM_SYMBOLS + 1][4];    // ignore [0][*] or use for space char ' '
    int[][] mediumRotor = new int[NUM_SYMBOLS + 1][4];
    int[][] fastRotor   = new int[NUM_SYMBOLS + 1][4];

    // Used to encrypt.  Assigned in setRotorBeginPositions() Method.  Defaulted to Figure 2.8.
    int slowRotorBeginPosition   = 24;
    int mediumRotorBeginPosition = 26;
    int fastRotorBeginPosition   = 1;

    // Used to decrypt.  Assigned in setRotorEndPositions() Method.
    int slowRotorEndPosition   = slowRotorBeginPosition;
    int mediumRotorEndPosition = mediumRotorBeginPosition;
    int fastRotorEndPosition   = fastRotorBeginPosition;

    // Index between 1 and NUM_SYMBOLS indicating which Left number is at the top of the rotor
    int slowRotorTopIndex   = slowRotorBeginPosition;
    int mediumRotorTopIndex = mediumRotorBeginPosition;
    int fastRotorTopIndex   = fastRotorBeginPosition;

    String plainText  = "";
    String cypherText = "";


    // A no-arg constructor that creates a default object.
    Rotor() {
        setRotors();        // Load cyphers onto all three Rotors
        if (DEBUG) {
            setRotorBeginPositions(1, 1, 1);
        }
    }


    public void getPlainText() {
        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        System.out.print("\nPlease enter Plain text (letters only): ");
        plainText = keyboard.nextLine().toUpperCase();
        cypherText = "";
    }


    public void getCypherText() {
        // Make scanner class object that accepts input
        Scanner keyboard = new Scanner(System.in);

        System.out.print("\nPlease enter Cypher text (letters only): ");
        cypherText = keyboard.nextLine().toUpperCase();
        plainText = "";
    }

    public void printPlainText() {
        System.out.print("Plain text:  \"" + plainText + "\"\n");
    }

    public void printCypherText() {
        System.out.print("Cypher text: \"" + cypherText + "\"\n");
    }


    public void encryptString() {
        if (!DEBUG) {
            // Feed characters to encryper starting with the first character
            for (int i = 0; i < plainText.length(); i++) {
                cypherText = cypherText.concat(Character.toString(encryptCharacter(plainText.charAt(i))));
            }
        } else {
            // DEBUG CODE
            int slow, medium, fast;

            for (int i = 0; i < plainText.length(); i++) {
                slow = slowRotorTopIndex;
                medium = mediumRotorTopIndex;
                fast = fastRotorTopIndex;

                cypherText = cypherText.concat(Character.toString(encryptCharacter(plainText.charAt(i))));

                System.out.print(
                        "CHAR IN: '" + plainText.charAt(i) + "',  " +
                        "CHAR OUT: '" + cypherText.charAt(i) + "',  " +
                        "TOP IN SMF: [" + slow + ", " + medium + ", " + fast + "],  " +
                        "TOP OUT SMF: [" + slowRotorTopIndex + ", " + mediumRotorTopIndex + ", " + fastRotorTopIndex + "],  " +
                        "PLAIN TEXT: (" + plainText + "),  " +
                        "CYPHER TEXT: (" + cypherText + ")\n");
            }
        }
    }


    public void decryptString() {
        // Decrypting, so set top indexes to rotor end positions
        calculateRotorEndPositions();
        // getRotorEndPositions();     // DEBUG STATEMENT

        if (!DEBUG) {
            // Feed characters to decryper starting with the last character
            // The last char encrypted is now the first char decrypted.
            for (int i = cypherText.length() - 1; i >= 0; i--) {
                plainText = plainText.concat(Character.toString(decryptCharacter(cypherText.charAt(i))));
            }
        } else {
            // DEBUG CODE
            int slow, medium, fast;

            for (int i = cypherText.length() - 1; i >= 0; i--) {
                slow   = slowRotorTopIndex;
                medium = mediumRotorTopIndex;
                fast   = fastRotorTopIndex;

                plainText = plainText.concat(Character.toString(decryptCharacter(cypherText.charAt(i))));

                System.out.print(
                        "CHAR IN: '" + cypherText.charAt(i) + "',  " +
                        "CHAR OUT: '" + plainText.charAt(cypherText.length() - i - 1) + "',  " +
                        "TOP IN SMF: [" + slow + ", " + medium + ", " + fast + "],  " +
                        "TOP OUT SMF: [" + slowRotorTopIndex + ", " + mediumRotorTopIndex + ", " + fastRotorTopIndex + "],  " +
                        "CYPHER TEXT: (" + cypherText + "),  " +
                        "PLAIN TEXT: (" + plainText + ")\n");
            }
        }

        // Reverse plainText char order, from L to R, to R to L (e.g. last char first, first char last)
        plainText = reverseString(plainText);
    }


    // Reverses the characters in a string, making the last char be the first char, etc.
    // Used by decryptString() method.
    private String reverseString (String s) {
        StringBuilder sTemp = new StringBuilder();  // StringBuilder has a reverse function
        sTemp.append(s);                            // Append the string s into StringBuilder sTemp
        sTemp = sTemp.reverse();                    // Reverse StringBuilder sTemp
        return sTemp.toString();
    }


    // Make sure rotor indexes are valid, e.g. between 1 and NUM_SYMBOLS.
    // Used by encryptCharacter() and decryptCharacter() methods.
    private int normalizeIndex(int index) {
        if (index % NUM_SYMBOLS == 0) return (NUM_SYMBOLS); // CASES: 1) 26 - 26 = 0 so 26;  2) 26 - 0 = 26;  3) 26 + 26 = 52, so 26
        index = (index + 3*NUM_SYMBOLS) % NUM_SYMBOLS;        // Add NUM_SYMBOLS to avoid roll over NEGATIVE INDEX ERROR
        if (index == 0) index = 1;
        return (index);
    }


    // Determine how far the rotor is from 1 at the top.
    // Used by encryptCharacter() and decryptCharacter() methods.
    private int calculateOffset(int rotorTopIndex) {
        int offset;
        if (rotorTopIndex == 1) {
            offset = 0;
        } else {
            offset = NUM_SYMBOLS - rotorTopIndex + 1;
        }
        return (offset);
    }


    private char encryptCharacter(char ch) {
        char c = ' ';               // c is for the ciphertext character
        int charIndex = 0;          // 'A' = 1, 'B' = 2, 'C' = 3, etc.
        int enterRotorIndex = 0;    // Entry point relative to the TopIndex
        int exitRotorIndex = 0;     // Exit point relative to the TopIndex
        int offset = 0;             // How far the rotor has rolled downwards from 1 at the top (rows above the 1)

        // Verify valid character [a..zA..Z] input
        ch = Character.toUpperCase(ch);
        if ((ch < 'A') || (ch > 'Z')) {
            System.out.print("ERROR in encryptCharacter() Method: '" + ch + "' is not an alphabet character\n");
            return '?';
        }

        // Convert the character into a character number
        charIndex = (int) ch;                       // ASCII 'A' is DEC 65
        charIndex = charIndex - (int) 'A' + 1;      // 'A' = 1, 'B' = 2, 'C' = 3,  etc.

        // Pass through slow rotor L to R (ENTER = LEFT, EXIT = RIGHT)
        // TRANSFORMATION: Relative_to_Top_of_Rotor  >>  Relative_to_1  >>  Relative_to_Top_of_Rotor
        // NOTE: THESE THREE LINES OF CODE ARE THE HEART OF THIS PROGRAM
        offset = calculateOffset(slowRotorTopIndex);
        enterRotorIndex = normalizeIndex(slowRotorTopIndex + charIndex - 1);
        exitRotorIndex = normalizeIndex(enterRotorIndex + slowRotor[enterRotorIndex][2] + offset);

        // Pass through medium rotor L to R (ENTER = LEFT, EXIT = RIGHT)
        offset = calculateOffset(mediumRotorTopIndex);
        enterRotorIndex = normalizeIndex(exitRotorIndex - offset);
        exitRotorIndex = normalizeIndex(enterRotorIndex + mediumRotor[enterRotorIndex][2] + offset);

        // Pass through fast rotor L to R (ENTER = LEFT, EXIT = RIGHT)
        offset = calculateOffset(fastRotorTopIndex);
        enterRotorIndex = normalizeIndex(exitRotorIndex - offset);
        exitRotorIndex = normalizeIndex(enterRotorIndex + fastRotor[enterRotorIndex][2] + offset);

        // Convert character number back into a character
        c = (char) (exitRotorIndex + 'A' - 1);

        incrementRotors();      // Ready the rotors for the next character
        return (c);
    }


    private char decryptCharacter(char ch) {
        char p = ' ';               // p is for the plaintext character
        int charIndex = 0;          // 'A' = 1, 'B' = 2, 'C' = 3, etc.
        int enterRotorIndex = 0;    // Absolute entry point
        int exitRotorIndex = 0;     // Relative exit point
        int offset = 0;             // How far the rotor has rolled downwards from 1 at the top

        // Verify valid character [a..zA..Z] input
        ch = Character.toUpperCase(ch);
        if ((ch < 'A') || (ch > 'Z')) {
            System.out.print("ERROR in decryptCharacter() Method: '" + ch + "' is not an alphabet character\n");
            return '?';
        }

        // Convert the character into a character number
        charIndex = (int) ch;            // ASCII 'A' is DEC 65
        charIndex -= ((int) 'A' - 1);    // 'A' = 1, 'B' = 2, 'C' = 3,  etc.

        // Pass through fast rotor R to L (ENTER = RIGHT, EXIT = LEFT)
        // TRANSFORMATION: Relative_to_Top_of_Rotor  >>  Relative_to_1  >>  Relative_to_Top_of_Rotor
        offset = calculateOffset(fastRotorTopIndex);
        enterRotorIndex = normalizeIndex(fastRotorTopIndex + charIndex - 1);
        exitRotorIndex = normalizeIndex(enterRotorIndex + fastRotor[enterRotorIndex][3] + offset);

        // Pass through medium rotor R to L (ENTER = RIGHT, EXIT = LEFT)
        offset = calculateOffset(mediumRotorTopIndex);
        enterRotorIndex = normalizeIndex(exitRotorIndex - offset);
        exitRotorIndex = normalizeIndex(enterRotorIndex + mediumRotor[enterRotorIndex][3] + offset);

        // Pass through slow rotor R to L (ENTER = RIGHT, EXIT = LEFT)
        offset = calculateOffset(slowRotorTopIndex);
        enterRotorIndex = normalizeIndex(exitRotorIndex - offset);
        exitRotorIndex = normalizeIndex(enterRotorIndex + slowRotor[enterRotorIndex][3] + offset);

        // Convert character number back into a character
        p = (char) (exitRotorIndex + 'A' - 1);

        decrementRotors();
        return (p);
    }


    //
    // Getter and setter methods for the rotors
    //

    private void incrementRotors() {
        // boolean slowRollOver = false;
        boolean fastRollOver = false;
        boolean mediumRollOver = false;

        // Rotate fast rotor forward 1 click, checking for roll over
        fastRotorTopIndex--;    // Direction of rotation is down, so decrement
        if (fastRotorTopIndex == 0) {fastRotorTopIndex = NUM_SYMBOLS;}   // Makes a wheel ... 3 2 1 26 25 24 ...
        if (fastRotorTopIndex == fastRotorBeginPosition) {fastRollOver = true;}

        // If fast rotor made a full revolution, then rotate medium rotor forward 1 click, checking for roll over
        if (fastRollOver) {
            mediumRotorTopIndex--;    // Direction of rotation is down, so decrement
            if (mediumRotorTopIndex == 0) {mediumRotorTopIndex = NUM_SYMBOLS;}   // Makes a wheel ... 3 2 1 26 25 24 ...
            if (mediumRotorTopIndex == mediumRotorBeginPosition) {mediumRollOver = true;}
        }

        // If medium rotor made a full revolution, then rotate slow rotor forward 1 click
        if (mediumRollOver) {
            slowRotorTopIndex--;    // Direction of rotation is down, so decrement
            if (slowRotorTopIndex == 0) {slowRotorTopIndex = NUM_SYMBOLS;}   // Makes a wheel ... 3 2 1 26 25 24 ...
            // if (slowRotorTopIndex == slowRotorBeginPosition) {slowRollOver = true;}
        }
    }


    private void decrementRotors() {
        // boolean slowRollOver = false;
        boolean mediumRollOver = false;
        boolean fastRollOver = false;

        // Rotate fast rotor backwards by 1 click, checking for a full revolution
        fastRotorTopIndex++;    // Direction of rotation is up, so increment
        if (fastRotorTopIndex > NUM_SYMBOLS) {fastRotorTopIndex = 1;}   // Makes a wheel ... 24 25 26 1 2 3 ...
        if (fastRotorTopIndex == fastRotorBeginPosition + 1) {fastRollOver = true;}     // The +1 makes it work

        // If medium rotor rolls back to 1, then rotate slow rotor backwards 1 click, checking for roll over
        if (fastRollOver) {
            mediumRotorTopIndex++;    // Direction of rotation is up, so increment
            if (mediumRotorTopIndex > NUM_SYMBOLS) {mediumRotorTopIndex = 1;}   // Makes a wheel ... 24 25 26 1 2 3 ...
            if (mediumRotorTopIndex == mediumRotorBeginPosition + 1) {mediumRollOver = true;}
        }

        // If slow rotor rolls back to 1, then rotate medium rotor backwards 1 click, checking for roll over
        if (mediumRollOver) {
            slowRotorTopIndex++;    // Direction of rotation is up, so increment
            if (slowRotorTopIndex > NUM_SYMBOLS) {slowRotorTopIndex = 1;}   // Makes a wheel ... 24 25 26 1 2 3 ...
            // if (slowRotorTopIndex == slowRotorBeginPosition + 1) {slowRollOver = true;}
        }
    }


    public void setRotorBeginPositions (int slowRotorStart, int mediumRotorStart, int fastRotorStart) {
        // Verify rotor start positions are valid [1..NUM_SYMBOLS]
        if (    (slowRotorStart < 1)   || (slowRotorStart > NUM_SYMBOLS) ||
                (mediumRotorStart < 1) || (mediumRotorStart > NUM_SYMBOLS) ||
                (fastRotorStart < 1)   || (fastRotorStart > NUM_SYMBOLS)) {
            System.out.print("ERROR in setRotorBeginPositions() Method: At least one rotor start position is out of range\n");
            return;
        }

        // Prepare for encryption
        slowRotorTopIndex = slowRotorBeginPosition = slowRotorStart;
        mediumRotorTopIndex = mediumRotorBeginPosition = mediumRotorStart;
        fastRotorTopIndex = fastRotorBeginPosition = fastRotorStart;
    }


    public void getRotorBeginPositions () {
        System.out.print("Slow   Rotor Begin Position = " + slowRotorBeginPosition + "\n");
        System.out.print("Medium Rotor Begin Position = " + mediumRotorBeginPosition + "\n");
        System.out.print("Fast   Rotor Begin Position = " + fastRotorBeginPosition + "\n\n");
    }


    // Needed to begin decryption
    private void calculateRotorEndPositions () {
        setRotorBeginPositions(slowRotorBeginPosition, mediumRotorBeginPosition, fastRotorBeginPosition);
        for (int i = 1; i < cypherText.length(); i++) {incrementRotors();}
        slowRotorEndPosition   = slowRotorTopIndex;
        mediumRotorEndPosition = mediumRotorTopIndex;
        fastRotorEndPosition   = fastRotorTopIndex;
    }


    // OPTIONAL
    public void setRotorEndPositions (int slowRotorEnd, int mediumRotorEnd, int fastRotorEnd) {
        // Verify rotor start positions are valid [1..NUM_SYMBOLS]
        if (    (slowRotorEnd < 1)   || (slowRotorEnd > NUM_SYMBOLS) ||
                (mediumRotorEnd < 1) || (mediumRotorEnd > NUM_SYMBOLS) ||
                (fastRotorEnd < 1)   || (fastRotorEnd > NUM_SYMBOLS)) {
            System.out.print("ERROR in setRotorEndPositions() Method: At least one rotor end position is out of range\n");
            return;
        }

        // Prepare for decryption
        slowRotorTopIndex = slowRotorEndPosition = slowRotorEnd;
        mediumRotorTopIndex = mediumRotorEndPosition = mediumRotorEnd;
        fastRotorTopIndex = fastRotorEndPosition = fastRotorEnd;
    }


    public void getRotorEndPositions () {
        System.out.print("Slow   Rotor End Position = " + slowRotorEndPosition + "\n");
        System.out.print("Medium Rotor End Position = " + mediumRotorEndPosition + "\n");
        System.out.print("Fast   Rotor End Position = " + fastRotorEndPosition + "\n\n");
    }


    public void setRotors () {
        setSlowRotor();     // Set slow rotor key 1-1, 2-19, 3-10, ...
        setMediumRotor();   // Set medium rotor key 1-1, 2-6, 3-4, ...
        setFastRotor();     // Set fast rotor key 1-8, 2-18, 3-26, ...
    }


    private void setSlowRotor () {
        // Fill first column with sequential numbers from 1 - NUM_SYMBOLS
        for (int i = 0; i <= NUM_SYMBOLS; i++) {
            slowRotor[i][0] = i;
        }

        // Fill second column with this rotor's cypher numbers
        if (!DEBUG) {
            slowRotor[0][1] = 0;
            slowRotor[1][1] = 1;
            slowRotor[2][1] = 19;
            slowRotor[3][1] = 10;
            slowRotor[4][1] = 14;
            slowRotor[5][1] = 26;
            slowRotor[6][1] = 20;
            slowRotor[7][1] = 8;
            slowRotor[8][1] = 16;
            slowRotor[9][1] = 7;
            slowRotor[10][1] = 22;
            slowRotor[11][1] = 4;
            slowRotor[12][1] = 11;
            slowRotor[13][1] = 5;
            slowRotor[14][1] = 17;
            slowRotor[15][1] = 9;
            slowRotor[16][1] = 12;
            slowRotor[17][1] = 23;
            slowRotor[18][1] = 18;
            slowRotor[19][1] = 2;
            slowRotor[20][1] = 25;
            slowRotor[21][1] = 6;
            slowRotor[22][1] = 24;
            slowRotor[23][1] = 13;
            slowRotor[24][1] = 21;
            slowRotor[25][1] = 3;
            slowRotor[26][1] = 15;
        } else {
            // DEBUG CODE
            slowRotor[0][1] = 0;
            slowRotor[1][1] = 4;
            slowRotor[2][1] = 1;
            slowRotor[3][1] = 5;
            slowRotor[4][1] = 2;
            slowRotor[5][1] = 3;
        }
        // Crypto Key Checksum to identify missing, invalid, or double entries (but not swapped entries)
        int checksum = 0;
        for (int i = 1; i <= NUM_SYMBOLS; i++) {checksum += slowRotor[i][1];}
        //  Add 1 + 2 + 3 + ... + 25 + 26, n = 26, SUM = (n*(n+1))/2 = 351
        if (checksum != ((NUM_SYMBOLS * (NUM_SYMBOLS + 1)) / 2)) {
            System.out.print("ERROR in setSlowRotor() Method: Bad Checksum. Possible duplicate wiring.\n\n");
        }

        // Fill third column with positive jump values for wiring configuration for encryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (slowRotor[i][0] == slowRotor[j][1]) {
                    slowRotor[i][2] = j - i;
                    if (slowRotor[i][2] < 1) {slowRotor[i][2] += NUM_SYMBOLS;}
                    if (slowRotor[i][2] == NUM_SYMBOLS) {slowRotor[i][2] = 0;}
                    break;
                }
            }
        }

        // Fill forth column with positive jump values for wiring configuration for decryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (slowRotor[i][1] == slowRotor[j][0]) {
                    slowRotor[i][3] = j - i;
                    if (slowRotor[i][3] < 1) {slowRotor[i][3] += NUM_SYMBOLS;}
                    if (slowRotor[i][3] == NUM_SYMBOLS) {slowRotor[i][3] = 0;}
                    break;
                }
            }
        }
    }



    private void setMediumRotor () {
        // Fill first column with sequential numbers from 1 - NUM_SYMBOLS
        for (int i = 0; i <= NUM_SYMBOLS; i++) {
            mediumRotor[i][0] = i;
        }

        // Fill second column with this rotor's cypher numbers
        if (!DEBUG) {
            mediumRotor[0][1] = 0;
            mediumRotor[1][1] = 1;
            mediumRotor[2][1] = 6;
            mediumRotor[3][1] = 4;
            mediumRotor[4][1] = 15;
            mediumRotor[5][1] = 3;
            mediumRotor[6][1] = 14;
            mediumRotor[7][1] = 12;
            mediumRotor[8][1] = 23;
            mediumRotor[9][1] = 5;
            mediumRotor[10][1] = 16;
            mediumRotor[11][1] = 2;
            mediumRotor[12][1] = 22;
            mediumRotor[13][1] = 19;
            mediumRotor[14][1] = 11;
            mediumRotor[15][1] = 18;
            mediumRotor[16][1] = 25;
            mediumRotor[17][1] = 24;
            mediumRotor[18][1] = 13;
            mediumRotor[19][1] = 7;
            mediumRotor[20][1] = 10;
            mediumRotor[21][1] = 8;
            mediumRotor[22][1] = 21;
            mediumRotor[23][1] = 9;
            mediumRotor[24][1] = 26;
            mediumRotor[25][1] = 17;
            mediumRotor[26][1] = 20;
        } else {
            // DEBUG CODE
            mediumRotor[0][1] = 0;
            mediumRotor[1][1] = 4;
            mediumRotor[2][1] = 1;
            mediumRotor[3][1] = 5;
            mediumRotor[4][1] = 2;
            mediumRotor[5][1] = 3;

        }

        // Crypto Key Checksum to identify missing, invalid, or double entries (but not swapped entries)
        int checksum = 0;
        for (int i = 1; i <= NUM_SYMBOLS; i++) {checksum += mediumRotor[i][1];}
        //  Add 1 + 2 + 3 + ... + 25 + 26, n = 26, SUM = (n*(n+1))/2 = 351
        if (checksum != ((NUM_SYMBOLS * (NUM_SYMBOLS + 1)) / 2)) {
            System.out.print("ERROR in setMediumRotor() Method: Bad Checksum. Possible duplicate wiring.\n\n");
        }

        // Fill third column with positive jump values for wiring configuration for encryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (mediumRotor[i][0] == mediumRotor[j][1]) {
                    mediumRotor[i][2] = j - i;
                    if (mediumRotor[i][2] < 1) {mediumRotor[i][2] += NUM_SYMBOLS;}
                    if (mediumRotor[i][2] == NUM_SYMBOLS) {mediumRotor[i][2] = 0;}
                    break;
                }
            }
        }

        // Fill forth column with positive jump values for wiring configuration for decryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (mediumRotor[i][1] == mediumRotor[j][0]) {
                    mediumRotor[i][3] = j - i;
                    if (mediumRotor[i][3] < 1) {mediumRotor[i][3] += NUM_SYMBOLS;}
                    if (mediumRotor[i][3] == NUM_SYMBOLS) {mediumRotor[i][3] = 0;}
                    break;
                }
            }
        }
    }


    private void setFastRotor () {
        // Fill first column with sequential numbers from 1 - NUM_SYMBOLS
        for (int i = 0; i <= NUM_SYMBOLS; i++) {
            fastRotor[i][0] = i;
        }

        // Fill second column with this rotor's cypher numbers
        if (!DEBUG) {
            fastRotor[0][1] = 0;
            fastRotor[1][1] = 8;
            fastRotor[2][1] = 18;
            fastRotor[3][1] = 26;
            fastRotor[4][1] = 17;
            fastRotor[5][1] = 20;
            fastRotor[6][1] = 22;
            fastRotor[7][1] = 10;
            fastRotor[8][1] = 3;
            fastRotor[9][1] = 13;
            fastRotor[10][1] = 11;
            fastRotor[11][1] = 4;
            fastRotor[12][1] = 23;
            fastRotor[13][1] = 5;
            fastRotor[14][1] = 24;
            fastRotor[15][1] = 9;
            fastRotor[16][1] = 12;
            fastRotor[17][1] = 25;
            fastRotor[18][1] = 16;
            fastRotor[19][1] = 19;
            fastRotor[20][1] = 6;
            fastRotor[21][1] = 15;
            fastRotor[22][1] = 21;
            fastRotor[23][1] = 2;
            fastRotor[24][1] = 7;
            fastRotor[25][1] = 1;
            fastRotor[26][1] = 14;
        } else {
            // DEBUG CODE
            fastRotor[0][1] = 0;
            fastRotor[1][1] = 4;
            fastRotor[2][1] = 1;
            fastRotor[3][1] = 5;
            fastRotor[4][1] = 2;
            fastRotor[5][1] = 3;

        }

        // Crypto Key Checksum to identify missing, invalid, or double entries (but not swapped entries)
        int checksum = 0;
        for (int i = 1; i <= NUM_SYMBOLS; i++) {checksum += fastRotor[i][1];}
        //  Add 1 + 2 + 3 + ... + 25 + 26, n = 26, SUM = (n*(n+1))/2 = 351
        if (checksum != ((NUM_SYMBOLS * (NUM_SYMBOLS + 1)) / 2)) {
            System.out.print("ERROR in setFastRotor() Method: Bad Checksum. Possible duplicate wiring.\n\n");
        }

        // Fill third column with positive jump values for wiring configuration for encryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (fastRotor[i][0] == fastRotor[j][1]) {
                    fastRotor[i][2] = j - i;
                    if (fastRotor[i][2] < 1) {fastRotor[i][2] += NUM_SYMBOLS;}
                    if (fastRotor[i][2] == NUM_SYMBOLS) {fastRotor[i][2] = 0;}
                    break;
                }
            }
        }

        // Fill forth column with positive jump values for wiring configuration for decryption
        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            for (int j = 1; j <= NUM_SYMBOLS; j++) {
                if (fastRotor[i][1] == fastRotor[j][0]) {
                    fastRotor[i][3] = j - i;
                    if (fastRotor[i][3] < 1) {fastRotor[i][3] += NUM_SYMBOLS;}
                    if (fastRotor[i][3] == NUM_SYMBOLS) {fastRotor[i][3] = 0;}
                    break;
                }
            }
        }
    }


    public void printRotors () {
        int slowIndex   = slowRotorTopIndex;
        int mediumIndex = mediumRotorTopIndex;
        int fastIndex   = fastRotorTopIndex;

        // Verify topIndex is valid [1..NUM_SYMBOLS]
        if ((slowIndex < 1)   || (slowIndex   > NUM_SYMBOLS) ||
                (mediumIndex < 1) || (mediumIndex > NUM_SYMBOLS) ||
                (fastIndex < 1)   || (fastIndex   > NUM_SYMBOLS)) {
            System.out.print("ERROR in printRotors() Method: index out of range\n");
        }

        System.out.print("\n Slow Rotor\t\t Medium Rotor\t Fast Rotor");
        // System.out.print("\nLEFT\tRIGHT\tLEFT\tRIGHT\tLEFT\tRIGHT");
        System.out.print("\n|-----------|\t|-----------|\t|-----------|\n");

        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            // Compute index.  Skip over [0], roll over [NUM_SYMBOLS] >> [1]
            if ((slowIndex <= 0) || (slowIndex > NUM_SYMBOLS)) {
                slowIndex = 1;
            }
            if ((mediumIndex <= 0) || (mediumIndex > NUM_SYMBOLS)) {
                mediumIndex = 1;
            }
            if ((fastIndex <= 0) || (fastIndex > NUM_SYMBOLS)) {
                fastIndex = 1;
            }

            // Print out 1 row at a time
            System.out.print("|"
                    + slowRotor[slowIndex][0]     + "\t\t"
                    + slowRotor[slowIndex][1]     + "\t|\t");
            System.out.print("|"
                    + mediumRotor[mediumIndex][0] + "\t\t"
                    + mediumRotor[mediumIndex][1] + "\t|\t");
            System.out.print("|"
                    + fastRotor[fastIndex][0]     + "\t\t"
                    + fastRotor[fastIndex][1]     + "\t|\n");

            slowIndex++;
            mediumIndex++;
            fastIndex++;
        }
        System.out.print("|-----------|\t|-----------|\t|-----------|\n");
    }


    // OPTIONAL DEBUG METHOD
    public void printRotorsWithJumps () {
        int slowIndex   = slowRotorTopIndex;
        int mediumIndex = mediumRotorTopIndex;
        int fastIndex   = fastRotorTopIndex;

        // Verify topIndex is valid [1..NUM_SYMBOLS]
        if ((slowIndex < 1)       || (slowIndex   > NUM_SYMBOLS) ||
                (mediumIndex < 1) || (mediumIndex > NUM_SYMBOLS) ||
                (fastIndex < 1)   || (fastIndex   > NUM_SYMBOLS)) {
            System.out.print("ERROR in printRotorsWithDeltas() Method: index out of range\n");
        }

        System.out.print("\n Slow Rotor\t\t\t\t\t\t\t Medium Rotor\t\t\t\t\t\t Fast Rotor\n");
        System.out.print("\nLEFT\tRIGHT\tJUMP_E\tJUMP_D\t\tLEFT\tRIGHT\tJUMP_E\tJUMP_D\t\tLEFT\tRIGHT\tJUMP_E\tJUMP_D");
        System.out.print("\n|---------------------------|\t\t|---------------------------|\t\t|---------------------------|\n");

        for (int i = 1; i <= NUM_SYMBOLS; i++) {
            // Compute index.  Skip over [0], roll over [NUM_SYMBOLS + 1] >> [1]
            if ((slowIndex <= 0) || (slowIndex > NUM_SYMBOLS)) {
                slowIndex = 1;
            }
            if ((mediumIndex <= 0) || (mediumIndex > NUM_SYMBOLS)) {
                mediumIndex = 1;
            }
            if ((fastIndex <= 0) || (fastIndex > NUM_SYMBOLS)) {
                fastIndex = 1;
            }

            // Print out 1 row at a time
            System.out.print("|"
                    + slowRotor[slowIndex][0]     + "\t\t"
                    + slowRotor[slowIndex][1]     + "\t\t"
                    + slowRotor[slowIndex][2]     + "\t\t"
                    + slowRotor[slowIndex][3]     + "\t|\t\t");
            System.out.print("|"
                    + mediumRotor[mediumIndex][0] + "\t\t"
                    + mediumRotor[mediumIndex][1] + "\t\t"
                    + mediumRotor[mediumIndex][2] + "\t\t"
                    + mediumRotor[mediumIndex][3] + "\t|\t\t");
            System.out.print("|"
                    + fastRotor[fastIndex][0]     + "\t\t"
                    + fastRotor[fastIndex][1]     + "\t\t"
                    + fastRotor[fastIndex][2]     + "\t\t"
                    + fastRotor[fastIndex][3]     + "\t|\n");

            slowIndex++;
            mediumIndex++;
            fastIndex++;
        }
        System.out.print("|---------------------------|\t\t|---------------------------|\t\t|---------------------------|\n");
    }

}