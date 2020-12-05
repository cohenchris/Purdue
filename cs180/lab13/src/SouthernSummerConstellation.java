public class SouthernSummerConstellation extends StellarGrouping {

    public SouthernSummerConstellation(String name, Star[] stars) {
        super(name, stars);
    }//Constructor

    @Override
    public boolean isVisible(Month month) {
        if (month.equals(Month.NOVEMBER) || month.equals(Month.DECEMBER) || month.equals(Month.JANUARY) || month.equals(Month.FEBRUARY) || month.equals(Month.MARCH) || month.equals(Month.APRIL)) {
            return true;
        }//end if
        return false;
    }//isVisible

    @Override
    public String planetsOrbiting() {
        for (Star s : getStars()) {
            if (s.getPlanetsOrbiting())
                return "Yes";
        }//end for each
        return "Unknown";
    }//planetsOrbiting
}

