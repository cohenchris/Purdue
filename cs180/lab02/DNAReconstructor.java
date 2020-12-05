import java.util.*;
import java.text.*;
public class DNAReconstructor
{
  public static void main(String args[])
  {
    Scanner s = new Scanner(System.in);
    String yourDNA = "";
    System.out.println("Input DNA fragments one line at a time.\nWe start with no DNA:\n");
    System.out.println("Your DNA (1): \""+yourDNA+"\"");
    System.out.print("Initial Sequence: ");
    String initial = s.nextLine();
    String cutDown = initial.substring(0,initial.length()-3);
    yourDNA += cutDown;
    yourDNA = yourDNA.toLowerCase();
    System.out.println("Your DNA (2): \""+initial+"\"");
    System.out.print("Sequencer: ");
    yourDNA += s.nextLine();
    yourDNA = yourDNA.toLowerCase();
    System.out.println("New Sequence: \""+yourDNA+"\"");
  }
}
