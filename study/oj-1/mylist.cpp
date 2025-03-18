#include "mylist.hpp"

#include <iostream>

using namespace std;

// 构造函数
MyList::MyList() : head(nullptr), list_size(0) {}

// 析构函数
MyList::~MyList() {
    //析构，但不调用clean（），我们不希望有输出
    Node* p = head;
    while (p != nullptr) {
        Node* q = p->next;
        delete p;
        p = q;
    }
    list_size = 0;
    head      = nullptr;
}

// 获取指定位置的节点
MyList::Node* MyList::getNodeAt(int index) const {
    //TODO
    //如果index不合法:
    //cout<<-1<<endl;
    if (index < 0 || index >= list_size) {
        cout << -1 << '\n';
        return nullptr;
    }
    Node* p = head;
    for (int i = 0; i < index; i++) {
        p = p->next;
    }
    return p;
}

// 插入操作
void MyList::insert(int index, int value) {
    //TODO
    //如果index不合法:
    //cout<<-1<<endl;
    if (index < 0 || index > list_size) {
        cout << -1 << '\n';
        return;
    }
    Node* new_node = new Node(value);
    if (index == 0) {
        new_node->next = head;
        head           = new_node;
    } else {
        Node* p        = getNodeAt(index - 1);
        new_node->next = p->next;
        p->next        = new_node;
    }
    list_size++;
}

// 删除操作
void MyList::remove(int index) {
    //TODO
    //如果index不合法:
    //cout<<-1<<endl;
    if (index < 0 || index >= list_size) {
        cout << -1 << '\n';
        return;
    }
    if (index == 0) {
        Node* p = head;
        head    = head->next;
        delete p;
    } else {
        Node* p = getNodeAt(index - 1);
        Node* q = p->next;
        p->next = q->next;
        delete q;
    }
    list_size--;
}

// 获取指定位置的元素
int MyList::get(int index) const {
    //TODO
    //如果index不合法,返回-1
    if (index < 0 || index >= list_size) {
        return -1;
    }
    Node* p = getNodeAt(index);
    return p->value;
}

// 设置指定位置的元素
void MyList::set(int index, int value) {
    //TODO
    //如果index不合法:
    //cout<<-1<<endl;
    if (index < 0 || index >= list_size) {
        cout << -1 << '\n';
        return;
    }
    Node* p  = getNodeAt(index);
    p->value = value;
}

// 获取线性表的大小
int MyList::size() const {
    //TODO
    //空就返回0
    return list_size;
}

// 清空线性表
void MyList::clear() {
    //TODO
    cout << "Cleared" << endl;  //不要修改
    Node* p = head;
    while (p != nullptr) {
        Node* q = p->next;
        delete p;
        p = q;
    }
    list_size = 0;
    head      = nullptr;
}

// 查询从 index 开始的前 k 个元素的和
int MyList::sum_from(int index, int k) const {
    //TODO
    //比如1，2，3，4，5，6 sum（0，2） 就是1+2=3
    //如果操作不合法,返回-1
    if (index < 0 || index >= list_size || k <= 0 || index + k > list_size) {
        return -1;
    }
    int sum = 0;
    Node* p = getNodeAt(index);
    for (int i = 0; i < k; i++) {
        sum += p->value;
        p    = p->next;
    }
    return sum;
}

// 反转链表
void MyList::reverse() {
    //TODO
    //注意指针的操作，我们不希望你只是交换值
    Node* p = head;
    Node* q = nullptr;
    while (p != nullptr) {
        Node* r = p->next;
        p->next = q;
        q       = p;
        p       = r;
    }
    head = q;
}

void MyList::merge(MyList& other) {
    //TODO
    //如果当前链表为空，直接连接
    if (head == nullptr) {
        head = other.head;
    } else {
        Node* p = head;
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = other.head;
    }
    list_size       += other.list_size;
    other.head       = nullptr;
    other.list_size  = 0;
}

//可以用这个main函数本地测试！或者修改内容~
//提交时候务必注释哦~
int main(){
  MyList L;
  L.insert (0, 10);
  L.insert (1, 20);
  L.insert (2, 30);
  cout<<L.get(0)<<endl;
  cout<<L.get(1)<<endl;
  L.set (1, 25);
  cout<<L.get(1)<<endl;
  cout<<L.size()<<endl;
  L.remove (1);
  cout<<L.get(1)<<endl;
  cout<<L.sum_from (0, 2)<<endl;
  L.reverse();
  cout<<L.get(0)<<endl;
}
