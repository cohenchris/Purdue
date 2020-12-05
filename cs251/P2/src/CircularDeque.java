//implement circular deque here

public class CircularDeque {
    private int frontIndex = -1;
    private int rearIndex = -1;
    private int queueSize = 2;
    private String[] queue = new String[2];

    public void enqueueFront(String newItem) {
        if (isEmpty()) {                                        //if the list is empty, set the front and rear indices appropriately
            frontIndex = 0;
            rearIndex = 0;
            queue[frontIndex] = newItem;
            return;
        }

        if (isFull()) {                                         //if the list is full, double the size and rearrange the elements
            resize();
        }

        if (frontIndex == 0)                                    //if the front index is originally 0, the new front index is the last element of the array
            frontIndex = queueSize - 1;
        else                                                    //otherwise, new front index is the previous index
            frontIndex--;
        queue[frontIndex] = newItem;
    }//enqueueFront()

    public void enqueueBack(String newItem) {
        if (isEmpty()) {                                        //if the list is empty, set the front and rear indices appropriately
            frontIndex = 0;
            rearIndex = 0;
            queue[rearIndex] = newItem;
            return;
        }

        if (isFull()) {                                         //if the list is full, double the size and rearrange the elements
            resize();
        }

        if (rearIndex == queueSize - 1)                         //if the rear index is originally the last element of the array, the new rear index is 0 (first element of array)
            rearIndex = 0;
        else                                                    //otherwise, new rear index is the next element.
            rearIndex++;
        queue[rearIndex] = newItem;
    }//enqueueBack()

    public String dequeueFront() {
        if (isEmpty()) {                                        //if the list is empty, there is nothing to dequeue, so the String "empty" is returned.
            return "empty";
        }

        String removed = queue[frontIndex];
        queue[frontIndex] = null;

        if (frontIndex == queueSize - 1)                        //if the original front index is the last element of the array, the new index is the first element of the array.
            frontIndex = 0;
        else                                                    //otherwise, the new front index is the previous element
            frontIndex++;
        return removed;
    }//dequeueFront()

    public String dequeueBack() {
        if (isEmpty()) {                                        //if the list is empty, there is nothing to dequeue, so the  String "empty" is returned.
            return "empty";
        }

        String removed = queue[rearIndex];
        queue[rearIndex] = null;

        if (rearIndex == 0)                                     //if the rear index is originally the first element of the array, the new rear index is the last element of the array
            rearIndex = queueSize - 1;
        else                                                    //otherwise, the new rear index is the previous element
            rearIndex--;
        return removed;
    }//dequeueBack()

    public int getQueueSize() { return queueSize; }//getQueueSize()

    public int getFrontIndex() { return frontIndex; }//getFrontIndex()

    public int getRearIndex() { return rearIndex; }//getRearIndex()

    public boolean isFull() {
        for (int i = 0; i < queueSize; i++)
            if (queue[i] == null)
                return false;
        return true;
    }//isFull()

    public boolean isEmpty() {
        for (int i = 0; i < queue.length; i++) {
            if (queue[i] != null)
                return false;
        }

        frontIndex = -1;                                        //if the array IS empty, the front and rear indices should be reset
        rearIndex = -1;
        return true;
    }//isEmpty()

    public void resize() {
        String[] newArr = new String[queueSize * 2];
        int index = 0;

        //Reordering queue where frontIndex is 0 and backIndex is the last element
        if (rearIndex >= frontIndex) {
            for (int i = frontIndex; i <= rearIndex; i++) {
                newArr[index] = queue[i];
                index++;
            }
        }

        else {
            for (int i = frontIndex; i < queueSize; i++) {
                newArr[index] = queue[i];
                index++;
            }

            for (int i = 0; i <= rearIndex; i++) {
                newArr[index] = queue[i];
                index++;
            }
        }

        queueSize *= 2;
        frontIndex = 0;
        rearIndex = (queueSize / 2) - 1;

        queue = newArr;
    }//resize()
}
