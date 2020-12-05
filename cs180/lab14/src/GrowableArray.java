/**
 * A growable array that resizes itself as needed.
 *
 * <p>CS18000 -- Spring 2018 -- Dynamic Data Structures -- Lab</p>
 *
 * @author 
 * @version April DD, 2018
 * @param <T> the type of the values of this growable array
 */
public final class GrowableArray<T> {
    /**
     * The values of this growable array.
     */
    private Object[] values;

    /**
     * The size of this growable array. The size is equal to the number of elements successfully added minus the
     * number of elements successfully removed.
     */
    private int size;

    /**
     * Constructs a newly allocated {@code GrowableArray} object with an initial capacity of ten.
     */
    public GrowableArray() {
        this.size = 0;
        values = new Object[10];
    } //GrowableArray

    /**
     * Adds the specified value to this growable array at the specified index. The possible values at and to the right
     * of the specified index are shifted to the right by one. Size is increased by one if an exception is not thrown.
     *
     * If size is equal to the length of the values array, double the capacity of the array, leaving the original values
     * in place, before shifting the values.
     *
     * @param index the index at which the specified value will be added
     * @param value the value to be added
     * @throws IndexOutOfBoundsException if the {@code index} argument is out of bounds
     * {@code (index < 0 || index > size())}
     */
    public void add(int index, T value) throws IndexOutOfBoundsException {
        if(index < 0 || index >= size)
            throw new IndexOutOfBoundsException();
        else {
            Object[] temp = values;
            if(size() == values.length)
                values = new Object[size() * 2];
            int indexValues = 0;
            for (int i = 0; i < this.size(); i++) {
                if (i == index)
                    values[i] = value;
                else {
                    values[i] = temp[indexValues];
                    indexValues++;
                }//end else
            }//end for
            size++;
        }//end else
    } //add

    /**
     * Gets the value at the specified index in this growable array.
     *
     * @param index the index of the value to be retrieved
     * @return the value at the specified index in this growable array
     * @throws IndexOutOfBoundsException if the {@code index} argument is out of bounds
     * {@code (index < 0 || index >= size())}
     */
    @SuppressWarnings("unchecked")
    public T get(int index) throws IndexOutOfBoundsException {
        if(index < 0 || index >= size())
            throw new IndexOutOfBoundsException();
        else{
            return (T)values[index];
        }
    } //get

    /**
     * Sets the value at the specified index in this growable array. Size does not change.
     *
     * @param index the index at which the value will be set
     * @param value the value to replace the existing value
     * @return the value previously held at the specified index
     * @throws IndexOutOfBoundsException if the {@code index} argument is out of bounds
     * {@code (index < 0 || index >= size())}
     */
    public T set(int index, T value) throws IndexOutOfBoundsException {
        if(index < 0 || index >= size())
            throw new IndexOutOfBoundsException();
        else {
            Object o = values[index];
            values[index] = value;
            return (T)o;
        }
    } //set

    /**
     * Gets the index of the first occurrence of the specified object in this growable array, or {@code -1} if it could
     * not be found.
     *
     * @param object the object to be searched for
     * @return the index of the first occurrence of the specified object in this growable array, or {@code -1} if it
     * could not be found
     */
    public int indexOf(Object object) {
        for(int i = 0; i < values.length; i++){
            if(values[i] == object)
                return i;
        }
        return -1;
    } //indexOf
    
    /**
     * Removes the value at the specified index in this growable array. The possible values to the right of the
     * specified index are shifted to the left by one. Size is decreased by one if an exception is not thrown.
     *
     * @param index the index of the value to be removed
     * @return the value removed at the specified index
     * @throws IndexOutOfBoundsException if the {@code index} argument is out of bounds
     * {@code (index < 0 || index >= size())}
     */
    public T remove(int index) throws IndexOutOfBoundsException {
        Object returnMe = new Object();
        int tempIndex = 0;
        if(index < 0 || index >= size())
            throw new IndexOutOfBoundsException();
        else{
            Object[] temp = new Object[values.length];
            for(int i = 0; i < values.length; i++){
                if(i == index)
                    returnMe = values[i];
                else {
                    temp[tempIndex] = values[i];
                    tempIndex++;
                }
            }
            values = temp;
            size--;
            return (T)returnMe;
        }
    } //remove

    /**
     * Clears the values in this growable array and resets the size to 0.
     */
    public void clear() {
        for(int i = 0; i < size(); i++)
            values[i] = null;
        size = 0;
    } //clear

    /**
     * Determines whether or not this growable array is empty based on the variable size.
     *
     * @return {@code true}, if this growable array is empty, and {@code false} otherwise
     */
    public boolean isEmpty() {
        if(size == 0)
            return true;
        return false;
    } //isEmpty

    /**
     * Gets the size of this growable array. The size is equal to the number of elements successfully added minus the
     * number of elements successfully removed (which is to say, you should modify the size variable when those methods
     * are called).
     *
     * @return the size of this growable array
     */
    public int size() {
    	return size;
    } //size

    /**
     * Determines whether or not the specified object is equal to this growable array. {@code true} is returned if and
     * only if the specified object is an instance of {@code GrowableArray}, and its size and values are equal to this
     * growable array's.
     *
     * @param object the object to be compared
     * @return {@code true}, if the specified object is equal to this growable array, and {@code false} otherwise
     */
    public boolean equals(Object object) {
        if(object instanceof GrowableArray){
            if(((GrowableArray) object).size == this.size){
                for(int i = 0; i < this.size(); i++){
                    if(!(this.values[i] == (((GrowableArray) object).values[i])))
                        return false;
                }
                return true;
            }
        }
        return false;
    } //equals

    /**
     * Gets a {@code String} representation of this growable array. The returned {@code String} is of the form
     * {@code {v[0], v[1], ... , v[n]}}, where {@code v} is the values of this growable array.
     *
     * @return a {@code String} representation of this growable array
     */
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder("{");

        for (int i = 0; i < this.size - 1; i++) {
            stringBuilder.append(this.values[i]);

            stringBuilder.append(", ");
        } //end for

        if (this.size != 0) {
            stringBuilder.append(this.values[this.size - 1]);
        } //end if

        stringBuilder.append("}");

        return stringBuilder.toString();
    } //toString
}