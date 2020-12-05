
public class BinarySearchTree {
    public Node root;

    public BinarySearchTree() {
        this.root = null;
    }//BinarySearchTree Constructor

    /**
     * This method calls the recursive insertNode() method and sets root equal to the updated tree.
     * @param key the key to insert into the tree
     */
    public void insertKey(int key) {
        root = insertNode(root, key);
    } //insertKey()

    /**
     * This method recursively traverses the tree until a sufficient insertion spot is found for 'key'.
     * @param currentNode the node currently being considered for insertion of 'key'
     * @param key the key to insert into the tree
     * @return the updated tree with newly inserted key
     */
    public Node insertNode(Node currentNode, int key) {
        if (currentNode == null) {                             //recursively, if a null node is reached, we have found the spot to insert
            currentNode = new Node(key);
            return currentNode;
        }
        else if (currentNode.getKey() < key) {                 //if the current node's key is less than what we need, that means that the key that we're looking for HAS to be in the right subtree
            currentNode.right = insertNode(currentNode.right, key);
        }
        else {                                          //if the current node's key is greater than what we need, that means that the key that we're looking for HAS to be in the left subtree
            currentNode.left = insertNode(currentNode.left, key);
        }
        return currentNode;
    } //insertNode()

    /**
     * This method updates the tree by calling the recursive delete method.
     * @param key the key that needs to be deleted from the tree
     * @return whether or not the key was deleted
     */
    public String deleteKey(int key) {
        String returnVal = searchKey(key) ? "deleted" : "deletion failed";
        root = recursiveDelete(root, key);
        return returnVal;
    } //deleteKey()

    /**
     * This method recursively traverses the tree until the node to be deleted is found, then it is deleted.
     * @param currentNode the node currently being considered for deletion
     * @param key the key that needs to be deleted from the tree
     * @return the updated tree after 'key' has been deleted
     */
    private Node recursiveDelete(Node currentNode, int key) {
        if (currentNode == null) {                                                      //base case
            return null;
        }

        //recursively travels the tree to find the node whose key is equal to the passed in argument 'key'
        if (currentNode.getKey() < key) {
                currentNode.right = recursiveDelete(currentNode.right, key);
        }
        else if (currentNode.getKey() > key) {
                currentNode.left = recursiveDelete(currentNode.left, key);
        }

        else {
            if (currentNode.left == null && currentNode.right == null) {                //no children (leaf node)
                return null;                                                            //returns null because the node that will "replace" the one that was deleted will be null
            }
            else if (currentNode.left == null && currentNode.right != null) {           //one child (right child)
                return currentNode.right;                                               //node that's replacing the deleted one will be the right node since it's not null
            }
            else if (currentNode.left != null && currentNode.right == null) {           //one child (left child)
                return currentNode.left;                                                //node that's replacing the deleted one will be the left node since it's not null
            }
            else {                                                                      //two children
                Node node = currentNode;                //temporary node variable to not disturb currentNode's value

                //finds the minimum value in the right subtree of currentNode
                Node rightNode = node.right;
                if (rightNode.left != null) {
                    while (rightNode.left != null) {
                        rightNode = rightNode.left;
                    }
                }

                currentNode.setKey(rightNode.getKey());                                         //sets the key of the currentNode to the lowest key in the right subtree

                currentNode.right = recursiveDelete(currentNode.right, currentNode.getKey());   //since we replaced the currentNode's key with a key already in the tree, we need to delete it. We travel right since it's guaranteed to be to the right
            }
        }
        return currentNode;
    } //recursiveDelete()

    /**
     * This method searches the current tree starting with 'root' to see if it exists
     * @param key the key that we want to find in the tree
     * @return whether the key is found (true) or not (false)
     */
    public boolean searchKey(int key) {
        Node currentNode = root;
        while(currentNode != null) {                    //if currentNode every equals null, we know that the key doesn't exist in the tree, so skip down to bottom and return false.
            if (currentNode.getKey() == key) {          //if currentNode's key is equal to the parameter 'key', we found it!
                return true;
            }
            else if (currentNode.getKey() > key) {      //if currentNode's key is greater than the key we want, the key we want HAS to be in the left subtree.
                currentNode = currentNode.left;
            }
            else {                                      //implied that the currentNode's key is less than the key we want, so the key we want HAS to be in the right subtree.
                currentNode = currentNode.right;
            }
        }
        return false;
    } //searchKey()

