import java.util.Comparator;
import java.util.HashMap;
import java.util.ArrayList;

public class Rhymer implements Comparator {

    String[] words;

    public Rhymer(String[] wordArr) {
        this.words = wordArr;
    }  //Rhymer Constructor

    /**
     * This method quicksorts the 'words' array using a completely random pivot
     * @param words array to quicksort
     * @param startIndex index in the first half of the array, helps find elements that need to be moved
     * @param endIndex index in the latter half of the array, helps find elements that need to be moved
     */
    public void quickSort(String[] words, int startIndex, int endIndex) {
        if (endIndex <= startIndex) {                                   //base case
            return;
        }
        int pivot = (int) (Math.random() * (endIndex - startIndex)) + startIndex;       //completely random pivot (between startIndex and endIndex)

        int rightPtr = endIndex - 1;                                                    //rightPtr starts on the far right
        int leftPtr = startIndex;

        while (rightPtr > leftPtr) {
            rightPtr = endIndex - 1;                                                    //makes sure rightPtr and leftPtr are reset every time to avoid infinite loops (when leftPtr = rightPtr)
            leftPtr = startIndex;

            while (compare(words[rightPtr], words[pivot]) > 0) {                        //find the first element, starting from the right, that needs to be to the left side of the pivot (less than the pivot)
                rightPtr--;
            }
            while (compare(words[leftPtr], words[pivot]) < 0) {                         //find the first element, starting from the left, that needs to be on the right side of the pivot (greater than the pivot)
                leftPtr++;
            }

            if (leftPtr <= rightPtr) {                                                  //If this is the case, the leftPtr and rightPtr need to be swapped
                String temp = words[leftPtr];                                           //If this wasn't the case, and the leftPtr is greater than the rightPtr, they are already in the spot where they need to be.
                words[leftPtr] = words[rightPtr];
                words[rightPtr] = temp;
            }
        }

        quickSort(words, startIndex, pivot);                                            //recursively quickSort() left side
        quickSort(words, pivot + 1, endIndex);                                //recursively quickSort() right side
    } //quickSort()

    /**
     * This method iteratively traverses a word array and find the suffixes that they share, storing each suffix and each corresponding word in a hash table.
     * @param words array to sort by suffixes
     * @param longestIndex index of the longest word in the array
     * @return a HashMap with all of the suffixes and corresponding words
     */
    public HashMap<String, ArrayList<String>> suffixShare(String[] words, int longestIndex) {
        int numRead = 0;                                                                //number of array elements read so far
        int start = 0;                                                                  //starting index for the blocks of words
        int end = 0;                                                                    //ending index for the blocks of words
        int maxIndex = 0;                                                               //index for the word with the most letters in the current block
        HashMap<String, ArrayList<String>> hashTable = new HashMap<>();                 //HashMap to store suffixes and their values

        for (int i = 1; i <= words[longestIndex].length(); i++) {                                       //this loop makes sure every suffix of the list is iterated through
            for (int j = 0; j < words.length; j++) {                                    //this loop makes sure every word in the list is iterated through
                if ((i <= words[j].length())) {                                         //if the suffix substring index is less than or equal to the current word's length, it won't cause an out of bounds error
                    String putKey = words[j].substring(words[j].length() - i);
                    if (!hashTable.containsKey(putKey)) {
                        hashTable.put(putKey, new ArrayList<>());                       //if the suffix isn't already in the table, declare a new ArrayList at that point in the table so it can handle collisions
                        hashTable.get(putKey).add(words[j]);                            //puts the word into the hash table
                    }
                    else {
                        hashTable.get(putKey).add(words[j]);                            //puts the word into the hash table
                    }
                }
            }
        }
        return hashTable;
    } //suffixShare()

    /**
     * This method determines if a given HashMap contains 'value'.
     * @param hashMap HashMap to search in
     * @param value value that we would like to find in hashMap
     * @return
     */
    public boolean hasValue(HashMap<Integer, ArrayList<String>> hashMap, String value) {
        for (int i = 0; i < hashMap.size(); i++) {
            if(hashMap.get(i) != null) {
                for (int j = 0; j < hashMap.get(i).size(); j++) {
                    if (hashMap.get(i).get(j).equals(value)) {
                        return true;
                    }
                }
            }
        }
        return false;
    } //hasValue()

    /**
     * This method computes a hash code based on 's'.
     * @param s String to compute hash code off of
     * @return integer hash code for 's'
     */
    public int hashCode(String s) {                       //returns an integer representation of the ascii values of the argument concatenated
        String finalVal = "";
        for (int i = 0; i < s.length(); i++) {
            finalVal += Integer.toString((int)s.charAt(i));
        }
        return Integer.parseInt(finalVal.substring(finalVal.length() - (finalVal.length() / 2)));                       //change this afterwards
    }

    /**
     * This method compares two objects by casting them to strings and comparing each character.
     * @param o1 object 1 (String 1)
     * @param o2 object 2 (String 2)
     * @return 1 if s1 > s2, -1 if s1 < s2, 0 if s1 = s2
     * @throws NullPointerException
     */
    @Override
    public int compare(Object o1, Object o2) throws NullPointerException{
        if (o1 == null || o2 == null) {
            throw new NullPointerException();
        }

        String s1 = (String)o1;
        String s2 = (String)o2;
        int minLength = (s1.length() < s2.length()) ? s1.length():s2.length();          //minLength is the minimum length of the two strings

        for (int i = 0; i < minLength; i++) {
            if (s1.charAt(i) < s2.charAt(i)) {                                          //if, at any point, the s1's char at index 1 is less than s2's char at index 1, that means s1 is "less than" s2, so return -1
                return -1;
            }
            else if (s1.charAt(i) > s2.charAt(i)) {
                return 1;                                                               //if, at any point, the s1's char at index 1 is greater than s2's char at index 1, that means s1 is "greater than" s2, so return 1
            }
        }

        if (s1.length() > s2.length()) {                                                //if the method has gotten here, that means if s1's length is greater than s2's, s1 is "greater than" s2, so return 1.
            return 1;
        }

        else if (s1.length() < s2.length()) {                                           //if the method has gotten here, that means if s1's length is less than s2's, s1 is "less than" s2, so return -1.
            return -1;
        }
        else {                                                                          //if the method has gotten here and the lengths are the same, s1 and s2 are the same String, so return 0.
            return 0;
        }
    } //compare()

    /**
     * This method iterates through a String array and prints out every element.
     * @param arr array to iterate through
     */
    public void printArray(String[] arr) {
        System.out.print("[");
        for (int i = 0; i < arr.length; i++) {
            if (i == arr.length - 1) {
                System.out.println(reverseWord(arr[i]) + "]");
            }
            else {
                System.out.print(reverseWord(arr[i]) + ", ");
            }
        }
    } //printArray()

    /**
     * This method reverses a String 's'.
     * @param s String to reverse
     * @return reversed version of 's'
     */
    public String reverseWord(String s) {
        String newString = "";
        for (int i = s.length() - 1; i >= 0; i--) {
            newString += s.charAt(i);
        }
        return newString;
    } //reverseWord()
}