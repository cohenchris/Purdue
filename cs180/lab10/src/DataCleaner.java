/**
 * Created by Sarah Rodenbeck on 3/20/2018.
 */

import java.io.*;
import java.util.ArrayList;

public class DataCleaner {
    private ArrayList<InternalRepresentation> rows = new ArrayList<InternalRepresentation>();

    public InternalRepresentation parseAndClean(String s){

        //TODO get rid of the last 3 data points (latitude,longitude, last modified)
        ArrayList<String> str = new ArrayList<>();

        String strings[] = s.split(",");

        for(String st:strings)
        {
            str.add(st);
        }

        str.remove(str.size()-1);
        str.remove(str.size()-2);
        str.remove(str.size()-3);
        //TODO get rid of empty element (borough) and omit row if any other data missing
        int count = 0;

        for(int i = 0; i < str.size(); i++)
        {
            if(str.get(i).equals("")){
                str.remove(i);
                count++;
            }
        }

        if(count > 1)
            return null;
        //TODO convert types and store in InternalRepresentation object
        int roomID = Integer.parseInt(str.get(0));
        int hostID = Integer.parseInt(str.get(1));
        String roomType = str.get(2);
        String neighborhood = str.get(3);
        int reviews = Integer.parseInt(str.get(4));
        double overallSatisfaction = Double.parseDouble(str.get(5));
        int accommodates = Integer.parseInt(str.get(6));
        double bedrooms = Double.parseDouble(str.get(7));
        double price = Double.parseDouble(str.get(8));
        double minStay = Double.parseDouble(str.get(9));

        return new InternalRepresentation(roomID, hostID, roomType, neighborhood, reviews, overallSatisfaction, accommodates, bedrooms, price, minStay);
    }

    public void openAndRead(){
        //TODO open file and create BufferedReader
        try {
            File f = new File("/homes/cohen50/CS180/lab10/src/airbnb_christchurch_2016.csv");
            FileReader fr = new FileReader(f);
            BufferedReader br = new BufferedReader(fr);
        //TODO read in each line from the file; parse/clean input and add result to ArrayList
            br.readLine();
            while(true)
            {
                String s = br.readLine();
                if(s == null)
                    break;
                rows.add(parseAndClean(s));
            }
            br.close();
        }catch (FileNotFoundException e){
            System.out.println("File Not Found!");
        }catch(IOException e){
            System.out.println("IOException");
        }
    }

    public void writeCleanData() {
        //TODO create files and PrintWriters
        try {
            File f1 = new File("/homes/cohen50/CS180/lab10/src/Cleaned_Airbnb_Christchurch.csv");
            File f2 = new File("/homes/cohen50/CS180/lab10/src/GoodPrivate_Airbnb_Christchurch.csv");
            FileOutputStream fos1 = new FileOutputStream(f1, true);
            FileOutputStream fos2 = new FileOutputStream(f2, true);
            PrintWriter pw1 = new PrintWriter(fos1);
            PrintWriter pw2 = new PrintWriter(fos2);
            for(int i = 0; i < rows.size(); i++)
            {
                if(rows.get(i) != null)
                    pw1.println(rows.get(i).PrintRowCSV());
            }

            for(int j = 0; j < rows.size(); j++)
            {
                if(rows.get(j) != null) {                                   //Printing things that have shared room value in GoodPrivate csv file, even with this loop. Megan and Romil looked at it for a while and couldn't find anything wrong.
                    if (rows.get(j).getOverall_satisfaction() >= 4.0 && rows.get(j).getReviews() >= 10 && !rows.get(j).getRoomType().equalsIgnoreCase("Shared room"))
                        pw2.println(rows.get(j).PrintRowCSV());
                }
            }
            pw1.close();
            pw2.close();
        }catch(FileNotFoundException e){
            System.out.println("File Not Found!");
        }
        //TODO write to files based on conditions specified in handout

    }


    public static void main(String[] args) throws IOException{
        DataCleaner d = new DataCleaner();
        d.openAndRead();
        d.writeCleanData();
    }
}
