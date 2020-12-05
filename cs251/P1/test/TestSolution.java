import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.assertEquals;

public class TestSolution {
    private static final String INPUT_FOLDER = "inputFiles/";
    private static final String OUTPUT_FOLDER = "outputFiles/";
    private static final String TEMP_FILE_NAME = "temp.txt";

    private static final int TIME_OUT = 10000;

    @Test(timeout = TIME_OUT)
    public void testSingleDigitZeroX() {
        checkOutput("singleDigitZeroX.txt");
    }

    @Test(timeout = TIME_OUT)
    public void testMultipleDigitsNonzeroX() {
        checkOutput("multipleDigitsNonzeroX.txt");
    }

    /**
     * Read the input file given under inputFiles/
     * then compare your output against the expected output file
     * Note: the expected output file under outputFiles/ has the same file name as the given input file.
     *
     * To write your own test cases:
     * You can simply put your input files under inputFiles and expected output files under outputFiles
     * (input file and output file should have the same file name),
     * then call checkOutput(inputFileName).
     * @param testFileName input file name (not path).
     */
    private void checkOutput(String testFileName) {
        // Create a TriangularNums instance written by you
        TriangularNums tn = new TriangularNums(INPUT_FOLDER + testFileName);
        tn.writeToFile(TEMP_FILE_NAME);

        String answer;
        String expected;
        try {
            answer = new String(Files.readAllBytes(Paths.get(TEMP_FILE_NAME))).replace("\r", "");
            expected = new String(Files.readAllBytes(Paths.get(OUTPUT_FOLDER + testFileName))).replace("\r", "");
            // Compare your output with the expected output
            assertEquals(expected, answer);
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            Files.delete(Paths.get(TEMP_FILE_NAME));
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }
}
