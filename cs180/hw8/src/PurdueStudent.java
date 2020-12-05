public class PurdueStudent extends CollegeStudent
{
    private double bookFees, financialAid;
    private String major;

    public PurdueStudent(String residency, String major, boolean livesOffCampus, double GPA) throws IllegalArgumentException
    {
        super(residency, livesOffCampus);
        if(residency.equalsIgnoreCase("INTERNATIONAL"))
            super.setTuition(30954);
        else if(residency.equalsIgnoreCase("OUT_OF_STATE"))
            super.setTuition(28794);
        else if(residency.equalsIgnoreCase("IN_STATE"))
            super.setTuition(9992);
        this.major = major;
        if(major.equalsIgnoreCase("Computer Science"))
            bookFees = 200;
        else if(major.equalsIgnoreCase("Engineering"))
            bookFees = 500;
        else if(major.equalsIgnoreCase("Liberal Arts"))
            bookFees = 750;
        else
            bookFees = 100;
        if(GPA < 2.00)
            throw new IllegalArgumentException("GPA needs to be above or equals to 2.00");
        if(GPA >= 2.50)
            financialAid = 2500;
        if(GPA >= 3.00)
            financialAid += 500;
        if(GPA >= 3.50)
            financialAid += 1500;
        if(GPA >= 3.75)
            financialAid += 500;
    }

    public void setTuition(double tuition)
    {
        super.setTuition(tuition);
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

    public String getMajor()
    {
        return major;
    }

    public double calculateYearlyCost()
    {
        return super.calculateYearlyCost() + bookFees - financialAid;
    }

    public String toString()
    {
        return super.toString() + "\nBook Fees: "+getBookFees()+"\nFinancial Aid: "+getFinancialAid();
    }
}
