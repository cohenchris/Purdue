import java.io.PrintStream;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Heap {

    private static final int BRANCHES = 2;

    private int[] heap;
    private int size;

    /**
     * Construct a new heap
     */
    public Heap() {
        this.size = 0;
        this.heap = new int[size];
    }

    /**
     * Insert element into this heap
     */
    public void insert(int e) {
        resize();
        heap[size() - 1] = e;
        int heapIndex = size() - 1;
        int tempVal;

        while (heap[heapIndex] < heap[heapIndex / 2]) {                 //if the node (heapIndex) is less than its parent (floor(heapIndex / 2)), then swap them.
            tempVal = heap[heapIndex / 2];
            heap[heapIndex / 2] = heap[heapIndex];
            heap[heapIndex] = tempVal;
            heapIndex = heapIndex / 2;
        }
    }

    /**
     * Return number of elements in this heap
     * @return size of the heap
     */
    public int size() {
        return size;
    }

    /**
     * Peek the smallest element in this heap
     * @throws NoSuchElementException if heap is empty
     * @return the smallest element in this heap.
     */
    public int min() {
        if (size() == 0)
            throw new NoSuchElementException();
        return heap[0];                         //the minimum element will always be the root (1st element) because, in this heap, the children are always greater than or equal to their parent
    }

    /**
     * Remove the smallest element in this heap
     * @throws NoSuchElementException if heap is empty
     * @return the smallest element in this heap
     */
    public int removeMin() {
        if (size() == 0)
            throw new NoSuchElementException();

        int minVal;

        if (size() == 1) {
            minVal = heap[0];
            heap = new int[0];
            size--;
            return minVal;
        }

        minVal = heap[0];
        heap[0] = heap[size() - 1];
        int[] newHeap = new int[size() - 1];
        for (int i = 0; i < size() - 1; i++)
            newHeap[i] = heap[i];

        heap = newHeap;
        size--;

        heapify(0);

        return minVal;
    }

    /**
     * Recursively explores the heap array and makes sure everything is in heap order.
     * @param parent
     */
    public void heapify(int parent) {
        //There's no explicit base case, even though this is a recursive function. The "base case" is making sure the left or right indices don't exceed the size() of the heap array.
        int left = (parent * 2) + 1;
        int right = (parent * 2) + 2;
        int tempVal;
        if ((size() > left) && (heap[parent] > heap[left])) {          //recursively makes sure the left subtrees are in heap order
            tempVal = heap[parent];
            heap[parent] = heap[left];
            heap[left] = tempVal;
            heapify(left);
        }

        if((size() > right) && (heap[parent] > heap[right])) {         //recursively makes sure the right subtrees are in heap order
            tempVal = heap[parent];
            heap[parent] = heap[right];
            heap[right] = tempVal;
            heapify(right);
        }
    }
    /**
     * Return a sorted array of this heap.
     * Call removeMin until all elements are removed from the heap.
     * @return a sorted array of this heap
     */
    public int[] sort() {
        int[] sortedHeap = new int[size()];
        for (int i = 0; i < sortedHeap.length; i++) {                               //to sort the heap in ascending order, all we need to do is set the next element in sortedHeap[] to removeMin() until the heap is empty.
            sortedHeap[i] = removeMin();
        }
        return sortedHeap;
    }

    /**
     * Return a boolean value that denotes whether or not the heap is empty.
     * @return a boolean value that denotes whether or not the heap is empty.
     */
    public boolean empty() {
        return size() == 0;
    }

    /**
     * Read in an input file and write output to output stream
     * Scanner in starts from the beginning of the file
     * @param in input Scanner
     * @param out output PrintStream
     */
    public static void generateOutput(Scanner in, PrintStream out) {
        in.nextLine();                                          //first line is the problem number, which we don't need to worry about in this project
        int numLines = Integer.parseInt(in.nextLine());         //second line is the number of lines to follow the current line

        String[] problemArr;
        Heap testHeap = new Heap();
        boolean somethingPrinted = false;                       //this boolean keeps track if something has been printed or not. This helps avoid stray newline characters at the beginning or end of the file.

        for (int i = 0; i < numLines; i++) {
            problemArr = in.nextLine().split(" ");

            switch(problemArr[0]) {
                case "i":                   //insert(e)         doesn't print anything!
                    testHeap.insert(Integer.parseInt(problemArr[1]));
                    break;
                case "m":                   //min()             prints the value of the returned element, or "empty" if the heap is empty
                    if (!somethingPrinted) {
                        if (testHeap.empty())
                            out.print("empty");
                        else
                            out.print(testHeap.min());
                        somethingPrinted = true;
                    }
                    else {
                        if (testHeap.empty())
                            out.print("\nempty");
                        else
                            out.print("\n" + testHeap.min());
                    }
                    out.flush();
                    break;
                case "r":                   //removeMin()       prints "empty" if the heap is empty
                    if (!somethingPrinted) {
                        if (testHeap.empty()) {
                            out.print("empty");
                            somethingPrinted = true;
                        }
                        else
                            testHeap.removeMin();
                    }
                    else {
                        if (testHeap.empty())
                            out.print("\nempty");
                        else
                            testHeap.removeMin();
                    }
                    out.flush();
                    break;
                case "s":                   //sort              //prints sorted contents of heap
                    if (testHeap.empty()) {
                        if (!somethingPrinted) {
                            out.print("empty");
                            somethingPrinted = true;
                        }
                        else
                            out.print("\nempty");
                        out.flush();
                        break;
                    }

                    int[] sortedHeap = testHeap.sort();

                    if (!somethingPrinted) {
                        out.print(sortedHeap[0]);
                        for (int j = 1; j < sortedHeap.length; j++)
                            out.print("\n" + sortedHeap[j]);
                        somethingPrinted = true;
                    }
                    else {
                        for (int j = 0; j < sortedHeap.length; j++)
                            out.print("\n" + sortedHeap[j]);
                    }
                    out.flush();
                    break;
                default:                    //none of the above commands
                    System.out.println("Error! Command character must be i, m, r, or s!");
            }
        }
    }

    private void resize() {
        int[] newHeap = new int[size() + 1];                    //we need to create an array 1 larger than the current array
        for (int i = 0; i < size(); i++)                        //copies values of heap over to newHeap exactly as they were in heap
            newHeap[i] = heap[i];
        heap = newHeap;                                         //sets heap equal to the newly resized newHeap
        size++;                                                 //make sure to increment the size since we just added a new space!
    }

    private void printHeap() {
        for (int i = 0; i < heap.length; i++)
            System.out.println(heap[i]);
        System.out.println("end heap");
    }
}
