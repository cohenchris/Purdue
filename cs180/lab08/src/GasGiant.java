public class GasGiant extends Planet
{
    private int numMoons;

    public GasGiant(double mass, double radius, String name, double distFromSun, int numMoons) throws PlutoNotAPlanetException
    {
        super(mass, radius, name, distFromSun);
        this.numMoons = numMoons;
    }

    public int getNumMoons()
    {
        return numMoons;
    }

    public void printInfo()
    {
        super.printInfo();
        System.out.printf(" This gas giant has %d moons.", getNumMoons());
    }
}
