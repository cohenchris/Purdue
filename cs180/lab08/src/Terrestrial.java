public class Terrestrial extends Planet
{
    private String habitability;

    public Terrestrial(double mass, double radius, String name, double distFromSun, String habitability) throws PlutoNotAPlanetException
    {
        super(mass, radius, name, distFromSun);
        this.habitability = habitability;
    }

    public String getHabitability()
    {
        return habitability;
    }

    public void printInfo()
    {
        super.printInfo();
        System.out.printf(" This terrestrial planet is %s.",getHabitability());
    }
}
