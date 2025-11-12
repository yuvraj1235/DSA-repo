#include <iostream>
#include <algorithm> // For std::max
#include <vector>

using namespace std;

// --- 1. Node Structure ---
struct Node {
    int key;
    Node *left;
    Node *right;
    int height;
};

// --- 2. Utility Functions ---

// Get the height of a node
int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Get maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Helper function to create a new node
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // New node is initially added at height 1
    return node;
}

// Get the balance factor of a node
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Find the node with the minimum key value (used for deletion)
Node *minValueNode(Node *node) {
    Node *current = node;
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
    return current;
}

// --- 3. Rotation Functions (The Self-Balancing Core) ---

// A utility function to right rotate subtree rooted with y
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

// --- 4. Insertion Function ---
Node* insert(Node* node, int key) {
    // 1. Perform standard BST insertion
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Duplicate keys not allowed
        return node;

    // 2. Update height of current node
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Get the balance factor
    int balance = getBalance(node);

    // 4. If unbalanced, apply rotations:
    
    // Left Left Case (LL)
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case (RR)
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case (LR)
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case (RL)
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // 5. Return the (potentially unchanged) node pointer
    return node;
}

// --- 5. Deletion Function ---
Node* deleteNode(Node* root, int key) {
    // 1. Perform standard BST deletion
    if (root == NULL)
        return root;

    // Key is in the left subtree
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    // Key is in the right subtree
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    // Node with key found
    else {
        // Node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            // One child case
            else {
                // Copy the contents of the non-empty child
                *root = *temp; 
            }
            delete temp;
        } 
        // Node with two children
        else {
            // Get the inorder successor (smallest in the right subtree)
            Node *temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node, then return
    if (root == NULL)
        return root;

    // 2. Update height of current node
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Get the balance factor
    int balance = getBalance(root);

    // 4. If unbalanced, apply rotations (same four cases as insertion):

    // Left Left Case (LL)
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case (LR)
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case (RR)
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case (RL)
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// --- 6. Traversal Function ---
void inorder(Node *root) {
    if (root != NULL) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// --- 7. Main Driver Code ---
int main() {
    Node *root = NULL;

    // Insertion
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30); // RR rotation happens here (left rotate at 10)
    root = insert(root, 40);
    root = insert(root, 50); // RR rotation happens here (left rotate at 30)
    root = insert(root, 25); // RL rotation happens here (right rotate at 40, left rotate at 20)

    cout << "Inorder traversal after insertions: ";
    inorder(root);
    cout << endl; // Expected: 10 20 25 30 40 50

    // Deletion
    root = deleteNode(root, 20); // Deleting a node that requires rebalancing

    cout << "Inorder traversal after deleting 20: ";
    inorder(root);
    cout << endl; // Expected: 10 25 30 40 50

    root = deleteNode(root, 30); // Deleting the root

    cout << "Inorder traversal after deleting 30: ";
    inorder(root);
    cout << endl; // Expected: 10 25 40 50

    // Clean up memory
    // (A proper cleanup function would be needed for production code)

    return 0;
}