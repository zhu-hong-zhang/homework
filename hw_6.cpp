#include <iostream>
#include <iomanip>
using namespace std;

class Node {
public:
    int key;
    Node* left, * right;
    Node(int item) {
        key = item;
        left = right = nullptr;
    }
};

class BST {
private:
    Node* root;

    void inorderRec(Node* node) {
        if (node != nullptr) {
            inorderRec(node->left);
            cout << node->key << " ";
            inorderRec(node->right);
        }
    }
    
    void printTree(Node* root, string indent = "", bool last = true) {
        if (root != nullptr) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "     ";
            } else {
                cout << "L----";
                indent += "|    ";
            }
            cout << root->key << endl;
            printTree(root->left, indent, false);
            printTree(root->right, indent, true);
        }
    }

    Node* deleteRec(Node* root, int key) {
        if (root == nullptr) return root;

        if (key < root->key)
            root->left = deleteRec(root->left, key);
        else if (key > root->key)
            root->right = deleteRec(root->right, key);
        else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }

            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteRec(root->right, temp->key);
        }
        return root;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

public:
    BST() { root = nullptr; }

    void insert(int key) {
        root = insertRec(root, key);
    }

    Node* insertRec(Node* node, int key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insertRec(node->left, key);
        else if (key > node->key)
            node->right = insertRec(node->right, key);

        return node;
    }

    void deleteNode(int key) {
        root = deleteRec(root, key);
    }

    void inorder() {
        inorderRec(root);
    }

    void printTree() {
        printTree(root);
    }
};

int main() {
    BST bst;
    int val, choice;
    cout << "�п�J�@���ƭȡ]�H�Ů���j�A��J������� Enter�^: ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    while (ss >> val) {
        bst.insert(val);
    }

    while (true) {
        cout << "��ܾާ@: 1. ���J 2. �R�� 3. ��� BST �� 4. ��� m-way �j�M�� 5. �h�X\n";
        cin.sync();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "��J�n���J���ƭ�: ";
                cin >> val;
                bst.insert(val);
                break;
            case 2:
                cout << "��J�n�R�����ƭ�: ";
                cin >> val;
                bst.deleteNode(val);
                break;
            case 3:
                cout << "BST �𪺵��c:\n";
                bst.printTree();
                break;
            case 4:
                break;
            case 5:
                return 0;
            default:
                cout << "�L�Ī���ܡA�Э��s��J�C\n";
        }
    }

    return 0;
}