//implementation of stack using your implemented Linked List

public class Stack {
    private int size = 0;  //size variable
    private Node first = null;

    public void push(int pointOne, int pointTwo){
        Node oldFirst = first;
        first = new Node();

        first.point = new Pair();
        first.point.x = pointOne;
        first.point.y = pointTwo;
        first.next = oldFirst;
        size++;        //increases size every time this function is called
    }

    public String pop(){
        // If the linked list is empty, there's nothing to pop off of the stack, so it returns "empty"
        if (isEmpty())
            return "empty";

        String returnString = first.point.x + " " + first.point.y;
        first = first.next;
        size--;        //decreases size every time this function is called
        return returnString;
    }

    public boolean isEmpty() {
        return first == null;
    }

    public int size() {
        return size;
    }
}