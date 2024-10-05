#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

class SufArr;

class Node {
public:
    map<char, Node *> dest;
    string::iterator begin, end;
    Node *sufLink;

    Node(string::iterator start, string::iterator end) : begin(start), end(end), sufLink(nullptr) {};

    ~Node() = default;
};

class SufTree {
public:
    SufTree(string s): text(move(s)), root(new Node(text.end(), text.end())), isReady(0) {
        activeEdge = text.begin();
        activeLen = 0;

        activeNode = root;
        SLmarker = root;
        root->sufLink = root;

        for(string::iterator i = text.begin(); i != text.end(); ++i) {
            TrExtend(i);
        }
    }

    ~SufTree() = default;

    friend SufArr;
private: 
    string text;
    Node *root;
    Node *SLmarker;
    Node *activeNode;
    int isReady;
    int activeLen;
    string::iterator activeEdge;

    void TrExtend(string::iterator current) {
        SLmarker = root;
        ++isReady;

        while(isReady) {
            if(!activeLen) {
                activeEdge = current;
            }
            auto tree_it = activeNode->dest.find(*activeEdge);
            Node *next = (tree_it == activeNode->dest.end()) ? NULL : tree_it->second;
            if(!next) {
                Node *leaf = new Node(current, text.end());
                activeNode->dest[*activeEdge] = leaf;
                SufLinkActive(activeNode);
            } else {
                if(WalkDown(current, next)) {
                    continue;
                }

                if(*(next->begin + activeLen) == *current) {
                    ++activeLen;
                    SufLinkActive(activeNode);
                    break;
                }

                Node *split = new Node(next->begin, next->begin + activeLen);
                Node *leaf = new Node(current, text.end());
                activeNode->dest[*activeEdge] = split;

                split->dest[*current] = leaf;
                next->begin += activeLen;
                split->dest[*next->begin] = next;
                SufLinkActive(split);
            }
            --isReady;
            if(activeNode == root && activeLen) {
                --activeLen;
                activeEdge = current - isReady + 1;
            } else {
                activeNode = (activeNode->sufLink) ? activeNode -> sufLink : root;
            }
        }
    }

    void SufLinkActive(Node *node) {
        if(SLmarker != root) {
            SLmarker->sufLink = node;
        }

        SLmarker = node;
    }

    bool WalkDown(string::iterator current_position, Node *node) {
        int edgeLen = EdgeLen(node, current_position);
        if(activeLen >= edgeLen) {
            activeEdge += edgeLen;
            activeLen -= edgeLen;
            activeNode = node;
            return true;
        }
        return false;
    }

    int EdgeLen(Node *node, string::iterator current_position) {
        return static_cast<int>(min(node->end, current_position + 1) - node->begin);
    }

    void DFS(Node *node, vector<int > &result, int depth) {
        if(node->dest.empty()) {
            result.push_back(static_cast<int &&>(text.size() - depth));
            return;
        }
        for(auto &it: node->dest) {
            int tmp = depth;
            tmp += it.second->end - it.second->begin;
            DFS(it.second, result, tmp);
        }
    }

    void Destroyer(Node *node) {
        for(auto &it: node->dest) {
            Destroyer(it.second);
        }
        delete node;
    }

};

class SufArr {
public:
    SufArr(SufTree tree): text(tree.text), array() {
        tree.DFS(tree.root, array, 0);
        tree.Destroyer(tree.root);
    }

    vector<int> Search(string pattern) {
        pair<vector<int>::iterator, vector<int>::iterator> borders(array.begin(), array.end());
        for(int i = 0; i < pattern.size() && borders.first != borders.second; ++i) {
            borders = equal_range(borders.first, borders.second, numeric_limits<int>::max(),
                [this, &pattern, &i](int l, int r) -> bool {
                    return (l == numeric_limits<int>::max()) ? (pattern[i] < text[i + r]) : (text[i + l] < pattern[i]);
                }
            ); 
        }

        vector<int> result(borders.first, borders.second);
        sort(result.begin(), result.end());

        return result;
    }

    ~SufArr() = default;
private: 
    string text;
    vector<int> array;
};

int main() {
    string text, pattern;
    cin >> text;
    SufTree tree(text + "$");

    SufArr array(tree);

    for(int num = 1; cin >> text; ++num) {
        vector<int> result = array.Search(text);
        if(!result.empty()) {
            cout << num << ": ";
            for(int i = 0; i < result.size(); ++i) {
                cout << result[i] + 1;
                if(i < result.size() - 1) 
                    cout << ", ";
            }
            cout << "\n";
        }
    }
    return 0;
}