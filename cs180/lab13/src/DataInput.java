import java.io.*;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

/**
 *
 */
public class DataInput {
    public static Star[] stars;
    public void read(String filename){
        try {
            BufferedReader bfr = new BufferedReader(new FileReader(new File(filename)));
            ArrayList<Star> stars = new ArrayList<Star>();

            bfr.readLine();
            while(true) {
                String s = bfr.readLine();
                if (s==null){
                    break;
                }
                stars.add(parse(s));
            }
            this.stars = stars.toArray(new Star[1]);
            bfr.close();
        }catch (FileNotFoundException e) {
            System.out.println("File not found!");
        }
        catch (IOException e) {
            System.out.println("IO Exception");
        }
    }

    public Star parse(String input){
        String [] initial = input.split(",");
        boolean planets = false;
        if (initial[3].equals("Yes")){
            planets = true;
        }
        return new Star(initial[0],Double.parseDouble(initial[1]),Double.parseDouble(initial[2]),planets);
    }

    public void printInfo(StellarGrouping sg, Month month){
        //TODO: Print the requested information
        String visible = "No";
        if(sg.isVisible(month))
            visible = "Yes";
        System.out.println("Grouping name: " + sg.getName() + "\nThe brightest star in this grouping is " + sg.brightestStar().getName() + ".\n " + sg.brightestStar().getName()+ " is " + sg.distanceFromEarth(sg.brightestStar()) + " trillion miles from earth.\nThe furthest star from earth in this grouping is " + sg.furthestStar() + " trillion miles from earth.\nCurrently visible to the naked eye: " + visible + "\nPlanets orbiting one or more stars in this grouping: " + sg.planetsOrbiting());
        //Tip: The numbers printed will only be tested for up to the first three decimal places

    }

    public static void main(String[] args) {
        //TODO: Parse command line arguments (the first should be the CSV name, the second should be the month)
        //Tip: You can use <Enum_Name>.valueOf(<String>) to get an enum constant of the same name as the String (case sensitive)
        String name = args[0];
        Month monthValue = Month.valueOf(args[1]);
        //TODO: Create an instance of DataInput and pass the filename to .read
        DataInput di = new DataInput();
        di.read(name);
        //TODO: Create a StellarGrouping instance and print the info for that grouping and the month given above
        StellarGrouping sgTest = new SouthernSummerConstellation("Canis Major", di.stars);
        di.printInfo(sgTest,monthValue);
    }

}
