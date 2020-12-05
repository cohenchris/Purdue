import java.util.*;
public class Caesar
{
    public static void main(String[] args)
    {
        Scanner s = new Scanner(System.in);
        System.out.println("Welcome to the Caesar cipher encryption/decryption service.");
        String encrypt, decrypt, encrypted, decrypted;
        Random rand = new Random();
        int choice, keyI, keyD;
        char x,y;
        do
        {
            encrypt = "";
            decrypt = "";
            encrypted = "";
            decrypted = "";
            System.out.println("Select what service you would like below");
            System.out.println("1 - Encrypt a message\n2 - Decrypt a message\n3 - Exit");
            keyI = rand.nextInt(25)+1;
            choice = s.nextInt();
            if(choice == 1)
            {
                System.out.println("Enter the message you would like to encrypt:");
                s.nextLine();
                encrypt = s.nextLine();
                encrypt = encrypt.toLowerCase();
                for(int i = 0;i<encrypt.length();i++)
                {
                    if(encrypt.charAt(i)<97 || encrypt.charAt(i)>122)
                    {
                        encrypted+=encrypt.charAt(i);
                        continue;
                    }
                    x=(char)(encrypt.charAt(i)+keyI);
                    if((int)x>122)
                        x-=26;
                    encrypted+=x;
                }
                System.out.println("Encrypted message is: "+encrypted);
            }
            else if(choice == 2)
            {
                System.out.println("Enter the message you would like to decrypt:");
                s.nextLine();
                decrypt = s.nextLine();
                decrypt = decrypt.toLowerCase();
                System.out.println("Please enter your guess for a key:");
                keyD = s.nextInt();
                while(keyD > 25 || keyD < 1)
                {
                    System.out.println("That's not a valid key.\nPlease enter a new guess between 1 and 25 (inclusive):");
                    keyD = s.nextInt();
                }
                for(int j = 0;j<decrypt.length();j++)
                {
                    if(decrypt.charAt(j)<97 || decrypt.charAt(j)>122)
                    {
                        decrypted+=decrypt.charAt(j);
                        continue;
                    }
                    y=(char)(decrypt.charAt(j)-keyD);
                    if((int)y<97)
                        y+=26;
                    decrypted+=y;
                }
                System.out.println("The decrypted message with this key is: "+decrypted);
            }
            else
                System.exit(0);
        }while(choice != 3);

    }

}
