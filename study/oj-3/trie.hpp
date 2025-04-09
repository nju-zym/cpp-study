#ifndef TRIE_H
#define TRIE_H
#include <string>
#include <vector>

struct TrieNode {
    char val;
    bool isend;
    std::vector<TrieNode*> child;
    // 你可以添加其他的成员变量

    TrieNode();
    explicit TrieNode(char v);
    ~TrieNode();

    // 禁用拷贝构造和赋值操作，因为我们使用了原始指针
    TrieNode(TrieNode const&)            = delete;
    TrieNode& operator=(TrieNode const&) = delete;
};
class Trie {
   private:
    TrieNode* root;
    // 你可以添加其他的成员变量

   public:
    Trie();
    ~Trie();

    // 禁用拷贝构造和赋值操作，因为我们使用了原始指针
    Trie(Trie const&)            = delete;
    Trie& operator=(Trie const&) = delete;

    void Insert(std::string const& st);
    bool Lookup(std::string const& st);
    int Prefix(std::string const& st);
};

#endif