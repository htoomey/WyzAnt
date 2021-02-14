package Vigenere;

public class VigenereCipher {

    private String keyPhrase;

    // Constructor
    public VigenereCipher(String keyword) {
        // Creates keyPhrase to be used for encryption and decryption
        // based on the given keyword
        if (keyword == null) {
            this.keyPhrase = "";
            return;
        }
        char[] allChars = keyword.toUpperCase().toCharArray();
        StringBuilder alphaBuf = new StringBuilder(allChars.length);
        for (char c : allChars) {
            // Append only alphabets in the range: [A-Z]
            if (Character.isLetter(c)) {
                alphaBuf.append(c);
            }
        }
        this.keyPhrase = alphaBuf.toString();
    }

    //Encrypt given plain text using the keyword, and return cipher text.
    public String encrypt(String plainText) {
        // Check if null or is of length zero.
        if ((plainText == null) || (keyPhrase.length() == 0)){
            return "";
        }

//		char[] allChars = plainText.toLowerCase().toCharArray();
        char[] allChars = plainText.toCharArray();

        // Convert all spaces "_" to '[' before the encryption
        for(int i  = 0 ; i < allChars.length; i++) {
            if(allChars[i] == '_') {
                allChars[i] = '[';
            }
        }

        // Construct a string with the repeated key at least the size of our message
        String matchingLengthKey = keyPhrase;
        while (matchingLengthKey.length() < plainText.length()) {
            matchingLengthKey += keyPhrase;
        }

        System.out.println(matchingLengthKey + "\n"); // DEBUG

        for (int i = 0; i < allChars.length; i++) {
            char offset = matchingLengthKey.charAt(i);
            int shiftNumber = offset - 'A';
            allChars[i] = Character.toUpperCase(allChars[i]);
            allChars[i] += shiftNumber;
            //System.out.print(allChars[i]); // DEBUG

            // Subtract out of range index values.  Range = [1..27] = [A..Z_}
            if (allChars[i] > '[') {
                allChars[i] -= '[';
                allChars[i] += ('A' - 1);   // Convert char index to char, 1 = 'A'
            }
        }

        // Convert all '[' to spaces "_" after the encryption
        for(int i = 0 ; i < allChars.length; i++) {
            if(allChars[i] == '[') {
                allChars[i] = '_';
            }
        }

        return new String(allChars);
    }

    //Decrypt given encrypted text using the keyword, and return plain text.
    public String decrypt(String cipher) {
        // Check if null or is of length zero.
        if ((cipher == null) || (keyPhrase.length() == 0)){
            return "";
        }

        char[] allChars = cipher.toUpperCase().toCharArray();
        String matchingLengthKey = keyPhrase;
        while (matchingLengthKey.length() < cipher.length()) {
            matchingLengthKey += keyPhrase;
        }

        // Convert all spaces "_" to '[' before the encryption
        for(int i = 0 ; i <allChars.length; i++) {
            if(allChars[i] == '_') {
                allChars[i] = '[';
            }
        }

        for (int i = 0; i < allChars.length; i++) {
            // Check if the char is in the range: [A-Z]
                        /*			if (allChars[i] < 'A' || allChars[i] > '[') {
                        //Ignore characters which are NOT in the range [A-Z]
                        continue;
                        }*/

            char offset = matchingLengthKey.charAt(i);
            int shiftNumber = offset - 'A';
            //allChars[i] = Character.toUpperCase(allChars[i]);
            allChars[i] -= shiftNumber;
            if (allChars[i] < 'A') {
                allChars[i] += '[';
                allChars[i] -= ('A' - 1);
            }
        }

        // Convert all '[' to spaces "_" after the decryption
        for(int i = 0 ; i <allChars.length; i++) {
            if (allChars[i] == '[') {
                allChars[i] = '_';
            }
        }

        return new String(allChars);
    }

}