import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner in;
        PrintStream out;
        try {
            in = new Scanner(new File(args[0]));
            out = new PrintStream(new File(args[1]));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        // Construct HashTable or Heap here
    }
}
