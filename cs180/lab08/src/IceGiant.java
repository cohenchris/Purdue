public class IceGiant extends Planet
{
    private String spacecraftVisited;

    public IceGiant(double mass, double radius, String name, double distFromSun, String spacecraftVisited) throws PlutoNotAPlanetException
    {
        super(mass, radius, name, distFromSun);
        this.spacecraftVisited = spacecraftVisited;
    }

    public String getSpacecraftVisited()
    {
        return spacecraftVisited;
    }

    public void printInfo()
    {
        super.printInfo();
        System.out.printf(" This ice giant has been visited by the %s spacecraft.", getSpacecraftVisited());
    }
}
