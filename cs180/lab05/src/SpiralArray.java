import java.util.*;
public class SpiralArray
{
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        System.out.println("Welcome to the Spiral Array Generator!");
        boolean keepGenerating = true;
        do {
            System.out.println("What dimensions would you like?");
            int rows = s.nextInt();
            while (rows <= 0)
            {
                System.out.println("Please enter an integer greater than or equal to 1. How many rows do you want?");
                rows = s.nextInt();
            }
            int[][] arr = new int[rows][rows];
            int startCol = 0;
            int startRow = 0;
            int endCol = rows;
            int endRow = rows;
            int val = 1;
            String output = "";

            while (startRow < endRow && startCol < endCol) {
                for (int i = startCol; i < endCol; i++) {
                    arr[startRow][i] = val;
                    val++;
                }
                startRow++;
                for (int i = startRow; i < endRow; i++) {
                    arr[i][endCol-1] = val;
                    val++;
                }
                endCol--;
                for (int i = endCol-1; i > startCol; i--) {
                    arr[endRow-1][i] = val;
                    val++;
                }
                endRow--;
                for (int i = endRow; i >= startRow; i--) {
                    arr[i][startCol] = val;
                    val++;
                }
                startCol++;

            }

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < rows; j++) {
                    output += arr[i][j] + "\t";
                }
                output += "\n";
            }
            System.out.println(output);
            System.out.println("Do you want to generate another Spiral Array? (y/n)");
            s.nextLine();
            String answer = s.nextLine();
            while(!answer.equalsIgnoreCase("y") && !answer.equalsIgnoreCase("n"))
            {
                System.out.println("ERROR: input only 'y' or 'n'. Do you want to generate another Spiral Array?");
                answer = s.nextLine();
            }
            if(answer.equalsIgnoreCase("y"))
                keepGenerating = true;
            else if(answer.equalsIgnoreCase("n"))
            {
                keepGenerating = false;
                System.out.println("Thank you for using the Spiral Array Generator!");
            }

        }while(keepGenerating);
    }
    }
