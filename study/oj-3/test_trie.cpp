#include "trie.hpp"
#include <iostream>

int main() {
    // Test case 1: Basic Insert and Lookup
    {
        std::cout << "Test case 1: Basic Insert and Lookup" << std::endl;
        Trie* trie = new Trie;
        trie->Insert("Fibonacci");
        if (trie->Lookup("Fibonacci")) {
            std::cout << "Found Fibonacci: Yes" << std::endl;
        } else {
            std::cout << "Found Fibonacci: No" << std::endl;
        }
        
        if (trie->Lookup("Fibnacci")) {
            std::cout << "Found Fibnacci: Yes" << std::endl;
        } else {
            std::cout << "Found Fibnacci: No" << std::endl;
        }
        delete trie;
        std::cout << std::endl;
    }
    
    // Test case 2: Prefix function
    {
        std::cout << "Test case 2: Prefix function" << std::endl;
        Trie* trie = new Trie;
        trie->Insert("apple");
        trie->Insert("app");
        trie->Insert("april");
        trie->Insert("banana");
        trie->Insert("band");
        
        std::cout << "Prefix 'a': " << trie->Prefix("a") << std::endl;       // Should be 3
        std::cout << "Prefix 'ap': " << trie->Prefix("ap") << std::endl;     // Should be 3
        std::cout << "Prefix 'app': " << trie->Prefix("app") << std::endl;   // Should be 2
        std::cout << "Prefix 'appl': " << trie->Prefix("appl") << std::endl; // Should be 1
        std::cout << "Prefix 'ban': " << trie->Prefix("ban") << std::endl;   // Should be 2
        std::cout << "Prefix 'bana': " << trie->Prefix("bana") << std::endl; // Should be 1
        std::cout << "Prefix 'bandit': " << trie->Prefix("bandit") << std::endl; // Should be 0
        
        delete trie;
        std::cout << std::endl;
    }
    
    // Test case 3: Empty string prefix
    {
        std::cout << "Test case 3: Empty string prefix" << std::endl;
        Trie* trie = new Trie;
        trie->Insert("a");
        trie->Insert("b");
        trie->Insert("c");
        
        std::cout << "Prefix '': " << trie->Prefix("") << std::endl; // Should be 3
        
        delete trie;
        std::cout << std::endl;
    }
    
    return 0;
}