    /**
     * This method calls the recursive rangeTraverse() function.
     * @param left lower bound of the range that we want a sum from
     * @param right upper bound of the range that we want a sum from
     * @return the sum of all numbers in the tree between 'left' and 'right'.
     */
    public int rangeSum(int left, int right) {
        return rangeTraverse(root, left, right);
    } //rangeSum()

    /**
     * This method recursively traverses the tree and keeps track of a sum of keys that are in between 'left' and 'right'.
     * @param currentNode the current node that's considered being added to the total sum
     * @param left lower bound of the range that we want a sum from
     * @param right upper bound of the range that we want a sum from
     * @return the sum of all numbers in the tree between 'left' and 'right'.
     */
    private int rangeTraverse(Node currentNode, int left, int right) {
        int sum = 0;
        if (currentNode == null) {                                                  //base case
            return sum;
        }
        if ((currentNode.getKey() <= right) && (currentNode.getKey() >= left)) {    //this if statement checks to see if the currentNode's key is in the valid range
            sum += currentNode.getKey();
        }
        sum += rangeTraverse(currentNode.left, left, right);                        //recursively travels down the left subtree and adds anything it finds down there
        sum += rangeTraverse(currentNode.right, left, right);                       //recursively travels down the right subtree and adds anything it finds down there

        return sum;
    } //rangeTraverse()

    /**
     * This method iteratively travels down the tree to find the height of the passed in 'key'.
     * @param key the key whose height that we want to find in the tree
     * @return the height of 'key' in the tree
     */
    public String height(int key) {
        if ((searchKey(key)) && (root != null)) {
            Node currentNode = root;
            int height = 0;
            while (true) {                                      //an infinite loop is ok here because, with the previous if statement, we have guaranteed that the value will eventually be found.
                if (currentNode.getKey() == key) {              //if we've found the correct node we're done!
                    return Integer.toString(height);
                } else if (currentNode.getKey() > key) {        //if the current node's key is greater than the key we want, the key we want HAS to be in the left subtree.
                    currentNode = currentNode.left;
                    height++;
                } else {                                        //implied that the current node's key is less than the key we want, so the key we want HAS to be in the left subtree.
                    currentNode = currentNode.right;
                    height++;
                }
            }
        }
        return "none";                                          //if the key isn't in the tree, this method will automatically return "none".
    } //height()

    public int treeHeight(Node currentNode) {
        if (currentNode == null) {                              //base case
            return 0;
        }

        int leftHeight = treeHeight(currentNode.left);          //recursively travel down the left subtree
        int rightHeight = treeHeight(currentNode.right);        //recursively travel down the right subtree

        if (rightHeight < leftHeight) {
            return leftHeight + 1;
        }
        else {
            return rightHeight + 1;
        }

    } //treeHeight()
    /**
     * This method calls the recursivePostorder method, starting at the root.
     * @return "none" if the tree is empty, or the postorder traversal if not
     */
    public String postorder() {
        return (recursivePostorder(root).length() == 0) ? "none" : recursivePostorder(root);
    } //postorder()
    /**
     * This method recursively traverses the tree in postorder fashion (left-right-node method).
     * @param currentNode the current node being looked at
     * @return a String representation of the postorder traversal
     */
    public String recursivePostorder(Node currentNode) {
        String returnVal = "";
        if (currentNode == null) {                                     //base case
            return returnVal;
        }

        returnVal += recursivePostorder(currentNode.left);      //since postorder is LRN, start by going all the way to the left
        returnVal += recursivePostorder(currentNode.right);     //then, go all the way to the right
        returnVal += currentNode.getKey() + " ";
        return returnVal;
    } //recursivePostorder()

