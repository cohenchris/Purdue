import java.util.*;
public class WordFiller
  {
  public static void main(String args[])
  {
    Scanner s = new Scanner(System.in);
    System.out.println("Please provide the following:");
    System.out.println("adjective");
    String adj1 = s.nextLine();
    System.out.println("nationality");
    String nat = s.nextLine();
    System.out.println("person");
    String per = s.nextLine();
    System.out.println("noun");
    String noun1 = s.nextLine();
    System.out.println("adjective");
    String adj2 = s.nextLine();
    System.out.println("noun");
    String noun2 = s.nextLine();
    System.out.println("adjective");
    String adj3 = s.nextLine();
    System.out.println("adjective");
    String adj4 = s.nextLine();
    System.out.println("plural noun");
    String pnoun = s.nextLine();
    System.out.println("noun");
    String noun3 = s.nextLine();
    System.out.println("number");
    String num1 = s.nextLine();
    System.out.println("shapes");
    String shapes = s.nextLine();
    System.out.println("food");
    String food1 = s.nextLine();
    System.out.println("food");
    String food2 = s.nextLine();
    System.out.println("number");
    int num2 = s.nextInt();
    System.out.println("number");
    int num3 = s.nextInt();
    int num4 = num3 * num2;
    System.out.println("Pizza was invented by a "+adj1+" "+nat+" chef named "+per+". To make a pizza, you need to take a lump of "+noun1+", and make a thin, round "+adj2+" "+noun2+". Then you cover it with "+adj3+" sauce, "+adj4+" cheese, and fresh chopped "+pnoun+". Next you have to bake it in a very hot "+noun3+". When it is done, cut it into "+num1+" "+shapes+". Some kids like "+food1+" pizza the best, but my favorite is the "+food2+" pizza. I eat "+num2+" whole pizzas every day that I eat pizza! In fact, I eat pizza "+num3+" days every week! That means I eat "+num4+" pizzas every week!");
  }
}