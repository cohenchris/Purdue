import java.util.*;
public class ProteinValidator 
{
  public static void main(String args[])
  {
    Scanner s = new Scanner(System.in);
    boolean isProtein = false;
    System.out.print("Input the DNA fragment to be validated: ");
    String validate = s.nextLine();
    String lastThree = validate.substring(validate.length()-3);
    int startLocation = validate.indexOf("atg");
    if(startLocation < 0)
     startLocation = 0;
    if(lastThree.equals("tag")||lastThree.equals("taa")||lastThree.equals("tga"))
      isProtein = true;
    String gene = validate.substring(startLocation);
    System.out.println("Input DNA: "+validate);
    System.out.println("Start codon position: "+startLocation);
    System.out.println("Gene: "+gene);
    System.out.println("Protein: "+isProtein);
  }
}
