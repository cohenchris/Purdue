public interface CryptoCurrency
{
	//TODO: Write method signatures.
    public double getProfit();  //returns profit variable
    public double getInterest();    //returns interest on the coin - interest calculated based on demand
    public void assessProfit(); //calculates the profit on your coin and assigns it to the profit variable
    public void mine(int attempts); //attempts to mine one coin 'attempts' number of times
    public Demand getDemand();  //gets the demand
    public void setDemand(Demand demand);   //allows you to set the demand
    public void purchase(int coin); //uses money from the profit variable in your crypto account and uses it to buy you more coins
}