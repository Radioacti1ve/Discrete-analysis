#include <iostream>
#include <utility>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

using Tree_pair = pair<string, unsigned long long>;

class AVLTree;

class Node {
public:
    Node(Tree_pair value, Node *parent);
    void Print(int height);
    void InsertNode(AVLTree *tree, const Tree_pair &valueToInsert, bool &flag);
    void RemoveNode(AVLTree *tree, const string &key, bool &flag);
    unsigned long long Search(const string &key, bool &flag);

    void Clear();

private:
    Tree_pair value;
    int balance = 0;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    void Balance(AVLTree *tree, const string &key);
    void RemoveBalance(AVLTree *tree, const string &key);
    Tree_pair SearchMinimumAndRemoveNode(AVLTree *tree);
    void LeftRotate(AVLTree *tree);
    void RightRotate(AVLTree *tree);
};


class AVLTree {
private:
    Node *root = nullptr;
public:
    void SetRoot(Node *root);
    void Print();
    string InsertNode(Tree_pair &value);
    string RemoveNode(const string &key);
    string Search(const string &key);
    void Clear();

};

void AVLTree::SetRoot(Node *newRoot) {
    this->root = newRoot;
}

void AVLTree::Print() {
    root->Print(0);
}

string AVLTree::InsertNode(Tree_pair &value) {
    bool flag = true;
    if (!root) {
        root = new Node(value, nullptr);
    } else {
        root->InsertNode(this, value, flag);
    }
    return flag ? "OK" : "Exist";
}

string AVLTree::RemoveNode(const string& key) {
    bool flag = true;
    if (!root) {
        flag = false;
    } else {
        root->RemoveNode(this, key, flag);
    }
    return flag ? "OK" : "NoSuchWord";
}

string AVLTree::Search(const string &key) {
    bool flag = true;
    unsigned long long value;
    if (!root) {
        flag = false;
    } else {
        value = root->Search(key, flag);
    }
    return flag ? ("OK: " + to_string(value)) : "NoSuchWord";
}

void AVLTree::Clear() {
    if (!root) {
        return;
    }
    root->Clear();
    root = nullptr;
}


Node::Node(Tree_pair value, Node* parent) : value(move(value)), parent(parent) {}

void Node::Print(int height){
    if (this->right != nullptr) {
        this->right->Print(height + 2);
    }
    cout << height / 2 << ":";
    for (int i = 0; i < height; i++) {
        cout << "-";
    }
    cout << this->value.first << ": " << this->balance << "\n";
    if (this->left != nullptr) {
        this->left->Print(height + 2);
    }
}

void Node::InsertNode(AVLTree* tree, const Tree_pair& valueToInsert, bool& flag) {
    if (valueToInsert.first == this->value.first) {
        flag = false;
        return;
    }
    if (valueToInsert.first.compare(this->value.first) < 0) {
        if (this->left == nullptr) {
            this->left = new Node(valueToInsert, this);
            Balance(tree, valueToInsert.first);
        } else {
            this->left->InsertNode(tree, valueToInsert, flag);
        }
    } else {
        if (this->right == nullptr) {
            this->right = new Node(valueToInsert, this);
            Balance(tree, valueToInsert.first);
        } else {
            this->right->InsertNode(tree, valueToInsert, flag);
        }
    }
}

void Node::Balance(AVLTree* tree, const string& key) {
    if (key.compare(this->value.first) < 0) {
        this->balance++;
    } else {
        this->balance--;
    }
    if (abs(this->balance) == 1) {
        if (this->parent != nullptr) {
            this->parent->Balance(tree, this->value.first);
        }
        return;
    }
    if (this->balance == -2) {
        if (this->right->balance == -1) {
            this->right->balance = 0;
            this->balance = 0;
            this->LeftRotate(tree);
        } else {
            switch (this->right->left->balance) {
                case -1:
                    this->balance = 1;
                    this->right->balance = 0;
                    break;
                case 0:
                    this->balance = 0;
                    this->right->balance = 0;
                    break;
                case 1:
                    this->balance = 0;
                    this->right->balance = -1;
                    break;
            }
            this->right->left->balance = 0;
            this->right->RightRotate(tree);
            this->LeftRotate(tree);
        }
        return;
    }
    if (this->balance == 2) {
        if (this->left->balance == 1) {
            this->left->balance = 0;
            this->balance = 0;
            this->RightRotate(tree);
        } else {
            switch (this->left->right->balance) {
                case -1:
                    this->balance = 0;
                    this->left->balance = 1;
                    break;
                case 0:
                    this->balance = 0;
                    this->left->balance = 0;
                    break;
                case 1:
                    this->balance = -1;
                    this->left->balance = 0;
                    break;
            }
            this->left->right->balance = 0;
            this->left->LeftRotate(tree);
            this->RightRotate(tree);
        }
        return;
    }
}

void Node::LeftRotate(AVLTree* tree) {
    Node* new_root = this->right;
    this->right = new_root->left;
    if (new_root->left != nullptr) {
        new_root->left->parent = this;
    }
    new_root->parent = this->parent;
    if (this->parent == nullptr) {
        tree->SetRoot(new_root);
    } else if (this == this->parent->left) {
        this->parent->left = new_root;
    } else {
        this->parent->right = new_root;
    }
    new_root->left = this;
    this->parent = new_root;
}

