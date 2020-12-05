public abstract class StellarGrouping {
    private String name;
    private Star[] stars;

    public StellarGrouping(String name, Star[] stars){
        this.name = name;
        this.stars = stars;
    }//Constructor

    public String planetsOrbiting(){
        return "Unknown";
    }//planetsOrbiting

    public Star brightestStar(){
        double min = stars[0].getMagnitude();
        Star minStar = stars[0];
        for(Star s: stars){
            if(s.getMagnitude() < min){
                minStar = s;
                min = s.getMagnitude();
            }//end if
        }//end for each
        return minStar;
    }//brighestStar

    public abstract boolean isVisible(Month month);//isVisible

    public double distanceFromEarth(String starName){
        for(Star s: stars){
            if(s.getName().equalsIgnoreCase(starName))
                return s.getLightyearsFromEarth() * 5.878625;
        }//end for each
        return -1;
    }//distanceFromEarth

    public double distanceFromEarth(Star s){
        for(Star st: stars){
            if(st == s)
                return s.getLightyearsFromEarth() * 5.878625;
        }//end for each
        return -1;
    }//distanceFromEarth

    public double furthestStar(){
        double max = distanceFromEarth(stars[0]);
        for(Star s: stars){
            if(distanceFromEarth(s) > max)
                max = distanceFromEarth(s);
        }//end for each
        return max;
    }//furthestStar

    public String getName(){
        return name;
    }//getName

    public Star[] getStars() {
        return stars;
    }//getStars
}
