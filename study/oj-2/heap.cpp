#include"heap.hpp"

#include"MM.hpp"

#include<cstddef>
#include<iostream>

using std::cout;

Heap::Heap(size_t _size) {
    //TODO
    if (_size < 4) {
        return;
    }
    Heap::size = _size;
    Heap::head = new MM_Struct();
    for (int i = 1; i < _size / 4; i++) {
        MM_Struct* temp = new MM_Struct();
        temp->nxt       = Heap::head;
        Heap::head      = temp;
    }
}
Heap::~Heap() {
    //TODO
    MM_Struct* temp = head;
    while (temp != nullptr) {
        MM_Struct* next = temp->nxt;
        delete temp;
        temp = next;
    }
    head = nullptr;
}
void Heap::dilatation() {
    //TODO
    MM_Struct* temp = Heap::head;
    while (temp->nxt != nullptr) {
        temp = temp->nxt;
    }
    size_t old_size = Heap::size;
    Heap::size *= 2;
    for (int i = 0; i < old_size / 4; i++) {
        MM_Struct* next = new MM_Struct();
        temp->nxt       = next;
        temp            = next;
    }
}
MM_Struct* Heap::Malloc(size_t target_size) {
    //TODO
    int targrt = target_size / 4;
    MM_Struct* begin   = head;
    MM_Struct* check   = nullptr;
    while (true) {
        while (begin != nullptr) {
            if (begin->locked == true) {
                begin = begin->nxt;
                continue;
            }
            check = begin;
            for (int i = 1; i < targrt; i++) {
                if (check->nxt == nullptr || check->locked == true) {
                    begin = check->nxt;
                    check   = nullptr;
                    break;
                }
                check = check->nxt;
            }
            if (check != nullptr) {
                begin->size     = target_size;
                MM_Struct* temp = begin;
                for (int i = 1; i <= targrt; i++) {
                    temp->locked = true;
                    temp         = temp->nxt;
                }
                return begin;
            }
            begin = begin->nxt;
        }
        dilatation();
        begin = head;
        check  = nullptr;
    }
}
void Heap::Free(MM_Struct* p) {
    //TODO
    if (p == nullptr) {
        cout << "Segmentation Fault!\n";
        return;
    }
    if (p->locked == false) {
        cout << "Segmentation Fault!\n";
        return;
    }
    if (p->size == 0 && p->locked == true) {
        cout << "Error Free!\n";
        return;
    }
    if (p == head){
        MM_Struct* p_end = p;
        int length = p->size / 4;
        for (int i = 1; i < length; i++) {
            p_end = p_end->nxt;
            p_end->locked = false;
            p_end->val = 0;
        }
        p->locked = false;
        p->val = 0;
        p->size = 0;
        
        if (p_end->nxt == nullptr) {
            return;
        }

        head = p_end->nxt;
        MM_Struct* end = head;
        while (end->nxt != nullptr) {
            end = end->nxt;
        }
        end->nxt = p;
        p_end->nxt = nullptr;
        return;
    }
    MM_Struct* pre = head;
    while (pre->nxt != nullptr && pre->nxt != p) {
        pre = pre->nxt;
    }
    MM_Struct* p_end  = p;
    int length = p->size / 4;
    for (int i = 1; i < length; i++) {
        p_end = p_end->nxt;
        p_end->locked = false;
        p_end->val = 0;
    }
    p->locked = false;
    p->val = 0;
    p->size = 0;
    if (p_end->nxt == nullptr) {
        return;
    }
    pre->nxt = p_end->nxt;
    MM_Struct* end = head;
    while (end->nxt != nullptr){
        end = end->nxt;
    }
    end->nxt = p;
    p_end->nxt = nullptr;
}


void Heap::output(MM_Struct* p) {
    //TODO
    if (p == nullptr) {
        cout << "Segmentation Fault!\n";
        return;
    }
    if (p->locked == false) {
        cout << "Segmentation Fault!\n";
        return;
    }
    if (p->size != 0){
        int length = p->size / 4;
        MM_Struct* temp = p;
        for (int i = 0; i < length; i++) {
            cout << temp->val << ' ';
            temp = temp->nxt;
        }
        cout << '\n';
    }
    else {
        cout << p->val << '\n';
    }
}
void Heap::setval(MM_Struct* p, int x) {
    //TODO
    if (p == nullptr) {
        cout << "Segmentation Fault!\n";
        return;
    }
    if (p->locked == false) {
        cout << "Segmentation Fault!\n";
        return;
    }
    p->val = x;}
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
int main(){
    Heap* heap   = new Heap(12);
    MM_Struct* p = heap->Malloc(12);
    heap->setval(p, 1);
    heap->setval(p->nxt, 2);
    heap->setval(p->nxt->nxt, 3);
    heap->output(p);
    heap->output(p->nxt);
    heap->output(nullptr);
    delete heap;
}