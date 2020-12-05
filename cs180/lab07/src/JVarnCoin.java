//NOTE: You will be using the random package for probability
import java.util.Random;
import java.lang.Math;

public class JVarnCoin implements CryptoCurrency {
	private Demand demand;
	private double interest;
	private double profit;
	private int muns;
	private double worth;
	Random r = new Random();

	public JVarnCoin(Demand demand, int muns)
	{
		//TODO: Implement Constructor
		this.demand = demand;
		this.muns = muns;
		interest = 0.0;
		worth = 2.00;
	}

	//TODO: Implement methods required by the CryptoCurrency Interface
	public double getProfit()
	{
		return profit;
	}

	public double getInterest()
	{
		return interest;
	}

	public void assessProfit()
	{
		if(demand == Demand.LOW)
			interest = -0.15;
		else if(demand == Demand.BELOW_AVERAGE)
			interest = -0.075;
		else if(demand == Demand.AVERAGE)
			interest += 0;
		else if(demand == Demand.ABOVE_AVERAGE)
			interest = 0.075;
		else
			interest = 0.15;

		profit += muns*(worth*(Math.pow((1+(interest/3)),(9))));
	}

	public void mine(int attempts)
	{
		for(int i = 0; i < attempts; i++)
		{
			int x = r.nextInt(100) + 1;
			if (x == 1)
				muns++;
		}
	}

	public Demand getDemand()
	{
		return demand;
	}

	public void setDemand(Demand demand)
	{
		this.demand = demand;
	}

	public void purchase(int coin)
	{
		profit -= (coin*worth);
		muns += coin;
	}
}