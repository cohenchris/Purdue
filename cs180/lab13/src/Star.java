public class Star {
    private String name;
    private double magnitude, lightyearsFromEarth;
    private boolean planetsOrbiting;

    public Star(String name, double magnitude, double lightyearsFromEarth, boolean planetsOrbiting){
        this.name = name;
        this.magnitude=magnitude;
        this.lightyearsFromEarth = lightyearsFromEarth;
        this.planetsOrbiting =  planetsOrbiting;
    }//Constructor

    public String getName(){
        return name;
    }//getName

    public double getMagnitude(){
        return magnitude;
    }//getMagnitude

    public double getLightyearsFromEarth(){
        return lightyearsFromEarth;
    }//getLightyearsFromEarth

    public boolean getPlanetsOrbiting(){
        return planetsOrbiting;
    }//getPlanetsOrbiting
}
