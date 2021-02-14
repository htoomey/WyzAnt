package Vigenere;

public class Main {

    public static void main(String[] args) {
        String plaintext = "THE_ART_OF_WAR_TEACHES_US_NOT_TO_RELY_ON_THE";
        String keyword = "ARTW";

        VigenereCipher vigenereCipher2 = new VigenereCipher(keyword);
        String encrypted = vigenereCipher2.encrypt(plaintext);
        String decrypted = vigenereCipher2.decrypt(encrypted);

        System.out.println("encrypted: " + encrypted);
        System.out.println("decrypted: " + decrypted);
    }
}
