/**
 * Created by Sarah Rodenbeck on 3/21/2018.
 */
//room_id,host_id,room_type,neighborhood,reviews,overall_satisfaction,accommodates,bedrooms,price,minstay,
//        latitude,longitude,last_modified

public class InternalRepresentation {
    private String roomType,neighborhood;
    private int roomID,hostID,reviews,accomodates;
    private double overall_satisfaction,bedrooms,price,minStay;

    public InternalRepresentation(int roomID, int hostID, String roomType, String neighborhood, int reviews, double
            overall_satisfaction, int accomodates,double bedrooms, double price, double minStay) {
        this.neighborhood = neighborhood;
        this.roomID = roomID;

        this.hostID = hostID;
        this.roomType = roomType;
        this.reviews = reviews;
        this.accomodates = accomodates;
        this.overall_satisfaction = overall_satisfaction;
        this.bedrooms = bedrooms;
        this.price = price;
        this.minStay = minStay;
    }

    public String getRoomType() {
        return roomType;
    }

    public String getNeighborhood() {
        return neighborhood;
    }

    public int getRoomID() {
        return roomID;
    }

    public int getHostID() {
        return hostID;
    }

    public int getReviews() {
        return reviews;
    }

    public int getAccomodates() {
        return accomodates;
    }

    public double getOverall_satisfaction() {
        return overall_satisfaction;
    }

    public double getBedrooms() {
        return bedrooms;
    }

    public double getPrice() {
        return price;
    }

    public double getMinStay() {
        return minStay;
    }

    public void PrintRowFormatted() {
        System.out.println("neighborhood: " + getNeighborhood());
        System.out.println("roomID: " + getRoomID());
        System.out.println("hostID: " + getHostID());
        System.out.println("roomType: " + getRoomType());
        System.out.println("reviews: " + getReviews());
        System.out.println("accomodates: " + getAccomodates());
        System.out.println("overall satisfaction: " + getOverall_satisfaction());
        System.out.println("bedrooms: " + getBedrooms());
        System.out.println("price: " + getPrice());
        System.out.println("minStay: " + getMinStay());
    }

    public String PrintRowCSV(){

        return getRoomID()+","+getHostID()+","+getRoomType()+","+getNeighborhood()+","+getReviews()
                +","+getOverall_satisfaction()+","+getAccomodates()+","+getBedrooms()+","+getPrice()+","+getMinStay();
    }
}
