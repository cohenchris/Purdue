import java.util.*;
import java.lang.*;
import java.text.*;

public class CryptoClient {

    /**
     * @param args
     */
    public static void main(String[] args) {
        DecimalFormat f = new DecimalFormat("0.00");
        //DO NOT EDIT CODE BELOW THIS-----------------------------------------------------------------------------------
        CryptoCurrency s1, s2, s3;
        CryptoClient c = new CryptoClient();
        Scanner s = new Scanner(System.in);
        boolean investAgain = true;
        Demand JVarnDemand = c.Genrandom();
        Demand VictoryDemand = c.Genrandom();
        Demand XerosDemand = c.Genrandom();

        do {
            System.out.printf("Welcome to CryptoCo, the leading exchange for semi-legitimate currency!\n" +
                    "Our cheapest package involves allows you to invest in Three CryptoCurrencies\n" +
                    "Here are your options:\n\t1.JVarnCoin\n\t2.VictoryCoin\n\t3.XerosCoin\n" +
                    "Please enter what your first investment.\n");
            String choice1 = s.nextLine();
            System.out.printf("How many coins will you start with?\n");
            int invest1 = s.nextInt();
            System.out.printf("Great! Now what is the second crypto currency you would like to invest in?\n");
            s.nextLine();
            String choice2 = s.nextLine();
            System.out.printf("How many coins will you start with?\n");
            int invest2 = s.nextInt();
            System.out.printf("Great! Now what is the third crypto currency you would like to invest in?\n");
            s.nextLine();
            String choice3 = s.nextLine();
            System.out.printf("How many coins will you start with?\n");
            int invest3 = s.nextInt();
            System.out.println("Calculating... ");
            //DO NOT EDIT THE CODE ABOVE THIS---------------------------------------------------------------------------

            //TODO: Assign coins to s1, s2, and s3 based on the user's selections
            if(Integer.parseInt(choice1) == 1)
                s1 = new JVarnCoin(JVarnDemand, invest1);
            else if(Integer.parseInt(choice1) == 2)
                s1 = new VictoryCoin(VictoryDemand, invest1);
            else
                s1 = new XerosCoin(XerosDemand, invest1);

            if(Integer.parseInt(choice2) == 1)
                s2 = new JVarnCoin(JVarnDemand, invest2);
            else if(Integer.parseInt(choice2) == 2)
                s2 = new VictoryCoin(VictoryDemand, invest2);
            else
                s2 = new XerosCoin(XerosDemand, invest2);

            if(Integer.parseInt(choice3) == 1)
                s3 = new JVarnCoin(JVarnDemand, invest3);
            else if(Integer.parseInt(choice3) == 2)
                s3 = new VictoryCoin(VictoryDemand, invest3);
            else
                s3 = new XerosCoin(XerosDemand, invest3);

            System.out.println("great!");
            
            //TODO: Implement the menu for each of the user's three selections.
            System.out.println("How would you like to acquire capital for coin 1?\n1. Mine it\n2. Purchase some with current CryptoCurrency profit\n3. Do not alter CryptoCurrency");
            int capitalChoice1 = s.nextInt();
            if(capitalChoice1 == 1)
            {
                System.out.println("How many attempts would you like to make?");
                int attempts1 = s.nextInt();
                s1.mine(attempts1);
            }
            else if(capitalChoice1 == 2)
            {
                System.out.println("How many coins would you like to purchase?");
                int purchase1 = s.nextInt();
                s1.purchase(purchase1);
            }

            System.out.println("How would you like to acquire capital for coin 2?\n1. Mine it\n2. Purchase some with current CryptoCurrency profit\n3. Do not alter CryptoCurrency");
            int capitalChoice2 = s.nextInt();
            if(capitalChoice2 == 1)
            {
                System.out.println("How many attempts would you like to make?");
                int attempts2 = s.nextInt();
                s2.mine(attempts2);
            }
            else if(capitalChoice2 == 2)
            {
                System.out.println("How many coins would you like to purchase?");
                int purchase2 = s.nextInt();
                s2.purchase(purchase2);
            }

            System.out.println("How would you like to acquire capital for coin 3?\n1. Mine it\n2. Purchase some with current CryptoCurrency profit\n3. Do not alter CryptoCurrency");
            int capitalChoice3 = s.nextInt();
            if(capitalChoice3 == 1)
            {
                System.out.println("How many attempts would you like to make?");
                int attempts3 = s.nextInt();
                s3.mine(attempts3);
            }
            else if(capitalChoice3 == 2)
            {
                System.out.println("How many coins would you like to purchase?");
                int purchase3 = s.nextInt();
                s3.purchase(purchase3);
            }

            System.out.println("Calculating profit:\n1");
            //TODO: Calculate profit for s1
            s1.assessProfit();
            System.out.println("Done.\n2");
            //TODO: Calculate profit for s2
            s2.assessProfit();
            System.out.println("Done\n3");
            //TODO: Calculate profit for s3
            s3.assessProfit();
            System.out.println("Done");

            //TODO: Compare results
            double max;
            int option;
            if(s1.getProfit() > s2.getProfit() && s1.getProfit() > s3.getProfit())
            {
                max = s1.getProfit();
                option = 1;
            }
            else if(s2.getProfit() > s3.getProfit() && s2.getProfit() > s1.getProfit())
            {
                max = s2.getProfit();
                option = 2;
            }
            else
            {
                max = s3.getProfit();
                option = 3;
            }

            System.out.println("The best option is option "+option+" with CryptoCo account profit of $"+f.format(max));

            System.out.println("Would you like to invest again?");
            s.nextLine();
            String choice = s.nextLine();
            if (choice.equalsIgnoreCase("yes") || choice.equalsIgnoreCase("y")) {
                investAgain = true;
            } else {
                investAgain = false;
            }

        } while (investAgain);
    }
    //NOTE: THIS LINE SHOULD NOT BE BELOW LINE 175

    public Demand Genrandom() {
        Random r = new Random();
        int chance = r.nextInt(5);
        switch (chance) {
            case 0:
                return Demand.HIGH;
            case 1:
                return Demand.ABOVE_AVERAGE;
            case 2:
                return Demand.AVERAGE;
            case 3:
                return Demand.BELOW_AVERAGE;
            case 4:
                return Demand.LOW;
        }
        return Demand.AVERAGE;
    }
}