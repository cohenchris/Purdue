import org.junit.After;
import org.junit.Test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintStream;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Scanner;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

public class TestHashTable {
    private static final String INPUT_FOLDER = "inputFiles/";
    private static final String OUTPUT_FOLDER = "outputFiles/";
    private static final String TEMP_FILE = "temp.txt";

    @Test(timeout = 10000)
    public void testSampleHashTable1() throws IOException {
        checkOutput("sampleHashTable1.txt", TEMP_FILE);
    }

    @Test(timeout = 10000)
    public void testSampleHashTable2() throws IOException {
        checkOutput("sampleHashTable2.txt", TEMP_FILE);
    }

    @Test(timeout = 10000)
    public void testSampleHashTable3() throws IOException {
        checkOutput("sampleHashTable3.txt", TEMP_FILE);
    }

    @Test(timeout = 10000)
    public void testSampleHashTable4() throws IOException {
        checkOutput("sampleHashTable4.txt", TEMP_FILE);
    }

    @After
    public void deleteTemp() {
        try {
            Files.delete(Paths.get(TEMP_FILE));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void checkOutput(String fileName, String outPath) throws IOException {
        Scanner in = new Scanner(new File(INPUT_FOLDER + fileName));
        PrintStream out = new PrintStream(new File(outPath));
        HashTable.generateOutput(3, in, out);
        out.close();
        String answer = new String(Files.readAllBytes(Paths.get(outPath))).replace("\r", "");
        String expected = new String(Files.readAllBytes(Paths.get(OUTPUT_FOLDER + fileName))).replace("\r", "");
        // Compare your output with the expected output
        assertEquals(expected, answer);
    }
}