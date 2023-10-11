#include <iostream>

template <typename T>
class TreeNode {
public:
    T data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class Set {
private:
    TreeNode<T>* root;

    TreeNode<T>* insert(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return new TreeNode<T>(value);
        }

        if (value < parentNode->data) {
            parentNode->left = insert(parentNode->left, value);
        } else if (value > parentNode->data) {
            parentNode->right = insert(parentNode->right, value);
        }

        return parentNode;
    }

    bool containsNode(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return false;
        }

        if (value == parentNode->data) {
            return true;
        } else if (value < parentNode->data) {
            return containsNode(parentNode->left, value);
        } else {
            return containsNode(parentNode->right, value);
        }
    }

    TreeNode<T>* remove(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return parentNode;
        }

        if (value < parentNode->data) {
            parentNode->left = remove(parentNode->left, value);
        } else if (value > parentNode->data) {
            parentNode->right = remove(parentNode->right, value);
        } else {
            // if value == parentNode->data
            if (parentNode->left == nullptr) {
                TreeNode<T>* temp = parentNode->right;
                delete parentNode;
                return temp;
            } else if (parentNode->right == nullptr) {
                TreeNode<T>* temp = parentNode->left;
                delete parentNode;
                return temp;
            }

            //// if parentNode has two children
            TreeNode<T>* temp = findMin(parentNode->right);

            // Copy data to this node
            parentNode->data = temp->data;

            // Delete the node
            parentNode->right = remove(parentNode->right, temp->data);
        }

        return parentNode;
    }

    //used for remove method
    TreeNode<T>* findMin(TreeNode<T>* node) {
        TreeNode<T>* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

public:
    Set() : root(nullptr) {}

    bool add(T value) {
        if (!containsNode(root, value)) {
            root = insert(root, value);
            return true;
        }
        return false;
    }

    bool remove(T value) {
        if (containsNode(root, value)) {
            root = remove(root, value);
            return true;
        }
        return false;
    }

    bool containsNode(T value) {
        return containsNode(root, value);
    }
};

int main() {
    Set<int> set;
    
    set.add(30);
    set.add(30);
    set.add(0);
    set.add(50);

    std::cout << set.containsNode(0) << std::endl;
    set.remove(0);
    std::cout << set.containsNode(0) << std::endl;

    return 0;
}