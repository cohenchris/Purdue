/**
 * Runtime exception that indicates a part of the project has not been implemented yet.
 *
 * @author Hongxin Chu
 */
public class NotImplementedException extends RuntimeException{
    public NotImplementedException() {
        super();
    }

    public NotImplementedException(String message) {
        super(message);
    }
}