    /**
     * This method calls the recursiveLevelorder method, starting at the root.
     * @return "none" if the tree is empty, or the levelorder traversal if not
     */
    public String levelorder() {
            String returnVal = "";

            for (int i = 0; i < treeHeight(root); i++) {        //this helps print each level of the tree from the top down
                returnVal += recursiveLevelorder(root, i);
            }
            return (returnVal.equals("") ? "none" : returnVal); //if nothing was ever appended to returnVal, print "none", since there was nothing in the tree. otherwise, print returnVal as normal.
    } //levelorder()

    /**
     * This method recursively traverses the tree in breadth first (level order) fashion. If the tree were
     * printed out, it would print each horizontal level left to right.
     * @param currentNode the current node being looked at
     * @param height the countdown until a node should be printed - decreases when we go down a level
     * @return a String representation of the level order traversal
     */
    public String recursiveLevelorder(Node currentNode, int height) {
        String returnVal = "";
        if (currentNode == null || height < 0) {                                     //base case
            return returnVal;
        }

        if (height == 0) {                                                           //if height is 0, that means that we are at the node to print
            returnVal += currentNode.getKey() + " ";
        }
        else {                                                                       //otherwise, recursively traverse the tree in either direction, decreasing 'height' as you go down each level
            returnVal += recursiveLevelorder(currentNode.left, height - 1);
            returnVal += recursiveLevelorder(currentNode.right, height - 1);
        }
        return returnVal;
    } //recursiveLevelorder()

    /**
     * This method iteratively traverses the tree and finds the least common ancestor of key1 and key2
     * @param key1 the first key to find the ancestor of
     * @param key2 the second key to find the ancestor of
     * @return the key value of the lowest common ancestor
     */
    public int LCA(int key1, int key2) {
        if (!searchKey(key1) || !searchKey(key2)) {                                     //if either key isn't in the tree, we can't successfully exeute this method, so return MIN_VALUE to denote an error
            return Integer.MIN_VALUE;
        }

        Node currentNode = root;
        while (currentNode != null) {
            if ((currentNode.getKey() > key1) && (currentNode.getKey() > key2)) {       //if the currentNode is greater than both keys, that means both keys are on the left subtree, and currentNode IS a common ancestor, but not the LOWEST common ancestor
                currentNode = currentNode.left;
            }
            else if ((currentNode.getKey() < key1) && (currentNode.getKey() < key2)){   //if the currentNode is less than both keys, that means both keys are on the right subtree, and currentNode IS a common ancestor, but not the LOWEST common ancestor
                currentNode = currentNode.right;
            }
            else {                                                                      //if the currentNode is less than one key and greater than the other (or one is equal), that means the keys are on either side of it, and that will be the lowest it can go
                break;
            }
        }
        return (currentNode == null) ? Integer.MIN_VALUE : currentNode.getKey();        //if currentNode is null, print MIN_VALUE to denote that there is no LCA. otherwise, print currentNode's key like normal.
    } //LCA()

    /**
     * This method iteratively traverses the tree, searching for the floor of the passed in argument.
     * @param key the upper limit of what we can return
     * @return the highest value that's lower than the passed in 'key' argument (unless 'key' is in the tree, in which case 'key' is returned)
     */
    public String Floor(int key) {
        if (searchKey(key)) {                                                           //if the key is in the tree, return it to make a better best case for this method
            return Integer.toString(key);
        }
        Node currentNode = root;
        int floorVal = Integer.MIN_VALUE;                                               //floorVal starts at MIN_VALUE to make it easy to keep track of whether or not the floorVal was changed
        while (currentNode != null) {
            if (currentNode.getKey() > key) {                                           //if the currentNode's key is greater than the key we've been given, we know a better floorVal can only be to the left
                currentNode = currentNode.left;
            }
            else if (currentNode.getKey() < key) {                                      //if the currentNode's key is less than the key we've been given, it is a possible candidate for the final floorVal
                floorVal = currentNode.getKey();
                currentNode = currentNode.right;                                        //if there is an even better floorVal, it will be to the right
            }
        }
        return (floorVal == Integer.MIN_VALUE) ? "none" : Integer.toString(floorVal);
    } //Floor()

