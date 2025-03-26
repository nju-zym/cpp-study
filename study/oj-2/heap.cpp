#include "heap.hpp"

#include <iostream>

Heap::Heap(size_t _size) {
    size = _size;
    head = new MM_Struct[size / 4];
    for (size_t i = 0; i < (size / 4); i++) {
        head[i].locked = false;
        head[i].size   = 0;
        head[i].val    = 0;
    }

    for (size_t i = 0; i < (size / 4) - 1; i++) {
        head[i].nxt = &head[i + 1];
    }
    head[(size / 4) - 1].nxt = nullptr;
}

Heap::~Heap() {
    if (head != nullptr) {
        delete[] head;
    }
    size = 0;
    head = nullptr;
}
MM_Struct* Heap::Malloc(size_t requested_size) {
    size_t units_needed = (requested_size + 3) / 4;

    while (true) {
        MM_Struct* start   = nullptr;
        MM_Struct* current = head;
        MM_Struct* prev    = nullptr;
        MM_Struct* last    = nullptr;

        while (current != nullptr) {
            if (current->nxt == nullptr) {
                last = current;
            }
            if (current->locked) {
                prev    = current;
                current = current->nxt;
                continue;
            }
            size_t count    = 0;
            MM_Struct* temp = current;
            while (temp != nullptr && !temp->locked && count < units_needed) {
                count++;
                temp = temp->nxt;
            }
            if (count >= units_needed) {
                start = current;
                break;
            }
            prev    = current;
            current = current->nxt;
        }
        if (start != nullptr) {
            start->locked   = true;
            start->size     = requested_size;
            MM_Struct* temp = start->nxt;
            for (size_t i = 1; i < units_needed; i++) {
                temp->locked = true;
                temp->size   = 0;
                temp         = temp->nxt;
            }
            return start;
        }
        size_t old_size       = size;
        size                 *= 2;
        MM_Struct* new_units  = new MM_Struct[size / 4 - old_size / 4];  // 只分配新增的部分

        for (size_t i = 0; i < (size / 4 - old_size / 4); i++) {
            new_units[i].locked = false;
            new_units[i].size   = 0;
            new_units[i].val    = 0;
        }

        for (size_t i = 0; i < (size / 4 - old_size / 4) - 1; i++) {
            new_units[i].nxt    = &new_units[i + 1];
            new_units[i].locked = false;
        }
        new_units[(size / 4 - old_size / 4) - 1].nxt    = nullptr;
        new_units[(size / 4 - old_size / 4) - 1].locked = false;

        if (last == nullptr) {
            head = new_units;
        } else {
            last->nxt = &new_units[0];
        }
    }
}
void Heap::Free(MM_Struct* p) {
    if (p == nullptr || !p->locked) {
        std::cout << "Segmentation Fault!\n";
        return;
    }
    if (p->size == 0) {
        std::cout << "Error Free!\n";
        return;
    }

    size_t units_to_free = (p->size + 3) / 4;

    MM_Struct* start = p;
    MM_Struct* end   = p;
    for (size_t i = 1; i < units_to_free && end->nxt != nullptr; i++) {
        end = end->nxt;
    }
    MM_Struct* after_end = end->nxt;

    bool contains_last = false;
    if (after_end == nullptr) {
        contains_last = true;
    }

    MM_Struct* prev = nullptr;
    MM_Struct* curr = head;
    while (curr != nullptr && curr != p) {
        prev = curr;
        curr = curr->nxt;
    }

    curr = start;
    while (curr != after_end) {
        curr->locked = false;
        curr->val    = 0;
        curr->size   = 0;
        curr         = curr->nxt;
    }

    if (!contains_last) {
        if (prev != nullptr) {
            prev->nxt = after_end;
        } else {
            head = after_end;
        }

        MM_Struct* last = head;
        while (last != nullptr && last->nxt != nullptr) {
            last = last->nxt;
        }

        if (last != nullptr) {
            last->nxt = start;
            end->nxt  = nullptr;
        } else {
            head     = start;
            end->nxt = nullptr;
        }
    } else if (prev != nullptr) {
        prev->nxt = start;
    }
}
void Heap::setval(MM_Struct* p, int val) {
    if (p == nullptr || !p->locked) {
        std::cout << "Segmentation Fault!\n";
        return;
    }

    p->val = val;
}
void Heap::output(MM_Struct* p) {
    if (p == nullptr || !p->locked) {
        std::cout << "Segmentation Fault!\n";
        return;
    }
    if (p->size > 0) {
        size_t units    = p->size / 4;
        MM_Struct* curr = p;

        for (size_t i = 0; i < units; i++) {
            std::cout << curr->val;
            if (i < units - 1) {
                std::cout << " ";
            }
            curr = curr->nxt;
        }
        std::cout << std::endl;
    } else {
        std::cout << p->val << std::endl;
    }
}
// check the expanding of the Heap and some corner cases of Free method;
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
    test2();
    return 0;
}