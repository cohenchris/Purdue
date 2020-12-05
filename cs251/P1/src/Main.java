/**
 * @author Hongxin Chu
 */

import java.io.*;

public class Main {
    public static void main(String[] args) {
        if(args.length != 2) {
            System.out.println("Input file path and output file path should be specified.");
            return;
        }
        TriangularNums tn = new TriangularNums(args[0]);
        tn.writeToFile(args[1]);
    }
}