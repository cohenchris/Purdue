import java.util.Random;
/**
 * Skeleton Code for task 2
 */
public class SimpleGrader implements Runnable {
 
 	/* Do not change the code between this comment... */
	public static int[] correctAnswers = generateCorrectAnswers();                 //note that this variable is static
	public static int[][] studentAnswers = generateStudentAnswers(correctAnswers); //note that this variable is static
	/* ...and this comment */
 
 
	/* add or edit necessary variables here*/
	public static int grade = 0;
	public int qNum;
	public static Thread t[] = new Thread[10];
	public static Object obj = new Object();
	public static final int SEED = 2430;
	
	// TODO implement the constructor with necessary parameters
	/* Hint: When we create threads in main method, each thread only grades its assigned question
	 * Therefore, the user should be able to specify the index of the question being graded and we need a variable to store that index. That's it.
     */
	public SimpleGrader(int qNum)
	{
    	this.qNum = qNum;
    }
	

	@Override
	/**
	 * run() method will be called when a thread starts.
	 * in this case, the grading (changing the grade variable) should be done here
	 */
	public void run()
	{
		// TODO acquire the correct answer from correctAnswers, and grade each student's answer to that question accordingly.
		for(int i = 0; i < 1000; i++)
		{
			if(studentAnswers[qNum][i] == correctAnswers[qNum])
			{
				synchronized (obj)
				{
					grade += 10;
				}
			}
			else if(studentAnswers[qNum][i] == 0)
			{
				synchronized (obj)
				{
					grade += 0;
				}
			}
			else
			{
				synchronized (obj)
				{
					grade -= 3;
				}
			}
		}
	}
	
	public static void main(String[] args) {
		// TODO create ten threads, each responsible for one question (tip: make an array of threads could save you lots of work)
		/* As the class implements Runnable instead of extends Thread, here is an example line of creating a thread:
		 * Thread t = new Thread(new SimpleGrader(...)); //the arguments should be corresponding to your constructor
         */
		for (int i = 0; i < t.length; i++) {
			t[i] = new Thread(new SimpleGrader(i));
		}

		// TODO start all threads created
		for (int i = 0; i < t.length; i++)
		{
			t[i].start();
		}
		// TODO wait for all threads to finish. If there is an InterruptedException, print out "Oops!"
		try{
			for(int i = 0; i < t.length; i++)
			{
				t[i].join();
			}
		}catch(InterruptedException e)
		{
			System.out.println("Oops!");
		}
		// Determines the average for the test; you can make changes if necessary
		double average = grade / 1000.0;
		System.out.printf("The average score is %.3f\n", average);
	}
	
	/* Do not change lines below */
	public static int[] generateCorrectAnswers(){
		int[] answers = new int[10];
		Random r = new Random(SEED);
		for(int i = 0; i < answers.length; i ++){
			answers[i] = r.nextInt(4) + 1;
		}
		return answers;
	}

	public static int[][] generateStudentAnswers(int[] answers){
		int[][] response = new int[10][1000];
		Random r = new Random(SEED);
		for(int i = 0; i < response.length; i ++){
			int answer = answers[i];
			for(int j = 0; j < response[i].length; j++){
				if(r.nextInt(10) < 7){
					response[i][j] = answer;
				}else if(r.nextBoolean()){
					response[i][j] = 0;
				}else{
					response[i][j] = r.nextInt(4) + 1;
				}
			}
		}
		return response;
	}
}
