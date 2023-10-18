#include <iostream>

template <typename T>
class TreeNode {
public:
    T value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(T value_) : value(value_), left(nullptr), right(nullptr) {}
};

template <typename T>
class Set {
private:
    TreeNode<T>* root;

    TreeNode<T>* insert(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return new TreeNode<T>(value);
        }

        if (value < parentNode->value) {
            parentNode->left = insert(parentNode->left, value);
        } else if (value > parentNode->value) {
            parentNode->right = insert(parentNode->right, value);
        }

        return parentNode;
    }

    bool containsNode(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return false;
        }

        if (value == parentNode->value) {
            return true;
        } else if (value < parentNode->value) {
            return containsNode(parentNode->left, value);
        } else {
            return containsNode(parentNode->right, value);
        }
    }

    TreeNode<T>* remove(TreeNode<T>* parentNode, T value) {
        if (parentNode == nullptr) {
            return parentNode;
        }

        if (value < parentNode->value) {
            parentNode->left = remove(parentNode->left, value);
        } else if (value > parentNode->value) {
            parentNode->right = remove(parentNode->right, value);
        } else {
            // if value == parentNode->value
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

            // Copy value to this node
            parentNode->value = temp->value;

            // Delete the node
            parentNode->right = remove(parentNode->right, temp->value);
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
    set.add(30); //won't be added
    set.add(0);
    set.add(50);

    std::cout << set.containsNode(0) << std::endl;
    set.remove(0);
    std::cout << set.containsNode(0) << std::endl;

    return 0;
}