    /**
     * This method iteratively traverses the tree, searching for the ceiling of the passed in argument.
     * @param key the lower limit of what we can return
     * @return the lowest value that's higher than the passed in 'key' argument (unless 'key' is in the tree, in which case 'key' is returned)
     */
    public String Ceil(int key) {
        if (searchKey(key)) {                                                           //if the key is in the tree, return it to make a better best case for this method
            return Integer.toString(key);
        }
        Node currentNode = root;
        int ceilVal = Integer.MAX_VALUE;                                                //ceilVal starts at MAX_VALUE to make it easy to keep track of whether or not the ceilVal was changed
        while (currentNode != null) {
            if (currentNode.getKey() > key) {                                           //if the currentNode's key is greater than the key we've been give, it is a possible candidate for the final ceilVal
                ceilVal = currentNode.getKey();
                currentNode = currentNode.left;                                         //if there is an even better ceilVal, it will be to the left
            }
            else if (currentNode.getKey() < key) {                                      //if the currentNode's key is less than the key we've been give, we know that a better ceilVal can only be to the right
                currentNode = currentNode.right;
            }
        }
        return (ceilVal == Integer.MAX_VALUE) ? "none" : Integer.toString(ceilVal);
    } //Ceil()

    /**
     *This method finds the distance between 2 nodes using the formula "dist(a, b) = dist(root, a) + dist(root, b) - 2*dist(root, LCA(a, b))".
     * @param a lower node
     * @param b upper node
     * @return the distance between a and b
     */
    public String dist(int a, int b) {
        if (!searchKey(a) || !searchKey(b)) {                                           //if either key isn't in the tree, we can't continue
            return "none";
        }

        //dist(a, b) = dist(root, a) + dist(root, b) - 2*dist(root, LCA(a, b))

        int distToA = Integer.parseInt(height(a));
        int distToB = Integer.parseInt(height(b));
        int LCA = LCA(a, b);
        int distToLCA = Integer.parseInt(height(LCA));
        return Integer.toString(distToA + distToB - 2*distToLCA);
    } //dist()

    /**
     * This method finds the node with corresponding 'key'.
     * @param key key of the node that we are searching for
     * @return the Node that has key value 'key'
     */
    public Node getNode(int key) {
        if (searchKey(key)) {
            Node currentNode = root;
            while (currentNode != null) {
                if (currentNode.getKey() > key) {
                    currentNode = currentNode.left;
                }
                else if (currentNode.getKey() < key) {
                    currentNode = currentNode.right;
                }
                else {
                    return currentNode;
                }
            }
        }
        return null;
    } //getNode()

    ////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////RED-BLACK TREE/////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * This method checks to see whether 'currentNode' is red
     * @param currentNode node to check
     * @return true if the node is red, false otherwise
     */
    public boolean red(Node currentNode) {
        if (currentNode != null) {
            return (currentNode.getColor() == Color.RED);
        }
        return false;
    }

    /**
     * This method is here to call insertRBmethod. It cleans up the Main file by removing the need to set root equal to the output (we do it here instead)
     * @param key the key to insert into the tree
     */
    public void insertRB(int key) {
        root = insertRBmethod(root, key);
        root.setColor(Color.BLACK);
    } //insertRB()

