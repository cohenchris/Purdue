import java.nio.charset.StandardCharsets;
import java.util.*;
import java.io.*;
import java.lang.*;

/**
 * Read a file that contains 2 lines:
 * <p>
 * <ol>
 *     <li>There are 2 numbers in this line:
 *         <ul>
 *             <li>First one specifies maximum digits allowed for each number.</li>
 *             <li>Second one specifies X such that if current line in output has C numbers, then next line has C+X numbers.
 *                  The last line in the output can ignore this rule if numbers provided cannot fill up the last line.
 *             </li>
 *         </ul>
 *     </li>
 *     <li>Sequence of numbers.</li>
 * </ol>
 * writeToFile() writes the string of these numbers which are in triangular pattern to a file.
 * All numbers are <b>left justified</b>.
 * There is no "\n" at the end of the last line.
 *
 * E.g.
 * Input file:
 * 2 2
 * 1 2 3 4 5 6 7 8 9 10
 *
 * Output:
 * | 1  |
 * | 2  | 3  | 4  |
 * | 5  | 6  | 7  | 8  | 9  |
 * | 10 |
 *
 * @author Hongxin Chu
 */
public class TriangularNums {
    static int N = 0;
    static int X = 0;
    static List<String> listNums = new ArrayList<String>();
    private Scanner s;
    //private PrintWriter w;

    /**
     * Construct a new TriangularNums object.
     * @param inputPath Path of the input file.
     */
    public TriangularNums(String inputPath) {
        /** Creates a new Scanner object that reads from the file with path inputPath,
        argument 1.
         */

        try {
            s = new Scanner(new File(inputPath));
        } catch (Exception e) {
            e.printStackTrace();
        }

        /** Assigns N and X variables, then assigns all numbers to the List. */
        try {
            if (s.hasNext()) {
                String str = s.nextLine();
                String[] parts = str.split("\\s+");
                N = Integer.parseInt(parts[0]);
                X = Integer.parseInt(parts[1]);
            }

            while (s.hasNext()) {
                listNums.add(s.next());
            }
        } catch (Exception e) {
            System.out.println("File Read Error!");
            e.printStackTrace();
        }
        s.close();
    }

    /**
     * Writes the string of numbers which are in triangular pattern to a file.
     * Make sure you flush the content to file instead of keeping the string inside the buffer.
     * Make sure no output stream is open when return.
     * @param filePath The path of the file to write.
     */
    public void writeToFile(String filePath) {
        PrintWriter w = null;
        try {
            w = new PrintWriter(filePath);
        } catch (Exception e) {
            System.out.println("Error Opening File!");
            e.printStackTrace();
        }

        int numOnLine = 1;
        int listIndex = 0;
        while (listNums.get(listIndex) != null) {
            for (int i = 0; i < numOnLine; i++) {
                w.write("| ");
                w.flush();
                String numberString = listNums.get(listIndex);
                System.out.println(numberString);
                while(numberString.length() != (N+1)) {
                    numberString += " ";
                }


                w.write(numberString);
                w.flush();
                listIndex++;
                if (listIndex == listNums.size()) {
                    w.write("|");
                    w.flush();
                    return;
                }
            }
            w.write("|\n");
            w.flush();
            numOnLine += X;
        }
        w.close();
    }
}
