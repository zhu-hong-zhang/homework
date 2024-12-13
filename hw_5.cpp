#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

struct Node23 {
    vector<int> keys;
    vector<Node23*> children;
    Node23* parent;

    Node23(int key) : parent(nullptr) {
        keys.push_back(key);
        children.push_back(nullptr);
        children.push_back(nullptr);
    }
};

int height(Node *N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = new Node(key);
    return node;
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *N) {
    if (N == nullptr)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key) {
    if (node == nullptr)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node * minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int key) {
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node *temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void preOrder(Node *root) {
    if (root != nullptr) {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void split(Node23* node) {
    if (node->keys.size() < 3) return;

    int midKey = node->keys[1];
    Node23* leftChild = new Node23(node->keys[0]);
    Node23* rightChild = new Node23(node->keys[2]);

    leftChild->children[0] = node->children[0];
    leftChild->children[1] = node->children[1];
    rightChild->children[0] = node->children[2];
    rightChild->children[1] = node->children[3];

    if (leftChild->children[0]) leftChild->children[0]->parent = leftChild;
    if (leftChild->children[1]) leftChild->children[1]->parent = leftChild;
    if (rightChild->children[0]) rightChild->children[0]->parent = rightChild;
    if (rightChild->children[1]) rightChild->children[1]->parent = rightChild;

    if (node->parent == nullptr) {
        node->keys = {midKey};
        node->children = {leftChild, rightChild};
        leftChild->parent = node;
        rightChild->parent = node;
    } else {
        Node23* parent = node->parent;
        parent->keys.push_back(midKey);
        sort(parent->keys.begin(), parent->keys.end());

        for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
            if (*it == node) {
                parent->children.erase(it);
                break;
            }
        }
        parent->children.push_back(leftChild);
        parent->children.push_back(rightChild);
        sort(parent->children.begin(), parent->children.end(), [](Node23* a, Node23* b) {
            return a->keys[0] < b->keys[0];
        });

        leftChild->parent = parent;
        rightChild->parent = parent;

        split(parent);
    }
}

Node23* insert(Node23* root, int key) {
    if (root == nullptr) return new Node23(key);

    Node23* node = root;
    while (!node->children[0]) {
        if (key < node->keys[0]) {
            node = node->children[0];
        } else if (node->keys.size() == 1 || key < node->keys[1]) {
            node = node->children[1];
        } else {
            node = node->children[2];
        }
    }

    node->keys.push_back(key);
    sort(node->keys.begin(), node->keys.end());
    split(node);

    while (root->parent) root = root->parent;
    return root;
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

void printTree(Node23* root, string indent = "", bool last = true) {
    if (root != nullptr) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "     ";
        } else {
            cout << "L----";
            indent += "|    ";
        }
        for (int key : root->keys) {
            cout << key << " ";
        }
        cout << endl;
        for (size_t i = 0; i < root->children.size(); ++i) {
            printTree(root->children[i], indent, i == root->children.size() - 1);
        }
    }
}

int main() {
    Node *root = nullptr;
    Node23* root23 = nullptr;
    int choice, value;

    cout << "請輸入一串整數值（以空格分隔，輸入完成後按 Enter）: ";
    string input;
    getline(cin, input);
    stringstream ss(input);
    while (ss >> value) {
        root = insert(root, value);
        //root23 = insert(root23, value);
    }

    while (true) {
        cout << "選擇操作: 1. 插入 2. 刪除 3. 顯示 AVL 樹 4. 顯示 2-3 樹 5. 退出\n";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "輸入要插入的數值: ";
                cin >> value;
                root = insert(root, value);
                //root23 = insert(root23, value);
                break;
            case 2:
                cout << "輸入要刪除的數值: ";
                cin >> value;
                root = deleteNode(root, value);
                break;
            case 3:
                cout << "AVL 樹的結構:\n";
                printTree(root);
                break;
            case 4:
                cout << "2-3 樹的結構:\n";
                printTree(root23);
                break;
            case 5:
                return 0;
            default:
                cout << "無效的選擇，請重新輸入。\n";
        }
    }

    return 0;
}