    /**
     * This method inserts a node with key value 'key' into the tree
     * @param currentNode
     * @param key the key to insert into the table
     * @return the new representation of the red-black tree with newly inserted element
     */
    public Node insertRBmethod(Node currentNode, int key) {
        if (currentNode == null) {
            return new Node(key);
        }

        //recursively searches through the tree
        if (currentNode.getKey() > key) {
            currentNode.left = insertRBmethod(currentNode.left, key);
        }
        else if (currentNode.getKey() < key) {
            currentNode.right = insertRBmethod(currentNode.right, key);
        }
        else {
            //return currentNode;
            currentNode.setKey(key);
        }

        //The fact that this method is recursive eliminates the need to manually travel back up the tree to check for red-black correctness.
        //The recursion automatically checks every single node on the way back up the tree (once the final node is reached and returned)


        if (!red(currentNode.left) && red(currentNode.right)) {               //Right child is red, left child is black/null
            currentNode = rotateLeft(currentNode.getKey());
        }
        if (red(currentNode.left) && red(currentNode.left.left)) {       //Left child is red, left grandchild is red
            currentNode = rotateRight(currentNode.getKey());
        }
        if (red(currentNode.left) && red(currentNode.right)) {           //Left and right children are red
            //invert color of currentNode
            if (currentNode.getColor() == Color.BLACK) {
                currentNode.setColor(Color.RED);
            }
            else {
                currentNode.setColor(Color.BLACK);
            }

            currentNode.left.setColor(Color.BLACK);             //set left child of currentNode to black
            currentNode.right.setColor(Color.BLACK);            //set right child of currentNode to black
        }

        return currentNode;
    } //insertRB()

    /**
     * This method rotates the node with key 'key' right. Will only run if there is a sequence of 2 left-leaning red nodes.
     * @param key the key of the node to rotate
     * @return the new representation of the subtree that used to start at the node with key value 'key'
     */
    public Node rotateRight(int key) {
        Node currentNode = getNode(key);
        Node redNode = currentNode.left;                        //the red node on the left (one of two on the left)
        Node redNodeRight = redNode.right;                      //red node's right child (it's ok if it's null) - will be needed to move to the left child of currentNode

        currentNode.left = redNodeRight;
        redNode.right = currentNode;                            //most important part of the rotation here - makes sure the first red node is the new 'root' of this subtree, and currentNode is its right child

        redNode.setColor(Color.BLACK);                          //swap redNode's color
        redNodeRight.setColor(Color.RED);                       //swap redNode's right child's color

        return redNode;                                         //redNode is returned because it is in the place that currentNode was in
    } //rotateRight()

    /**
     * This method rotates the node with key 'key' left. Will only run if there is a right-leaning red node.
     * @param key the key of the node to rotate
     * @return the new representation of the subtree that used to start at the node with key value 'key'
     */
    public Node rotateLeft(int key) {
        Node currentNode = getNode(key);
        Node redNode = currentNode.right;                           //the red node on the right (the reason this method was called)
        Node redNodeLeft = redNode.left;                            //the red node's left child (it's ok if it's null) - will be needed to move to the right child of currentNode

        currentNode.right = redNodeLeft;
        redNode.left = currentNode;                                 //most important part of the rotation here - makes sure the red node is no longer right leaning

        redNode.setColor(Color.BLACK);                              //swap redNode's color
        redNode.left.setColor(Color.RED);                           //swap redNode's left child's color

        return redNode;                                             //redNode is returned because it is in the place that currentNode was in
    } //rotateLeft

    /**
     * This function iteratively traverses the tree until it finds the node with key value 'key', then counts the amount of black nodes from that node until the bottom of the tree.
     * @param key the key that we're looking for in the tree
     * @return a String representation of the black height of the tree from the node wih key value 'key'
     */
    public String getBlackHeight(int key) {
        if (!searchKey(key)) {                                                  //if the key is invalid, we can't calculate the black height
            return "none";
        }

        Node currentNode = root;
        int blackHeight = 0;

        //iteratively find the node with key value that is equal to 'key'
        while (currentNode.getKey() != key) {
            if (currentNode.getKey() < key) {
                currentNode = currentNode.right;
            }
            else if (currentNode.getKey() > key) {
                currentNode = currentNode.left;
            }
        }


        while (currentNode != null) {
            if ((currentNode.getColor() == Color.BLACK) || (currentNode.left == null && currentNode.right == null)) {   //if we come across any black node or a leaf (leaves aren't always black), black height increases
                blackHeight++;
            }

            currentNode = currentNode.right;                                                                            //since we can go any direction we want (red-black tree is balanced), I arbitrarily chose right
        }
        return Integer.toString(blackHeight);
    } //getBlackHeight()
} //Class BinarySearchTree
