import java.util.*;
public class Questions
{
  public static void main(String args[])
  {
    String animal = "";
    Scanner s = new Scanner(System.in);
    System.out.println("Does it have fur?");
    String fur = s.nextLine();
    if(fur.equals("yes"))
    {
      System.out.println("Does it live in trees?");
      String trees = s.nextLine();
      if(trees.equals("yes"))
         {
           System.out.println("Does it eat leaves?");
           String leaves = s.nextLine();
           if(leaves.equals("yes"))
           {
             System.out.println("Is it's poop green?");
             String poop = s.nextLine();
             if(poop.equals("yes"))
                animal = "Koala";
             else
                animal = "Panda";
           }
           else
           {
             System.out.println("Is it a big cat?");
             String cat = s.nextLine();
             if(cat.equals("yes"))
                animal = "Jaguar";
             else
                animal = "Monkey";
           }
      }
     else
     {
       System.out.println("Does it live underground?");
       String under = s.nextLine();
       if(under.equals("yes"))
            animal = "Groundhog";
       else
            animal = "Rat";
     }
         }
    
   else
   {
     System.out.println("Does it have scales?");
     String scales = s.nextLine();
     if(scales.equals("yes"))
          {
              System.out.println("Does it live in water?");
              String water = s.nextLine();
              if(water.equals("yes"))
                   animal = "Fish";
              else
                   animal = "Lizard";
          }
     else
     {
       System.out.println("Is it an amphibian?");
       String amp = s.nextLine();
       if(amp.equals("yes"))
            animal = "Newt";
       else
            {
                System.out.println("Can it fly?");
                String fly = s.nextLine();
                if(fly.equals("yes"))
                     animal = "Hawk";
                else
                     animal = "Penguin";
            }
     }
   }
System.out.println("It is a "+animal);
  }
}