import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * This is your Main program. This File will be run all operations, based on the input different function will be called.
 *
 * Complete the given and add necessary functions as per your requirement.
 */

public class Main {
    /**
     * This method reads in a list of words and sorts them in rhyming order based on suffix,
     * @param in file input stream
     * @param out file output stream
     */
    public static void RhymeOrder(Scanner in, PrintWriter out) {
        in.nextLine();                                      //skips over the second line (problem number)
        int numLines = Integer.parseInt(in.nextLine());     //reads in third line (number of words/lines to follow)
        String[] wordArr = new String[numLines];            //String array for the words that are read in

        for (int i = 0; i < numLines; i++) {
            wordArr[i] = reverseWord(in.nextLine());        //reads in each word (one per line), and reverses it when it's put in the array.
        }

        Rhymer rhymeOrder = new Rhymer(wordArr);

        int start = 0;
        int end = rhymeOrder.words.length;
        rhymeOrder.quickSort(rhymeOrder.words, start, end);

        for (String s : rhymeOrder.words) {                 //prints out the sorted words (reverses them on the spot since they were reversed for sorting)
            out.println(reverseWord(s));
            out.flush();
        }

    } //RhymeOrder

    /**
     * This method reads in a list of words and finds the ones that share suffixes. prints a list of the suffixes that have at least k words.
     * @param in file input stream
     * @param out file output stream
     * @param k the lowest number of words a suffix can have to be printed
     */
    public static void SuffixShare(Scanner in, PrintWriter out, int k) {
        in.nextLine();                                                  //skips over the second line (problem number)
        int numLines = Integer.parseInt(in.nextLine());                 //reads in the third line (number of words/lines to follow)
        String[] wordArr = new String[numLines];
        int longestIndex = 0;

        for (int i = 0; i < numLines; i++) {                            //reads in each word reversed (to work for rhyming quickSort)
            wordArr[i] = in.nextLine();
            if (wordArr[i].length() > wordArr[longestIndex].length()) { //simply here to find the longest word in the array without adding another loop (and subsequently, a higher runtime)
                longestIndex = i;
            }
        }

        Rhymer rhymeOrder = new Rhymer(wordArr);

        int start = 0;
        int end = rhymeOrder.words.length;
        rhymeOrder.quickSort(rhymeOrder.words, start, end);             //sorts all words by rhyming

        HashMap<String, ArrayList<String>> hashTable = rhymeOrder.suffixShare(rhymeOrder.words, longestIndex);

        //quickSort keys so they print in alphabetical order
        String[] keyArr = (hashTable.keySet().toArray(new String[0]));
        Rhymer keySort = new Rhymer(keyArr);
        keySort.quickSort(keyArr, start, keySort.words.length);

        //prints all values to output file
        for (String key: keyArr) {                                      //iterates through all keys in the hashTable
            if (hashTable.get(key).size() >= k) {                       //only runs if the number of collisions is greater than or equal to 'k' (the required number of collisions)
                out.print(key + " -> [");
                for (int i = 0; i < hashTable.get(key).size(); i++) {   //iterates through each ArrayList at each index in the hashTable
                    if (hashTable.get(key).get(i) != null) {            //avoids null pointer exceptions
                        if (i == hashTable.get(key).size() - 1) {       //formatting - avoids extra space at the end of the list of words
                            out.print(hashTable.get(key).get(i));
                        }
                        else {
                            out.print(hashTable.get(key).get(i) + ", ");
                        }
                    }
                }
                out.println("]");
            }
        }
    } //SuffixShare()

    /**
     * This method reads in binary tree commands and executes the desired commands.
     * @param in file input stream
     * @param out file output stream
     */
    public static void BinarySearchTree(Scanner in, PrintWriter out) {
        BinarySearchTree bt = new BinarySearchTree();

        in.nextLine();                                              //skips over first line (problem number)
        int numLines = Integer.parseInt(in.nextLine());             //reads the second line (number of lines left to read)
        String[] nextLine;

        for (int i = 0; i < numLines; i++) {
            nextLine = in.nextLine().split(" ");
            switch (nextLine[0]) {
                case "insert":
                    bt.insertKey(Integer.parseInt(nextLine[1]));        //nothing printed out to file
                    break;
                case "height":
                    out.println(bt.height(Integer.parseInt(nextLine[1])));  //prints height to file, "none" if there
                    break;
                case "delete":
                    out.println(bt.deleteKey(Integer.parseInt(nextLine[1])));
                    break;
                case "search":
                    if (bt.searchKey(Integer.parseInt(nextLine[1])))
                        out.println("found");
                    else
                        out.println("not found");
                    break;
                case "range":
                    if (bt.rangeSum(Integer.parseInt(nextLine[1]), Integer.parseInt(nextLine[2])) == 0)
                        out.println("none");
                    else
                        out.println(bt.rangeSum(Integer.parseInt(nextLine[1]), Integer.parseInt(nextLine[2])));
                    break;
                case "postorder":
                    out.println(bt.postorder());
                    break;
                case "levelorder":
                    out.println(bt.levelorder());
                    break;
                case "lca":
                    if (bt.LCA(Integer.parseInt(nextLine[1]), Integer.parseInt(nextLine[2])) == Integer.MIN_VALUE)
                        out.println("none");
                    else
                        out.println(bt.LCA(Integer.parseInt(nextLine[1]), Integer.parseInt(nextLine[2])));
                    break;
                case "ceil":
                    out.println(bt.Ceil(Integer.parseInt(nextLine[1])));
                    break;
                case "floor":
                    out.println(bt.Floor(Integer.parseInt(nextLine[1])));
                    break;
                case "dist":
                    out.println(bt.dist(Integer.parseInt(nextLine[1]), Integer.parseInt(nextLine[2])));
                    break;
                case "insertRB":
                    bt.insertRB(Integer.parseInt(nextLine[1]));
                    break;
                case "Bheight":
                    out.println(bt.getBlackHeight(Integer.parseInt(nextLine[1])));
                    break;
                default:            //error!
                    System.out.println("Error! Input not valid!");
                    break;
            }
        }
    } //BinarySearchTree()

    /**
     * This method calls the correct method for each part number.
     * @param args command line arguments (should be input and output file paths)
     */
    public static void main(String[] args) {
        if(args.length!=2){
            System.out.println("ERROR: Not enough Parameters");
            System.exit(0);
        }
        Scanner in=null;
        PrintWriter out=null;
        try {
            in = new Scanner(new File(args[0]));
            out= new PrintWriter(new File(args[1]));
        }
        catch (Exception e){
            e.printStackTrace();
            System.exit(0);
        }

        int PartNo=in.nextInt();

        if(PartNo==1){
            //Part 1: Word Sorting
            int OperationNo=in.nextInt();
            if(OperationNo==1){
                //Operation: Rhyme Word Sorting Order
                RhymeOrder(in,out);
            }
            else if (OperationNo==2){
                //Operation: Rhyme Word Sorting Order
                int k=in.nextInt();
                
                SuffixShare(in,out, k);
            }
            else{
                System.out.println("Invalid Input");
                System.exit(0);
            }
            
        }
       
        else if(PartNo==2){
            BinarySearchTree(in, out);
        }
        
        else{
            System.out.println("Invalid Input");
            System.exit(0);
        }

        in.close();
        out.close();
    } //main()

    /**
     * This method reverses the passed in word.
     * @param word word to be reversed
     * @return String representation of the reversed word
     */
    public static String reverseWord(String word) {
        StringBuilder stringBuilder = new StringBuilder(word);
        return stringBuilder.reverse().toString();
    } //reverseWord()
}