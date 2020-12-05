public class IUStudent extends CollegeStudent
{
    private double bookFees, transportationCost, financialAid;
    public IUStudent(String residency, boolean livesOffCampus, double GPA) throws IllegalArgumentException
    {
        super(residency, livesOffCampus);
        if(!residency.equalsIgnoreCase("OUT_OF_STATE") && !residency.equalsIgnoreCase("IN_STATE"))
            throw new IllegalArgumentException("Student must be in state or out of state.");
        bookFees = 1034;
        if(livesOffCampus)
        {
            transportationCost = 500;
            super.setDormCost(400);
        }
        else
        {
            transportationCost = 100;
            super.setDormCost(800);
        }
        if(GPA >= 2.50)
            financialAid = 2500;
        if(GPA >= 3.00)
            financialAid += 250;
        if(GPA >= 3.50)
            financialAid += 750;
        if(GPA >= 3.75)
            financialAid += 1000;
        if(GPA < 0)
            throw new IllegalArgumentException("GPA needs to be above or equal to 0.");
        if(getResidency() == Residency.OUT_OF_STATE)
            super.setTuition(34846);
        else if(getResidency() == Residency.IN_STATE)
            super.setTuition(10534);


    }

    public void setTuition(double tuition)
    {
        super.setTuition(tuition);
    }

    public void setDormCost(double dormCost)
    {
        super.setDormCost(dormCost);
    }

    public void setTransportationCost(double transportationCost)
    {
        this.transportationCost = transportationCost;
    }

    public void setBookFees(double bookFees)
    {
        this.bookFees = bookFees;
    }

    public void setFinancialAid(double financialAid)
    {
        this.financialAid = financialAid;
    }

    public double getBookFees()
    {
        return bookFees;
    }

    public double getFinancialAid()
    {
        return financialAid;
    }

    public double getTransportationCost()
    {
        return transportationCost;
    }

    public double calculateYearlyCost()
    {
        return super.calculateYearlyCost() + bookFees + transportationCost - financialAid;
    }

    public String toString()
    {
        return super.toString() + "\nBook Fees: "+getBookFees()+"\nTransportation Cost: "+getTransportationCost()+"\nFinancial Aid: "+getFinancialAid();
    }
}
