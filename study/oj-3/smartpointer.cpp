#include "SmartPointer.hpp"

//拷贝构造函数
SmartPointer::SmartPointer(SmartPointer const& sptr) {
    //拷贝构造时是新对象，不需要释放资源
    pointer = sptr.pointer;
    ref_cnt = sptr.ref_cnt;
    if (ref_cnt != nullptr) {
        (*ref_cnt)++;
    }
}

//该函数接收一个SmartPointer对象sptr，并将本对象的pointer指针赋值为sptr.pointer。
void SmartPointer::assign(SmartPointer const& sptr) {
    if (pointer == sptr.pointer) {
        return;
    }
    //释放当前对象管理的资源
    if (pointer != nullptr && ref_cnt != nullptr) {
        (*ref_cnt)--;
        if (*ref_cnt == 0) {
            delete pointer;
            delete ref_cnt;
        }
    }
    pointer = sptr.pointer;
    ref_cnt = sptr.ref_cnt;
    //增加引用计数，需要确保ref_cnt不为nullptr
    if (ref_cnt != nullptr) {
        (*ref_cnt)++;
    }
}

SmartPointer::~SmartPointer() {
    //TODO
    if (pointer != nullptr && ref_cnt != nullptr) {
        (*ref_cnt)--;
        if (*ref_cnt == 0) {
            delete pointer;
            delete ref_cnt;
        }
    }
    pointer = nullptr;
    ref_cnt = nullptr;
}

int main() {
    SmartPointer sp1(new Node(111));  // sp1 管理 Node(1)，ref_cnt = 1
    SmartPointer sp2(new Node(222));  // sp2 管理 Node(2)，ref_cnt = 1
    sp2.assign(sp1);                  // sp2 原本管理 Node(2)，其引用计数将减为 0，析构时输出2；
    // sp2 现在指向 Node(1)，和 sp1 共享，ref_cnt = 2
    return 0;
}