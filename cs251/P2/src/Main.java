import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;

/**
 * This is your Main program. This File will be run to test all cases, based on the input different function will be called.
 *
 * Complete the given and add necessary functions as  per your requirement.
 */


public class Main {
    public static void StackChecking(Scanner in, PrintWriter out){
        in.nextLine();      //This is here to make sure the program passes the first line, which denotes the problem number.
        int numLines = Integer.parseInt(in.nextLine());
        Stack stack = new Stack();

        // This for loop iterates numLines times. It reads each line in, then reads the first letter to find out which operation to do.
        for (int i = 0; i < numLines; i++) {
            String s = in.nextLine();
            String currentLine[] = s.split(" ");
            if (currentLine[0].equals("i")) {   //If the first letter is an "i", then the program should insert (push) the following point onto the stack.
                stack.push(Integer.parseInt(currentLine[1]), Integer.parseInt(currentLine[2]));
            }
            else if (currentLine[0].equals("p")) {  //If the first letter is a "p", then the program  should pop the first point on the stack.
                out.write(stack.pop());
                out.flush();

                if (i != numLines - 1) {     //If it's on any line BUT the last line, write a newline character
                    out.write("\n");
                    out.flush();
                }
            }
        }
    } //StackChecking()

    private static Stack wordSearchStack = new Stack();
    private static String word;
    private static int numLines = -1;
    private static int numColumns = -1;
    private static int wordIndex;
    private static String[][] puzzle;
    private static boolean wordFound = false;

    public static void WordSearch(Scanner in, PrintWriter out){

        in.nextLine();      //This is here to make sure the program passes the first line, which denotes the problem number.
        String sizeLine = in.nextLine();
        String[] sizeArr = sizeLine.split(" ");

        numLines = Integer.parseInt(sizeArr[0]);
        numColumns = Integer.parseInt(sizeArr[1]);
        puzzle = new String[numLines][numColumns];

        for (int i = 0; i < numLines; i++) {        //These nested loops iterate through the puzzle based on the numLines and numColumns, and essentially copy the puzzle over to the 2D array.
            for (int j = 0; j < numColumns; j++) {
                puzzle[i][j] = in.next();
            }
        }

        in.nextLine();            //This line is required for the program to correctly read in the word.
        word = in.nextLine();     //This line reads in the word that we're searching for in the puzzle.

        solveWordSearch(0,0);

        if (wordSearchStack.isEmpty()) {        //if solveWordSearch() comes back with an empty stack, the word is not found
            out.print("not found");
            out.flush();
        }

        else {
            String[] coordinates = new String[word.length()];
            for (int i = word.length() - 1; i >= 0; i--) {      //loops through the stack, making sure to fill the array BACKWARDS since the stack has them stored backwards.
                if (i == word.length() - 1)                                     //makes sure to not put a newline character when finishing the file
                    coordinates[i] = wordSearchStack.pop();
                else
                    coordinates[i] = wordSearchStack.pop() + "\n";
            }

            for (int i = 0; i < word.length(); i++) {           //prints all coordinates to the file in order
                out.print(coordinates[i]);
                out.flush();
            }
        }
    } //WordSearch()

    public static void solveWordSearch(int x, int y) {
        int currentX = x;
        int currentY = y;

        if (wordFound) { return; }

        if (wordIndex == word.length()) {       //returns if the word has been completely found
            wordFound = true;
            return;
        }

        if (wordSearchStack.isEmpty()) {        //only executes if the stack is empty (looking for the first letter)
            for (int i = 0; i < numLines; i++) {
                for (int j = 0; j < numColumns; j++) {
                    if ((wordIndex + 1 <= word.length()) && (puzzle[i][j].equals(word.substring(wordIndex, wordIndex + 1)))) {
                        wordSearchStack.push(i, j);
                        currentX = i;
                        currentY = j;
                        wordIndex++;
                        solveWordSearch(currentX, currentY);
                    }
                }
            }
        }

        /* NOTES ABOUT THE FOLLOWING SECTION:
           - currentX, currentY, and index are not directly changed in the scope in order to preserve their values, just in case we need to go back and continue
           the search from where it was left off

           - each adjacent space is checked in a circular fashion around the original space, starting from the bottom left and going clockwise.
           The only exception is when the bounds are exceeded.
         */

        if (wordFound) { return; }


        if (((currentX - 1 < numLines) && (currentX - 1 > 0)) && (currentY + 1 < numColumns)) {         //checks bottom left adjacent space
            if (puzzle[currentX - 1][currentY + 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX - 1, currentY + 1);
                wordIndex++;                solveWordSearch(currentX - 1, currentY + 1);
            }
        }

        if (wordFound) { return; }

        if ((currentX - 1 < numColumns) && (currentX - 1 > 0)) {                                        //checks middle left adjacent space
            if (puzzle[currentX - 1][currentY].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX - 1, currentY);
                wordIndex++;
                solveWordSearch(currentX - 1, currentY);
            }
        }

