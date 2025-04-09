#include "trie.hpp"

#include <iostream>

TrieNode::TrieNode() : val(' '), isend(false), child() {}

TrieNode::TrieNode(char v) : val(v), isend(false), child() {}

TrieNode::~TrieNode() {
    for (TrieNode* node : child) {
        delete node;
    }
    child.clear();
    std::cout << "~TrieNode!" << '\n';
}

Trie::Trie() : root(new TrieNode()) {}

Trie::~Trie() {
    delete root;
}

void Trie::Insert(std::string const& st) {
    TrieNode* current = root;

    for (char c : st) {
        bool found = false;
        for (TrieNode* child_node : current->child) {
            if (child_node->val == c) {
                current = child_node;
                found   = true;
                break;
            }
        }

        if (!found) {
            auto new_node = new TrieNode(c);
            current->child.push_back(new_node);
            current = new_node;
        }
    }

    current->isend = true;
}

bool Trie::Lookup(std::string const& st) {
    TrieNode* current = root;

    for (char c : st) {
        bool found = false;
        for (TrieNode* child_node : current->child) {
            if (child_node->val == c) {
                current = child_node;
                found   = true;
                break;
            }
        }

        if (!found) {
            return false;
        }
    }

    return current->isend;
}

// 辅助函数：计算以给定节点为根的子树中单词的数量
static int countWords(TrieNode* node) {
    int count = 0;

    if (node->isend) {
        count++;
    }

    for (TrieNode* child : node->child) {
        count += countWords(child);
    }

    return count;
}

int Trie::Prefix(std::string const& st) {
    TrieNode* current = root;

    if (st.empty()) {
        return countWords(root);
    }

    for (char c : st) {
        bool found = false;
        for (TrieNode* child_node : current->child) {
            if (child_node->val == c) {
                current = child_node;
                found   = true;
                break;
            }
        }

        if (!found) {
            return 0;
        }
    }

    return countWords(current);
}
