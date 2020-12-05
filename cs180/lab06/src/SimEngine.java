/**
 *
 */
public class SimEngine {

    public static void main(String[] args)
    {
        GameLogic g = new GameLogic();
        g.initGridGUI();
        sleepMethod(20);            //play around with duration at the end
        while(true)
        {
            g.update();
            g.printGridGUI();
            sleepMethod(20);        //play around with duration at the end
        }


    }





	/*You do not need to modify any of the below code!*/

    public static void sleepMethod(int duration){
        try {
            Thread.sleep(duration);
        } catch (InterruptedException e) {
            e.getMessage();
        }
    }


}
