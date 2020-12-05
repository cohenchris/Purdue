import java.io.File;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.ArrayList;

/**
 * Chris Cohen
 * Purdue University - CS25000
 * Last Edited 4/16/2019
 */

public class Main {
    /**
     * This method reads in a file using the 'in' argument, and uses the 'out' argument to .......... to an output file.
     * @param in input stream
     * @param out output stream
     */
    public static void Part1(Scanner in, PrintWriter out){
        in.nextLine();                                  //discards problem number (we don't need it)
        String[] numArr = in.nextLine().split(" ");
        int numCities = Integer.parseInt(numArr[0]);   //reads in numCities from the in file
        int numRoutes = Integer.parseInt(numArr[1]);   //reads in numRoutes from the in file

        Graph newGraph = new Graph(numCities, numRoutes);
        String[] readLine;              //current line being read from the in file
        for (int i = 0; i < numRoutes; i++) {
            readLine = in.nextLine().split(" ");
            newGraph.addRoute(readLine[0], readLine[1], Double.parseDouble(readLine[2]));
        }

        out.print(newGraph.analyzeGraph());
    }

    /**
     * This method reads in a file using the 'in' argument, and prints the result of dijkstra's shortest path for each needed path using the 'out' argument.
     * @param in input stream
     * @param out output stream
     */
    public static void Part2(Scanner in, PrintWriter out){
        in.nextLine();                                  //discards problem number (we don't need it)
        String[] numArr = in.nextLine().split(" ");
        int numCities = Integer.parseInt(numArr[0]);   //reads in numCities from the in file
        int numRoutes = Integer.parseInt(numArr[1]);   //reads in numRoutes from the in file

        Graph newGraph = new Graph(numCities, numRoutes);
        String[] readLine;
        for (int i = 0; i < numRoutes; i++) {
            readLine = in.nextLine().split(" ");
            newGraph.addRoute(readLine[0], readLine[1], Double.parseDouble(readLine[2]));
        }

        //FIND DIJKSTRA STUFF
        readLine = in.nextLine().split(" ");
        ArrayList<String[]> findDijkstra = new ArrayList<>();
        while (!readLine[0].equals("END")) {                    //loops to find dijkstra stuff until we hit "END", which will be the last line of the file
            findDijkstra.add(readLine);
            readLine = in.nextLine().split(" ");
        }

        //LOOPS THROUGH EVERY LINE THAT NEEDS DIJKSTRA AND PRINTS OUT RESULT
        for (int i = 0; i < findDijkstra.size(); i++) {
            if (i == findDijkstra.size() - 1) {
                out.print(newGraph.findCost(findDijkstra.get(i)[0], findDijkstra.get(i)[1]));
            }
            else {
                out.println(newGraph.findCost(findDijkstra.get(i)[0], findDijkstra.get(i)[1]));
            }
        }
    } //Part2()

    /**
     * This method reads in an in file using the 'in' argument, finds the MST for the read in graph, and prints the Euler tour of the MST using the 'out' argument.
     * @param in input stream
     * @param out output stream
     */
    public static void Part3(Scanner in, PrintWriter out){
        in.nextLine();                                  //discards problem number (we don't need it)
        String[] numArr = in.nextLine().split(" ");
        int numCities = Integer.parseInt(numArr[0]);   //reads in numCities from the in file
        int numRoutes = Integer.parseInt(numArr[1]);   //reads in numRoutes from the in file

        Graph newGraph = new Graph(numCities, numRoutes);
        String[] readLine;
        for (int i = 0; i < numRoutes; i++) {
            readLine = in.nextLine().split(" ");
            newGraph.addRoute(readLine[0], readLine[1], Double.parseDouble(readLine[2]));
        }
        String rootMST = in.nextLine();                 //reads in the city that will end up being the root of the MST

        String eulerTour = newGraph.eulerianTour(rootMST);//printMe will be a fully-formatted Eulerian tour
        out.print(eulerTour);
    } //Part3()

    /**
     * Main method of the program.
     * @param args program arguments
     */
    public static void main(String[] args) {
        if(args.length!=2){
            System.out.println("ERROR: Not enough Parameters");
            System.exit(0);
        }
        Scanner in=null;
        PrintWriter out=null;
        try {
            in = new Scanner(new File(args[0]));
            out= new PrintWriter(new File(args[1]));
        }
        catch (Exception e){
            e.printStackTrace();
            System.exit(0);
        }

        int PartNo=in.nextInt();

        if(PartNo==1){
            Part1(in, out);
        }else if(PartNo==2){
            Part2(in, out);
        }else if(PartNo==3){
            Part3(in, out);
        }else{
            System.out.println("Invalid Input");
            System.exit(0);
        }

        in.close();
        out.close();
    } //main()
}