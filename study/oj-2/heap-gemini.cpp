#include "heap-gemini.hpp"

#include <cmath>  // Not strictly necessary, using integer arithmetic for size calculations
#include <iostream>
#include <limits>  // For SIZE_MAX
#include <new>     // For std::bad_alloc handling

// Define SIZE_MAX if not available (older C++ standards) - usually in <limits> or <cstddef>
#ifndef SIZE_MAX
#    define SIZE_MAX std::numeric_limits<size_t>::max()
#endif

// --- Previous Code (Constructor, Destructor, Malloc, Free, expandHeap) ---
// --- Assume the code from the previous step is here ---

// Private helper function to expand the heap capacity
// Doubles the current capacity until it's at least 'min_required_capacity'.
// Appends new MM_Struct nodes to the end of the list.
// Assumes sizeof(int) is 4 as per the problem's memory unit definition.
void Heap::expandHeap(size_t min_required_capacity) {
    size_t new_total_size  = this->size;
    size_t const unit_size = sizeof(int);  // Should be 4

    // Determine a starting point for doubling if current size is 0
    if (new_total_size == 0) {
        new_total_size = unit_size;  // Start with minimum unit size if empty
        if (new_total_size == 0) {
            new_total_size = 4;  // Ensure non-zero base for doubling
        }
    }

    // Double the size until it meets the minimum requirement
    while (new_total_size < min_required_capacity) {
        // Check for potential overflow before doubling
        if (new_total_size > SIZE_MAX / 2) {
            std::cerr << "Error: Heap expansion would cause size_t overflow. Cannot expand further." << std::endl;
            return;  // Expansion fails
        }
        new_total_size *= 2;
    }

    // Only proceed if actual expansion is happening (new size > old size)
    if (new_total_size <= this->size) {
        return;
    }

    // Calculate number of units based on old and new sizes (using ceiling division)
    // Formula: (total_bytes + unit_size - 1) / unit_size
    size_t old_num_units = (this->size == 0) ? 0 : (this->size + unit_size - 1) / unit_size;
    size_t new_num_units = (new_total_size + unit_size - 1) / unit_size;
    size_t units_to_add  = new_num_units - old_num_units;

    if (units_to_add == 0) {
        return;  // No units to add
    }

    // Append new nodes
    MM_Struct* current_append_node = this->tail;  // Start appending after the current tail

    for (size_t i = 0; i < units_to_add; ++i) {
        MM_Struct* newNode = nullptr;
        try {
            newNode = new MM_Struct();  // Allocate new memory unit
        } catch (std::bad_alloc const& e) {
            std::cerr << "Error: Failed to allocate memory during heap expansion: " << e.what() << std::endl;
            // Stop expansion; the heap size 'this->size' remains unchanged.
            return;
        }

        if (current_append_node == nullptr) {  // If heap was initially empty
            this->head          = newNode;
            this->tail          = newNode;
            current_append_node = newNode;
        } else {
            current_append_node->nxt = newNode;  // Link previous tail to new node
            this->tail               = newNode;  // Update heap's tail pointer
            current_append_node      = newNode;  // Move local pointer to the new node
        }
    }
    // Update the total capacity *after* successfully adding all nodes
    this->size = new_total_size;
}

// Constructor
Heap::Heap(size_t _size) {
    size_t const unit_size = sizeof(int);  // Should be 4
    this->size             = _size;        // Store initial requested capacity
    this->head             = nullptr;
    this->tail             = nullptr;

    if (_size == 0) {
        return;  // Empty heap initialized
    }

    // Calculate number of 4-byte units needed (ceiling division)
    size_t num_units = (_size + unit_size - 1) / unit_size;
    if (num_units == 0) {
        this->size = 0;  // Adjust size to 0 if requested size < unit_size
        return;
    }

    // Create the linked list of memory units
    MM_Struct* current_node = nullptr;
    for (size_t i = 0; i < num_units; ++i) {
        MM_Struct* newNode = nullptr;
        try {
            newNode = new MM_Struct();
        } catch (std::bad_alloc const& e) {
            std::cerr << "Memory allocation failed during heap initialization: " << e.what() << std::endl;
            // Clean up already allocated nodes
            MM_Struct* cleanup_node = head;
            while (cleanup_node != nullptr) {
                MM_Struct* next = cleanup_node->nxt;
                delete cleanup_node;
                cleanup_node = next;
            }
            this->head = nullptr;
            this->tail = nullptr;
            this->size = 0;  // Reflect allocation failure
            return;          // Stop constructor
        }

        if (this->head == nullptr) {  // First node
            this->head   = newNode;
            this->tail   = newNode;
            current_node = newNode;
        } else {  // Subsequent nodes
            current_node->nxt = newNode;
            this->tail        = newNode;  // Update tail to the latest node
            current_node      = newNode;  // Move current node pointer
        }
    }
    // Tail's next pointer is already null by default constructor
}

