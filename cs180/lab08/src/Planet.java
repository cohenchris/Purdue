public class Planet {
    private double mass, radius, distFromSun;
    private String name;


    public Planet(double mass, double radius, String name, double distFromSun) throws PlutoNotAPlanetException
    {
        if(name.equalsIgnoreCase("pluto"))
            throw new PlutoNotAPlanetException("Sorry Pluto...");
        this.mass = mass;
        this.radius = radius;
        this.name = name;
        this.distFromSun = distFromSun;
    }

    public double getVolume()
    {
        return (4.0/3)*(Math.PI)*(Math.pow(radius,3));
    }

    public double getDensity()
    {
        return (((double)(mass))/(double)(getVolume()));
    }

    public double getDistFromSun()
    {
        return distFromSun;
    }

    public double distToPlanet(Planet otherPlanet)
    {
        return Math.abs(this.getDistFromSun()-otherPlanet.getDistFromSun());
    }

    public String getName()
    {
        return name;
    }

    public void printInfo()
    {
        System.out.printf("This is the planet %s. It is %.2f AU from the Sun. It has density of %.2f kg/m^3.",getName(), getDistFromSun(), getDensity());
    }
}

