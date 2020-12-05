import java.io.PrintStream;
import java.util.*;

/**
 * Generic Chaining Hashtable
 * @param <K> Key type
 * @param <V> Value type
 */
public class HashTable<K, V> {

    /**
     * Auxiliary class for HashTable
     * @param <F> Key type
     * @param <S> Value type
     */
    public class Pair<F, S> {
        public F key;
        public S value;

        /**
         * Construct a key value Pair
         * @throws NullPointerException if key is null
         * @param key key
         * @param value value
         */
        public Pair(F key, S value) {
            if(key == null) {
                throw new NullPointerException("Key cannot be null");
            }
            this.key = key;
            this.value = value;
        }

        @Override
        public boolean equals(Object other) {
            if(! (other instanceof Pair)) {
                return false;
            }
            @SuppressWarnings("unchecked")
            Pair<?, ?> po = (Pair) other;
            // Hack here. We only compare key value
            // so that we can use it to call List.contains
            return key.equals(po.key);
        }
    }

    private static final int[] primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271};

    private List<List<Pair<K, V>>> table;
    private int size;
    private int capacity;

    /**
     * Initialize an array of length capacity,
     * then initialize the table with empty Lists at each index.
     * @param capacity Initial capacity of the table.
     */
    public HashTable(int capacity) {
        this.capacity = capacity;
        this.size = 0;
        this.table = new ArrayList<>(capacity);

        for (int i = 0; i < capacity; i++)
            table.add(new ArrayList<>());
    }

    /**
     * Get the hash code of key.
     * You may design any hashCode function you like.
     * @param key key
     * @return hashCode of key
     */
    private int hashCode(K key) {
        if (key instanceof String) {                //if the key is a String, we need to make sure anagrams are hashed to the same value.
            String keyString = (String)key;
            int hash = 1;
            for (int i = 0; i < keyString.length(); i++)
                hash *= (int)(keyString.charAt(i)); //multiplying every character's int value together will ensure that anagrams are hashed to the same value.
            return hash;
        }
        else                                        //if the key isn't a String, we don't need to worry about any special cases.
            return (int)key;
    }

    /**
     * Returns the value of the key if found, return null otherwise.
     * @param key key
     * @throws NullPointerException if key is null
     * @return Returns the value of the key if found. Otherwise, return null.
     */
    public V get(K key) {
        if (key == null)
            throw new NullPointerException();

        int hashIndex = hashCode(key) % capacity;

        for (Pair<K, V> pair: table.get(hashIndex)) {
            if (pair.key.equals(key))                           //if we run across a Pair whose key equals the key argument, return that Pair's value since it's the Pair we were looking for.
                return pair.value;
        }
        return null;                                            //if the program gets to this point, a Pair with the key argument has not been found, so we return null to signify that we haven't found anything.
    }

    /**
     * Adds the (key, value) pair to the table.
     * If key already presents in the hashtable, overwrite the original value.
     * Resize the hashtable if the load factor becomes greater than 0.5 after inserting this (key, value) pair.
     * @throws NullPointerException if key or value is null
     * @param key key
     * @param value value
     * @return the previous value of the specified key in this hashtable, or null if it did not have one
     */
    public V put(K key, V value) {
        if (key == null || value == null)
            throw new NullPointerException();

        V prevVal = replace(key, value);
        if (prevVal == null) {                                  //if prevVal is null, that means that the replace() method didn't find a Pair with a matching key, so we must add a new Pair to the hash table.
            int hashIndex = hashCode(key) % capacity;
            table.get(hashIndex).add(new Pair<>(key, value));   //this line adds a new element to the collision chain using the key, value Pair that was passed in.
            size++;                                             //if we end up putting a brand new value in, rather than replacing one, the size will increment by 1.
        }

        if (((double)size()) / ((double)capacity) > 0.5)        //if the load factor is greater than 0.5, resize()
            resize();

        return prevVal;                                         //if there was a previous value, replace() would have returned it, and that previous value will be returned. If there wasn't, replace() will have returned null, so null will be returned here.
    }

    /**
     * Resize the hashtable if the load factor becomes greater than 0.5 after inserting this (key, value) pair.
     * We hard code a list of prime number for you to use. You can assume you will never run out of prime number to use.
     */
    private void resize() {
        //FINDING NEW CAPACITY
        int nextPrime = 0;
        int primeIndex = 0;
        while (nextPrime <= capacity * 2) {                 //simply finds the lowest prime number that is at least twice the current capacity
            nextPrime = primes[primeIndex];
            primeIndex++;
        }
        int newCapacity = nextPrime;
        capacity = newCapacity;                             //make sure to set capacity equal to the newly calculated capacity variable


        List<List<Pair<K, V>>> oldTable = table;

        table = new ArrayList<>(capacity);                  //this line and the next two lines completely resets the table variable so we can re-hash it.
        for (int i = 0; i < newCapacity; i++)
            table.add(new ArrayList<>());

        for (List<Pair<K, V>> list : oldTable) {
            for (Pair<K, V> pair : list) {
                size--;                                     //this is to negate the call to put(). If this were not here, the size would increase even when we're not adding additional Pairs.
                put(pair.key, pair.value);
            }
        }
    }

    /**
     * Removes the key from the table, if it's there.
     * @throws NullPointerException if the key is null
     * @param key key
     * @return Return the value of the key if key exists in the hashtable, return null otherwise
     */
    public V remove(K key) {
        if (key == null)
            throw new NullPointerException();


        int hashIndex = hashCode(key) % capacity;

        for (int i = 0; i < getCollision(hashIndex); i++) {             //this loop iterates through the collision chain based on how long it is.
            if (table.get(hashIndex).get(i).key.equals(key)) {          //if, at any point, the inspected Pair matches up with the key argument, we have found the value to remove.
                V prevVal = table.get(hashIndex).get(i).value;
                table.get(hashIndex).remove(i);                         //the ArrayList.remove() method removes the value from the ArrayList without disrupting the integrity of the rest of the ArrayList.
                size--;                                                 //if we find a value to remove, the size will decrease by 1.
                return prevVal;
            }
        }
        return null;
    }

    /**
     * Returns whether the key is in the hashtable.
     * @throws NullPointerException if key is null
     * @param key key
     * @return Return true if the key is in the hashtable. Return false otherwise.
     */
    public boolean containsKey(K key) {
        if (key == null)
            throw new NullPointerException();

        for (List<Pair<K, V>> list : table)                             //nested for-each loops to iterate through each and every value in the table (saves headache with indices).
            for (Pair<K, V> pair : list)
                if (pair.key.equals(key))                               //if we find a Pair whose key equals the key argument, the Hash Table DOES contain the key.
                    return true;
        return false;
    }

    /**
     * Returns the number of (key, value) pairs in the table.
     * @return size of hashtable
     */
    public int size() {
        return size;                                                    //size is updated when adding or removing Pairs from the hash table.
    }

    /**
     * Replaces the value for the specified key only if it is currently mapped to some value.
     * @throws NullPointerException if key is null
     * @param key key
     * @param value value
     * @return Return the previous value associated with the specified key, or null if there was no mapping for the key.
     */
    public V replace(K key, V value) {
        if (key == null)
            throw new NullPointerException();

        if (!containsKey(key))                                           //this function only fully executes if there is a Pair with a key that matches the key argument.
            return null;

        int hashIndex = hashCode(key) % capacity;
        for (Pair<K, V> pair : table.get(hashIndex)) {
            if (pair.key.equals(key)) {                                 //if we find a Pair whose key matches the argument key, we must replace that Pair's value with the argument value.
                V prevVal = pair.value;
                pair.value = value;
                return prevVal;
            }
        }
        return null;
    }

    /**
     * Return the chain length at index in this hashtable.
     * @param index index of the chain to get length of
     * @return chain length at index in this hashtable.
     */
    public int getCollision(int index) {
        return table.get(index).size();                                 //number of collisions is defined as the chain length, so we return the size of the ArrayList that is contained in a certain index.
    }

    /**
     * Read in an input file and write output to output stream
     * Scanner in starts from the beginning of the file
     * @param capacity the initial capacity of hash table.
     * @param in input Scanner
     * @param out output PrintStream
     */

    public static void generateOutput(int capacity, Scanner in, PrintStream out) {
        in.nextLine();
        int numLines = Integer.parseInt(in.nextLine());


        String[] problemArr = new String[numLines];
        for (int i = 0; i < numLines; i++)                                  //reads in the entire input file and
            problemArr[i] = in.nextLine();


        //OUTPUT
        String[] inputLine;                                                 //String array to split each line into separately manageable inputs.
        HashTable<String, Integer> testTable = new HashTable<>(capacity);   //Hash Table created to keep track of commands in input file

        for (int i = 0; i < numLines; i++) {
            inputLine = problemArr[i].split(" ");                       //splits the line into an array delimited by spaces

            switch (inputLine[0]) {
                //the "if (i == problemArr.length - 1)" statements are to detect if we are currently on the last line of output. If so, no newline character is printed.
                //this would potentially break and print a newline character if the last line read in isn't valid, but we are able to assume that input is completely correct.
                case "p":       //put(key, value)
                    if (i == problemArr.length - 1)
                        out.print(testTable.put(inputLine[1], Integer.parseInt(inputLine[2])));
                    else
                        out.println(testTable.put(inputLine[1], Integer.parseInt(inputLine[2])));
                    out.flush();
                    break;
                case "g":       //get(key)
                    if (i == problemArr.length - 1)
                        out.print(testTable.get(inputLine[1]));
                    else
                        out.println(testTable.get(inputLine[1]));
                    out.flush();
                    break;
                case "r":       //replace(key, value)
                    if (i == problemArr.length - 1)
                        out.print(testTable.replace(inputLine[1], Integer.parseInt(inputLine[2])));
                    else
                        out.println(testTable.replace(inputLine[1], Integer.parseInt(inputLine[2])));
                    out.flush();
                    break;
                case "d":       //remove(key)
                    if (i == problemArr.length - 1)
                        out.print(testTable.remove(inputLine[1]));
                    else
                        out.println(testTable.remove(inputLine[1]));
                    out.flush();
                    break;
                case "s":       //size()
                    if (i == problemArr.length - 1)
                        out.print(testTable.size());
                    else
                        out.println(testTable.size());
                    out.flush();
                    break;
                case "c":       //containsKey(key)      1 if TRUE, 0 if FALSE
                    if (i == problemArr.length - 1) {
                        if (testTable.containsKey(inputLine[1]))
                            out.print("1");
                        else
                            out.print("0");
                    }
                    else {
                        if (testTable.containsKey(inputLine[1]))
                            out.println("1");
                        else
                            out.println("0");
                    }
                    out.flush();
                    break;
                case "m":       //getCollision(index)
                    if (i == problemArr.length - 1)
                        out.print(testTable.getCollision(Integer.parseInt(inputLine[1])));
                    else
                        out.println(testTable.getCollision(Integer.parseInt(inputLine[1])));
                    out.flush();
                    break;
                default:        //if this executes, the command entered is not valid
                    System.out.println("Error! Command letter must be either p, g, r, d, s, c, or m!");
            }
        }
    }

    /**
     * Print out (this) hash table to the console.
     */
    public void printTable() {
        for (int i = 0; i < capacity; i++) {
            System.out.print("[");
            for (int j = 0; j < getCollision(i); j++) {
                System.out.print("(" + table.get(i).get(j).key + " " + table.get(i).get(j).value + ")");
            }
            System.out.println("]");
        }
        System.out.println("end table");
    }
}
