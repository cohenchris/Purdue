public class Node {
    private int key;
    public Node left;
    public Node right;
    public int color;
    public Node(int key) {
        this.key = key;
        this.left = null;
        this.right = null;
        this.color = Color.RED;
    } //Node()

    public int getKey() {
        return this.key;
    } //getKey()
    public void setKey(int key) {
        this.key = key;
    } //setKey()
    public int getColor() { return this.color; } //getColor()
    public void setColor(int color) { this.color = color; } //setColor()
}
