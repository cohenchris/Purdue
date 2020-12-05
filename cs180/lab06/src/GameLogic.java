import javax.swing.*;
import java.awt.*;
import java.util.Random;

/**
 *
 */

public class GameLogic
{
    static Cell grid[][];
    static Cell gridUpdate[][];
    Random r = new Random(3);

    public GameLogic()
    {
        grid = new Cell[100][100];
        gridUpdate = new Cell[100][100];
        int z;
        for(int i = 0;i < grid.length;i++)
        {
            for(int j = 0;j < grid[i].length;j++)
            {
                z = r.nextInt(10)+1;
                if(z == 1)
                    grid[i][j] = new Cell(true);
                else
                    grid[i][j] = new Cell(false);
            }
        }
        gridUpdate = grid;
    }

    boolean liveToNext(int m,int n)
    {
        //m = row, n = column
        int count = 0;

        if(((m+1) < 100) && (grid[m+1][n].getLiving()))           //counts living cells nearby
            count++;
        if(((m-1) >= 0) && grid[m-1][n].getLiving())
            count++;
        if(((n-1) >= 0) && grid[m][n-1].getLiving())
            count++;
        if(((n+1) < 100) && grid[m][n+1].getLiving())
            count++;
        if(((m+1) < 100) && ((n-1) >= 0) && grid[m+1][n-1].getLiving())
            count++;
        if(((m+1) < 100) && ((n+1) < 100) && grid[m+1][n+1].getLiving())
            count++;
        if(((m-1) >= 0) && ((n+1) < 100) && grid[m-1][n+1].getLiving())
            count++;
        if(((m-1) >= 0) && ((n-1) >= 0) && grid[m-1][n-1].getLiving())
            count++;

        if(count == 0)
            return false;

        if(grid[m][n].getLiving())
        {
            if(count == 1)
                return false;
            else if(count >= 4)
                return false;
            else if(count == 2 || count == 3)
                return true;
        }
        else
        {
            if(count == 3)
                return true;
        }
            return false;

    }


    /*You do not need to modify any of the below code!*/
    public void update() {
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (liveToNext(i, j)) {
                    gridUpdate[i][j].setLiving(true);
                } else {
                    gridUpdate[i][j].setLiving(false);
                }
            }
        }
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                grid[i][j].setLiving(gridUpdate[i][j].getLiving());
            }
        }
    }

    public static JFrame frame = new JFrame();
    public static JPanel[][] gridGUI;

    public void initGridGUI() {
        gridGUI = new JPanel[grid.length][grid[0].length]; //allocate the size of grid
        frame.setLayout(new GridLayout(grid.length, grid[0].length)); //set layout
        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                gridGUI[i][j] = new JPanel(); //creates new panel
                gridGUI[i][j].setBackground(Color.white);
                frame.add(gridGUI[i][j]);
            }
        }
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack(); //sets appropriate size for frame
        frame.setLocationRelativeTo(null);
        frame.setTitle("Conway's Game of Life");
        frame.setVisible(true);
    }

    public void printGridGUI() {

        for (int i = 0; i < grid.length; i++) {
            for (int j = 0; j < grid[i].length; j++) {
                if (grid[i][j].getLiving()) {
                    gridGUI[i][j].setBackground(Color.black);
                } else {
                    gridGUI[i][j].setBackground(Color.white);
                }
            }
        }
    }

    public void printGrid() {

        for (int i = 0; i < grid.length; i++) {
            if (i == 0) {
                for (int k = 0; k < grid[i].length + 1; k++) {
                    System.out.print("-");
                }
                System.out.println("-");
            }
            for (int j = 0; j < grid[i].length; j++) {
                if (j == 0) {
                    System.out.print("|");
                }
                if (j == grid[i].length - 1) {
                    if (grid[i][j].getLiving()) {
                        System.out.print("*");
                    } else {
                        System.out.print(" ");
                    }
                    System.out.println("|");
                } else {
                    if (grid[i][j].getLiving()) {
                        System.out.print("*");
                    } else {
                        System.out.print(" ");
                    }
                }
            }
            if (i == grid.length - 1) {
                for (int k = 0; k < grid[i].length; k++) {
                    System.out.print("-");
                }
                System.out.println("-");
            }
        }
    }
}