        if (wordFound) { return; }

        if (((currentX - 1 < numColumns) && (currentX - 1 > 0)) && ((currentY - 1 < numLines) && (currentY - 1 > 0))) {         //checks top left adjacent space
            if (puzzle[currentX - 1][currentY - 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX - 1, currentY - 1);
                wordIndex++;
                solveWordSearch(currentX - 1, currentY - 1);
            }
        }

        if (wordFound) { return; }

        if ((currentY - 1 < numLines) && (currentY - 1 > 0)) {                                          //checks top middle adjacent space
            if (puzzle[currentX][currentY - 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX, currentY - 1);
                wordIndex++;
                solveWordSearch(currentX, currentY - 1);
            }
        }

        if (wordFound) { return; }

        if ((currentX + 1 < numColumns) && ((currentY - 1 < numLines) && (currentY - 1 > 0))) {         //checks top right adjacent space
            if (puzzle[currentX + 1][currentY - 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX + 1, currentY - 1);
                wordIndex++;
                solveWordSearch(currentX + 1, currentY - 1);
            }
        }

        if (wordFound) { return; }

        if (currentX + 1 < numColumns) {                                        //checks middle right adjacent space
            if (puzzle[currentX + 1][currentY].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX + 1, currentY);
                wordIndex++;
                solveWordSearch(currentX + 1, currentY);
            }
        }

        if (wordFound) { return; }

        if ((currentX + 1 < numColumns) && (currentY + 1 < numLines)) {         //checks bottom right adjacent space
            if (puzzle[currentX + 1][currentY + 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX + 1, currentY + 1);
                wordIndex++;
                solveWordSearch(currentX + 1, currentY + 1);
            }
        }

        if (wordFound) { return; }

        if (currentY + 1 < numLines) {                                          //checks bottom middle adjacent space
            if (puzzle[currentX][currentY + 1].equals(word.substring(wordIndex, wordIndex + 1))) {
                wordSearchStack.push(currentX, currentY + 1);
                wordIndex++;
                solveWordSearch(currentX, currentY + 1);
            }
        }

        if (wordFound) { return; }

        wordIndex--;
        wordSearchStack.pop();      //if the method makes it down here, none of the adjacent cells are a match, so we pop()
    } //solveWordSearch()

    public static void CircularQueue(Scanner in, PrintWriter out){
        CircularDeque cd = new CircularDeque();
        in.nextLine();               //here to make sure the problem number isn't read in
        int numLines = Integer.parseInt(in.nextLine());

        String currentLine;
        String[] currentLineArray;

        for (int i = 0; i < numLines; i++) {
            currentLine = in.nextLine();
            currentLineArray = currentLine.split(" ");
            if (currentLineArray[0].equals("d")) {              //first character equals d if we want to dequeue something
                if (currentLineArray[1].equals("b")) {          //second character equals b if we want to work with the back of the array
                    out.print(cd.dequeueBack());
                    out.flush();
                    if (i != numLines - 1) out.print("\n");
                    out.flush();
                }

                else {                                          //else, second character equals f, which means we want to work with the front of the array
                    out.print(cd.dequeueFront());
                    out.flush();
                    if (i != numLines - 1) out.print("\n");
                    out.flush();
                }
            }

            else if (currentLineArray[0].equals("e")) {         //first character equals e if we want to enqueue something
                if (currentLineArray[1].equals("b"))            //second character equals b if we want to work with the back of the array
                    cd.enqueueBack(currentLineArray[2]);

                else
                    cd.enqueueFront(currentLineArray[2]);
            }

            else {                                              //only other situation is if the character is an s
                out.print(cd.getQueueSize() + " " + cd.getFrontIndex() + " " + cd.getRearIndex());
                out.flush();
                if (i != numLines - 1) out.print("\n");
                out.flush();
            }
        }
    }  //CircularQueue()

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

        int ProblemNo=in.nextInt();

        if(ProblemNo==1){
            //Problem 1: Checking of your implemented stack
            StackChecking(in,out);
        }
        else if(ProblemNo==2){
            //Problem 2: Word searching problem
            WordSearch(in, out);
        }
        else if(ProblemNo==3){
            //Problem 3: Circular Dequeue Implementation checking
            CircularQueue(in, out);
        }
        else{
            System.out.println("Invalid Input");
            System.exit(0);
        }

        in.close();
        out.close();
    } //main()
} //Main Class