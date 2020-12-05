import java.util.Arrays;
import java.util.Optional;

/**
 * The model of the vending machine application.
 *
 * <p>Lab 12 -- GUIs</p>
 */
public class VendingMachineModel {
    /**
     * The bins in the vending machine.
     */
    private Bin[] bins;

    /**
     * The profit of the vending machine.
     */
    private double profit;

    /**
     * Constructs a newly allocated {@code VendingMachineModel} object.
     */
    public VendingMachineModel() {
        this.bins = new Bin[12];
        this.profit = 0.0;

        this.initBins();
    } //VendingMachineModel

    /**
     * Initializes each element in {@code bins} with a reference to a specialized {@code Bin}.
     */
    private void initBins() {
        this.bins[0] = new Bin<Apple>("Apple", new Apple());
        this.bins[1] = new Bin<Aubergine>("Aubergine", new Aubergine());
        this.bins[2] = new Bin<Banana>("Banana", new Banana());
        this.bins[3] = new Bin<Broccoli>("Broccoli", new Broccoli());
        this.bins[4] = new Bin<Cherries>("Cherries", new Cherries());
        this.bins[5] = new Bin<Lemon>("Lemon", new Lemon());
        this.bins[6] = new Bin<Orange>("Orange", new Orange());
        this.bins[7] = new Bin<Pineapple>("Pineapple", new Pineapple());
        this.bins[8] = new Bin<Raspberry>("Raspberry", new Raspberry());
        this.bins[9] = new Bin<Strawberry>("Strawberry", new Strawberry());
        this.bins[10] = new Bin<Tomato>("Tomato", new Tomato());
        this.bins[11] = new Bin<Watermelon>("Watermelon", new Watermelon());
    } //initBins

    /**
     * Gets the bins in the vending machine.
     *
     * @return the bins in the vending machine
     */
    public Bin[] getBins() {
        return this.bins;
    } //getBins

    /**
     * Gets the profit of the vending machine.
     *
     * @return the profit of the vending machine
     */
    public double getProfit() {
        return this.profit;
    } //getProfit

    /**
     * Vends the specified item, adding to the vending machine's profit.
     *
     * @param itemName the name of the item to be vended
     */
    public Optional<Bin> vendItem(String itemName) {
        for (Bin bin : this.bins) {
            if (bin.getName().equalsIgnoreCase(itemName)) {
                Optional itemOptional = bin.getFirstItem();

                if (itemOptional.isPresent()) {
                    if (itemOptional.get() instanceof FoodItem) {
                        this.profit += ((FoodItem) itemOptional.get()).getPrice();
                    } //end if
                } //end if

                return Optional.of(bin);
            } //end if
        } //end for

        return Optional.empty();
    } //vendItem
}