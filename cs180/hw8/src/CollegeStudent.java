public class CollegeStudent
{
    private double dormCost, tuition;
    private Residency residency;
    private boolean livesOffCampus;

    public CollegeStudent(String residency) throws IllegalArgumentException
    {
        if(residency.equalsIgnoreCase("IN_STATE"))
        {
            this.residency = Residency.IN_STATE;
            tuition = 23000;
        }
        else if(residency.equalsIgnoreCase("OUT_OF_STATE"))
        {
            this.residency = Residency.OUT_OF_STATE;
            tuition = 42000;
        }
        else if(residency.equalsIgnoreCase("INTERNATIONAL"))
        {
            this.residency = Residency.INTERNATIONAL;
            tuition = 44500;
        }
        else
            throw new IllegalArgumentException("Student residency must be one of the three specified statuses.");
        livesOffCampus = false;
        dormCost = 800;
    }

    public CollegeStudent(String residency, boolean livesOffCampus) throws IllegalArgumentException
    {
        if(residency.equalsIgnoreCase("IN_STATE"))
        {
            this.residency = Residency.IN_STATE;
            tuition = 23000;
        }
        else if(residency.equalsIgnoreCase("OUT_OF_STATE"))
        {
            this.residency = Residency.OUT_OF_STATE;
            tuition = 42000;
        }
        else if(residency.equalsIgnoreCase("INTERNATIONAL"))
        {
            this.residency = Residency.INTERNATIONAL;
            tuition = 44500;
        }
        else
            throw new IllegalArgumentException("Student residency must be one of the three specified statuses.");
        this.livesOffCampus = livesOffCampus;
        if(livesOffCampus)
            dormCost = 500;
        else
            dormCost = 800;
    }

    public void setTuition(double tuition)
    {
        this.tuition = tuition;
    }

    public void setDormCost(double dormCost)
    {
        this.dormCost = dormCost;
    }

    public double calculateYearlyCost()
    {
        return tuition + dormCost *12;
    }

    public double getTuition()
    {
        return tuition;
    }

    public double getDormCost()
    {
        return dormCost;
    }

    public Residency getResidency()
    {
        return residency;
    }

    public boolean isLivingOffCampus()
    {
        return livesOffCampus;
    }

    public String toString()
    {
        String residence = "";
        if(getResidency() == Residency.IN_STATE)
            residence = "In State";
        else if(getResidency() == Residency.OUT_OF_STATE)
            residence = "Out Of State";
        else if(getResidency() == Residency.INTERNATIONAL)
            residence = "International";
        return "The total expenses are "+calculateYearlyCost()+"\nHere is the breakdown:\nThis student is "+residence+"\nYearly Tuition: "+getTuition()+"\nDorm Costs: "+getDormCost()*12;
    }
}