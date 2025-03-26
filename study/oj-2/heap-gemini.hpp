#ifndef HEAP_H
#define HEAP_H

// Include the definition of MM_Struct
#include "mm.hpp"
// Include standard definition for size_t
#include <cstddef>

class Heap {
   private:
    MM_Struct* head;  // Pointer to the head of the memory unit linked list
    MM_Struct* tail;  // Pointer to the tail of the memory unit linked list
    size_t size;      // Current total capacity of the heap in bytes

    // Private helper function to increase the heap's capacity
    // This is called by Malloc when more space is needed.
    void expandHeap(size_t min_required_capacity);

    // You can add other private helper functions here if you find them useful

   public:
    // Constructor: Initializes the heap with a capacity of _size bytes.
    Heap(size_t _size);

    // Destructor: Cleans up all allocated MM_Struct nodes.
    ~Heap();

    // Returns the current total capacity of the heap in bytes.
    size_t GetTotalSize() {
        return size;
    }

    // Allocates a contiguous block of memory of the specified size (in bytes).
    // Returns a pointer to the first MM_Struct unit of the allocated block,
    // or nullptr if allocation fails (e.g., due to fragmentation or inability to expand).
    MM_Struct* Malloc(size_t size);

    // Frees the block of memory previously allocated by Malloc, starting at address 'p'.
    // Handles error conditions like freeing null, already freed, or non-start pointers.
    void Free(MM_Struct* p);

    // Outputs the value(s) stored in the memory unit(s) pointed to by 'p'.
    // Behavior depends on whether 'p' points to the start or middle of an allocated block.
    // Handles error conditions like accessing null or unallocated pointers.
    void output(MM_Struct* p);

    // Sets the integer value 'x' for the memory unit pointed to by 'p'.
    // Handles error conditions like accessing null or unallocated pointers.
    void setval(MM_Struct* p, int x);
};

#endif  // HEAP_H