// Destructor
Heap::~Heap() {
    MM_Struct* current = head;
    while (current != nullptr) {
        MM_Struct* nextNode = current->nxt;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;  // Ensure tail is also nullified
}

// Allocate memory using first-fit strategy
MM_Struct* Heap::Malloc(size_t requested_size) {
    size_t const unit_size = sizeof(int);  // Should be 4

    // Validate input as per problem description (size > 0, multiple of 4)
    if (requested_size == 0 || requested_size % unit_size != 0) {
        return nullptr;
    }

    size_t num_units_needed = requested_size / unit_size;

    while (true) {  // Retry loop allows for expansion
        // --- Search Phase (First Fit) ---
        MM_Struct* current           = head;
        MM_Struct* block_start       = nullptr;
        size_t current_free_count    = 0;
        MM_Struct* found_block_start = nullptr;

        while (current != nullptr) {
            if (!current->locked) {
                if (current_free_count == 0) {
                    block_start = current;
                }
                current_free_count++;
                if (current_free_count >= num_units_needed) {
                    found_block_start = block_start;
                    break;
                }
            } else {
                current_free_count = 0;
                block_start        = nullptr;
            }
            current = current->nxt;
        }

        // --- Allocation Phase ---
        if (found_block_start != nullptr) {
            MM_Struct* node_to_lock = found_block_start;
            node_to_lock->locked    = true;
            node_to_lock->size      = requested_size;

            for (size_t i = 1; i < num_units_needed; ++i) {
                node_to_lock = node_to_lock->nxt;
                if (node_to_lock == nullptr) {
                    std::cerr << "Internal Malloc Error: Block shorter than expected during locking." << std::endl;
                    MM_Struct* undo_node = found_block_start;
                    for (size_t j = 0; j <= i && undo_node != nullptr; ++j) {
                        undo_node->locked = false;
                        if (j == 0) {
                            undo_node->size = 0;
                        }
                        undo_node = (j < i) ? undo_node->nxt : nullptr;
                    }
                    return nullptr;
                }
                if (node_to_lock->locked) {
                    std::cerr << "Internal Malloc Error: Node in free block search was already locked." << std::endl;
                    MM_Struct* undo_node = found_block_start;
                    for (size_t j = 0; j <= i && undo_node != nullptr; ++j) {
                        undo_node->locked = false;
                        if (j == 0) {
                            undo_node->size = 0;
                        }
                        undo_node = (j < i) ? undo_node->nxt : nullptr;
                    }
                    return nullptr;
                }
                node_to_lock->locked = true;
                node_to_lock->size   = 0;
            }
            return found_block_start;
        }

        // --- Expansion Check and Execution ---
        size_t current_locked_bytes  = 0;
        MM_Struct* counter_node      = head;
        size_t total_available_units = 0;  // Count total units in heap currently
        while (counter_node != nullptr) {
            if (counter_node->locked) {
                current_locked_bytes += unit_size;
            }
            total_available_units++;
            counter_node = counter_node->nxt;
        }
        size_t current_total_size_bytes = total_available_units * unit_size;  // More accurate current size

        size_t required_total_capacity = current_locked_bytes + requested_size;

        // Use current_total_size_bytes for comparison if it's more accurate, or this->size if it represents capacity
        // Problem statement implies this->size is capacity, so we use that.
        if (this->size < required_total_capacity) {
            size_t old_size = this->size;
            expandHeap(required_total_capacity);

            if (this->size <= old_size) {
                return nullptr;
            }
        } else {
            // Capacity sufficient, but no contiguous block (fragmentation)
            return nullptr;  // Cannot fulfill request
        }
    }  // End of while(true)
}

// Free memory block starting at p
void Heap::Free(MM_Struct* p) {
    size_t const unit_size = sizeof(int);  // Should be 4

    // Error Check 1: Null Pointer
    if (p == nullptr) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    // Error Check 2: Not a valid start of an allocated block
    if (!p->locked || p->size == 0) {
        if (!p->locked) {
            std::cout << "Segmentation Fault!\n";
        } else {
            std::cout << "Error Free!\n";
        }
        return;
    }

    // --- Valid Free Operation ---
    size_t bytes_to_free = p->size;
    size_t num_units     = bytes_to_free / unit_size;

    // --- Step 1: Find block boundaries and the node before p (prev_p) ---
    MM_Struct* prev_p = nullptr;
    if (p != this->head) {
        MM_Struct* finder = this->head;
        while (finder != nullptr && finder->nxt != p) {
            finder = finder->nxt;
        }
        if (finder == nullptr) {
            std::cout << "Segmentation Fault!\n";  // p is not in the list after head
            return;
        }
        prev_p = finder;
    }

    // Find the last node in the block and validate consistency
    MM_Struct* last_in_block = p;
    MM_Struct* check_node    = p;
    for (size_t i = 0; i < num_units; ++i) {
        if (check_node == nullptr) {
            std::cerr << "Internal Free Error: Block ends prematurely. Heap corrupted?" << std::endl;
            // Don't output standard errors here, this is internal inconsistency.
            return;  // Abort
        }
        if (!check_node->locked || (i > 0 && check_node->size != 0)) {
            std::cerr << "Internal Free Error: Inconsistent node state in block. Heap corrupted?" << std::endl;
            return;  // Abort
        }
        if (i == num_units - 1) {
            last_in_block = check_node;
        }
        check_node = check_node->nxt;
    }
    MM_Struct* node_after_block = last_in_block->nxt;

    // --- Step 2: Reset properties of nodes in the block ---
    MM_Struct* reset_iter = p;
    for (size_t i = 0; i < num_units; ++i) {
        reset_iter->locked = false;
        reset_iter->val    = 0;
        if (i == 0) {
            reset_iter->size = 0;
        }
        reset_iter = reset_iter->nxt;
    }

    // --- Step 3: Detach the block from its current position ---
    bool block_was_entire_list = (prev_p == nullptr && node_after_block == nullptr);

    if (!block_was_entire_list) {
        if (prev_p == nullptr) {
            this->head = node_after_block;
        } else {
            prev_p->nxt = node_after_block;
        }
        if (last_in_block == this->tail) {
            this->tail = prev_p;
        }
    } else {
        this->head = nullptr;
        this->tail = nullptr;
    }

    // --- Step 4: Append the detached block to the end of the list ---
    last_in_block->nxt = nullptr;  // Terminate the detached block

    if (!block_was_entire_list) {
        if (this->head == nullptr) {  // List became empty after detaching
            this->head = p;
            this->tail = last_in_block;
        } else {  // Append to non-empty list
            // Ensure tail pointer is valid before dereferencing
            if (this->tail != nullptr) {
                this->tail->nxt = p;
            } else {
                // This case (tail is null but head is not) shouldn't happen with correct Malloc/Free/Expand
                // If it does, it implies the list was just the detached block starting at head.
                // We handled the 'block_was_entire_list' case, maybe this is redundant?
                // If list had only 1 block which we removed, head became node_after_block.
                // If node_after_block is null, head/tail are now null.
                // If node_after_block is not null, head=node_after_block, tail should point to the end of that remaining list.
                // Let's reconsider the tail update in step 3.
                // If last_in_block was tail, and prev_p was null (block started at head), then tail correctly becomes null.
                // If last_in_block was tail, and prev_p was not null, tail correctly becomes prev_p.
                // So, `this->tail` should be valid here if `head` is not null.
                this->tail->nxt = p;  // Link current tail to start of freed block
            }
            this->tail = last_in_block;  // Update heap's tail
        }
    }
    // If block was entire list, head/tail remain nullptr, nothing to append.
}

// Set the value of a memory unit
void Heap::setval(MM_Struct* p, int x) {
    // Check 1: Is the pointer null?
    if (p == nullptr) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    // Check 2: Is the memory unit allocated (locked)?
    // We assume that 'p' points to a valid MM_Struct node within our heap's list structure.
    // Checking if it exists in the list explicitly would be slow.
    // We rely on the `locked` flag set by Malloc and cleared by Free.
    if (!p->locked) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    // If checks pass, set the value
    p->val = x;
}

// Output the value(s) associated with a memory unit pointer
void Heap::output(MM_Struct* p) {
    size_t const unit_size = sizeof(int);  // Should be 4

    // Check 1: Is the pointer null?
    if (p == nullptr) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    // Check 2: Is the memory unit allocated (locked)?
    if (!p->locked) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    // Pointer is valid and points to an allocated unit. Now determine output format.

    // Case 1: 'p' is the start of an allocated block (size > 0)
    if (p->size > 0) {
        size_t bytes_in_block = p->size;
        size_t num_units      = bytes_in_block / unit_size;
        MM_Struct* current    = p;

        for (size_t i = 0; i < num_units; ++i) {
            // Safety check: ensure we don't go past the end of the list unexpectedly
            if (current == nullptr) {
                std::cerr << "Internal Output Error: Block shorter than indicated by size. Heap corrupted?"
                          << std::endl;
                break;  // Stop outputting
            }
            // Safety check: ensure node is still locked (consistency check)
            if (!current->locked) {
                std::cerr << "Internal Output Error: Node within block became unlocked unexpectedly. Heap corrupted?"
                          << std::endl;
                break;  // Stop outputting
            }

            std::cout << current->val;
            if (i < num_units - 1) {
                std::cout << " ";  // Space between values
            }
            current = current->nxt;  // Move to the next unit in the block
        }
        std::cout << std::endl;  // Newline after the block's values
    }
    // Case 2: 'p' is in the middle of an allocated block (size == 0)
    else {
        // The checks for null and locked already passed.
        // p->size == 0 means it's not the start.
        std::cout << p->val << std::endl;  // Output only this unit's value and a newline
    }
}

// Add necessary private members to heap.h:
/*
#ifndef HEAP_H
#define HEAP_H
#include "mm.h" // Assuming mm.h contains MM_Struct definition
#include <cstddef> // For size_t

class Heap {
private:
    MM_Struct *head;
    MM_Struct *tail; // <<< ADDED (if not already from Task 1)
    size_t size; // Max capacity in bytes

    // Helper function for expanding the heap <<< ADDED (if not already from Task 1)
    void expandHeap(size_t min_required_capacity);

    // You can add other private helpers if needed

public:
    Heap(size_t _size);
    ~Heap();
    size_t GetTotalSize() {return size;}
    MM_Struct *Malloc(size_t size);
    void Free(MM_Struct *p);
    void output(MM_Struct *p); // <<< Implemented
    void setval(MM_Struct *p, int x); // <<< Implemented
};

#endif // HEAP_H
*/
static void test2() {
    Heap* heap = new Heap(12);
    std::cout << heap->GetTotalSize() << std::endl;
    MM_Struct* p = heap->Malloc(8);
    MM_Struct* q = heap->Malloc(4);
    heap->Free(q);
    heap->Free(q);
    heap->Free(p->nxt);
    std::cout << heap->GetTotalSize() << '\n';
    q = heap->Malloc(4);
    std::cout << heap->GetTotalSize() << '\n';
    q = heap->Malloc(4);
    std::cout << heap->GetTotalSize() << '\n';
    delete heap;

    heap = new Heap(12);
    p    = heap->Malloc(12);
    heap->Free(p);
    std::cout << heap->GetTotalSize() << '\n';
    delete heap;
}
int main() {
    Heap* heap = new Heap(12);
    std::cout << heap->GetTotalSize() << std::endl;
    MM_Struct* p = heap->Malloc(12);
    std::cout << heap->GetTotalSize() << std::endl;
    MM_Struct* q = heap->Malloc(4);
    heap->Free(q);
    std::cout << heap->GetTotalSize() << std::endl;
    heap->Free(q);
    std::cout << heap->GetTotalSize() << std::endl;

    delete heap;
}