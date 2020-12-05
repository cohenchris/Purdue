//NOTE: You will be using the random package for probability
import java.util.Random;
import java.lang.Math;

public class XerosCoin implements CryptoCurrency {
	private Demand demand;
	private double interest;
	private double profit;
	private int muns;
	private double worth;
	Random r = new Random();

	public XerosCoin(Demand demand, int muns)
	{
		//TODO: Implement Constructor
		this.demand = demand;
		this.muns = muns;
		interest = 0.0;
		worth = 0.75;
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
			interest = -0.05;
		else if(demand == Demand.BELOW_AVERAGE)
			interest = -0.025;
		else if(demand == Demand.AVERAGE)
			interest += 0;
		else if(demand == Demand.ABOVE_AVERAGE)
			interest = 0.025;
		else
			interest = 0.05;
		profit = muns*(worth*(Math.pow((1+(interest/3)),(9))));
	}

	public void mine(int attempts)
	{
		for(int i = 0; i < attempts; i++)
		{
			int x = r.nextInt(100) + 1;
			if (x == 1 || x == 2 || x == 3 || x == 4)
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