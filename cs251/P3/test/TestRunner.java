import org.junit.runner.JUnitCore;
import org.junit.runner.Result;
import org.junit.runner.notification.Failure;


public class TestRunner {
    public static void main(String[] args) {
        Result result = JUnitCore.runClasses(TestSuite.class);

        for (Failure failure : result.getFailures()) {
            System.out.println(failure.toString());
            System.out.println(failure.getTrace());
            System.out.println();
        }

        if(!result.wasSuccessful()) {
            System.out.println("Fails: " + result.getFailureCount());
        } else {
            System.out.println("All tests completed without failure.");
        }
    }
}