void Node::RightRotate(AVLTree* tree) {
    Node* new_root = this->left;
    this->left = new_root->right;
    if (new_root->right != nullptr) {
        new_root->right->parent = this;
    }
    new_root->parent = this->parent;
    if (this->parent == nullptr) {
        tree->SetRoot(new_root);
    } else if (this == this->parent->right) {
        this->parent->right = new_root;
    } else {
        this->parent->left = new_root;
    }
    new_root->right = this;
    this->parent = new_root;
}

void Node::RemoveNode(AVLTree *tree, const string &key, bool &flag) {
    if (key.compare(this->value.first) < 0) {
        if (this->left != nullptr) {
            this->left->RemoveNode(tree, key, flag);
        } else {
            flag = false;
            return;
        }
    } else if (key.compare(this->value.first) > 0){
        if (this->right != nullptr) {
            this->right->RemoveNode(tree, key, flag);
        } else {
            flag = false;
            return;
        }
    } else {
        if (this->left == nullptr and this->right == nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(nullptr);
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = nullptr;
            } else {
                this->parent->right = nullptr;
            }
            this->parent->RemoveBalance(tree, key);
            delete this;
            return;
        } else if (this->left != nullptr and this->right == nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(this->left);
                this->left->parent = nullptr;
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->left;
            } else {
                this->parent->right = this->left;
            }
            this->left->parent = this->parent;
            this->parent->RemoveBalance(tree, key);
            delete this;
            return;
        } else if (this->left == nullptr and this->right != nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(this->right);
                this->right->parent = nullptr;
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->right;
            } else {
                this->parent->right = this->right;
            }
            this->right->parent = this->parent;
            this->parent->RemoveBalance(tree, key);
            delete this;
            return;
        } else {
            this->value = this->right->SearchMinimumAndRemoveNode(tree);
            return;
        }
    }
}

void Node::RemoveBalance(AVLTree *tree, const string &key) {
    if (key.compare(this->value.first) < 0) {
        this->balance--;
    } else {
        this->balance++;
    }
    if (this->balance == 0) {
        if (this->parent != nullptr) {
            this->parent->RemoveBalance(tree, this->value.first);
        }
        return;
    }
    if (this->balance == -2) {
        if (this->right->balance == 0) {
            this->right->balance = 1;
            this->balance = -1;
            this->LeftRotate(tree);
        } else if (this->right->balance == -1) {
            this->right->balance = 0;
            this->balance = 0;
            this->LeftRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemoveBalance(tree, this->value.first);
            }
        } else {
            switch (this->right->left->balance) {
                case -1:
                    this->balance = 1;
                    this->right->balance = 0;
                    break;
                case 0:
                    this->balance = 0;
                    this->right->balance = 0;
                    break;
                case 1:
                    this->balance = 0;
                    this->right->balance = -1;
                    break;
            }
            this->right->left->balance = 0;
            this->right->RightRotate(tree);
            this->LeftRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemoveBalance(tree, this->value.first);
            }
        }
        return;
    }
    if (this->balance == 2) {
        if (this->left->balance == 0) {
            this->left->balance = -1;
            this->balance = 1;
            this->RightRotate(tree);
        } else if (this->left->balance == 1) {
            this->left->balance = 0;
            this->balance = 0;
            this->RightRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemoveBalance(tree, this->value.first);
            }
        } else {
            switch (this->left->right->balance) {
                case -1:
                    this->balance = 0;
                    this->left->balance = 1;
                    break;
                case 0:
                    this->balance = 0;
                    this->left->balance = 0;
                    break;
                case 1:
                    this->balance = -1;
                    this->left->balance = 0;
                    break;
            }
            this->left->right->balance = 0;
            this->left->LeftRotate(tree);
            this->RightRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemoveBalance(tree, this->value.first);
            }
        }
        return;
    }
}

Tree_pair Node::SearchMinimumAndRemoveNode(AVLTree* tree) {
    if (this->left == nullptr) {
        Tree_pair new_value = this->value;
        if (this->right != nullptr) {
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->right;
                this->right->parent = this->parent;
            } else {
                this->parent->right = this->right;
                this->right->parent = this->parent;
            }
        } else {
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = nullptr;
            } else {
                this->parent->right = nullptr;
            }
        }
        this->parent->RemoveBalance(tree, new_value.first);
        delete this;
        return new_value;
    }
    return this->left->SearchMinimumAndRemoveNode(tree);
}

unsigned long long Node::Search(const string& key, bool& flag) {
    if (this->value.first == key) {
        return this->value.second;
    } else if (this->value.first.compare(key) > 0) {
        if (this->left != nullptr) {
            return this->left->Search(key, flag);
        } else {
            flag = false;
            return -1;
        }
    } else {
        if (this->right != nullptr) {
            return this->right->Search(key, flag);
        } else {
            flag = false;
            return -1;
        }
    }
}

void Node::Clear() {
    if (this->left != nullptr) {
        this->left->Clear();
    }
    if (this->right != nullptr) {
        this->right->Clear();
    }
    delete this;
}



int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    AVLTree tree;
    Tree_pair pair;
    string command;


    while (cin >> command) {
        if (command == "+") {
            string key;
            unsigned long long value;
            cin >> key >> value;
            transform(key.begin(), key.end(), key.begin(),
                      [](unsigned char c){ return tolower(c); });
            pair = make_pair(key, value);
            cout << tree.InsertNode(pair) << "\n";
        } else if (command == "-") {
            string key;
            cin >> key;
            transform(key.begin(), key.end(), key.begin(),
                      [](unsigned char c){ return tolower(c); });
            cout << tree.RemoveNode(key) << "\n";
        } else {
            transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return tolower(c); });
            cout << tree.Search(command) << "\n";
        }
    }

    tree.Clear();

}


