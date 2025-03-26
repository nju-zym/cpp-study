#ifndef MM_HPP
#define  MM_HPP
#include  <iostream>
struct MM_Struct {
    bool locked;
    size_t size;
       //如果是一段连续分配内存的开头，就保存这段连续内存
        int val;
    MM_Struct * nxt;
    MM_Struct() {
        locked = false;
        val    = 0;
        nxt    = nullptr;
        size   = 0;
    }
    ~MM_Struct() {
        std:: cout <<"Deleting MM_Struct!\n";
    }
};
#endif