#include <cmath>
#include <cstring>
#include <iostream>
#include <memory>

struct SliceOutOfBoundError {};

class Slice {
   private:
    std::shared_ptr<int> buf;
    size_t base;
    size_t length;
    size_t capacity;

    Slice(std::shared_ptr<int> shared_buf, size_t b, size_t l, size_t cap)
        : buf(shared_buf), base(b), length(l), capacity(cap) {}

    static void free_buf(int* buf) {
        delete[] buf;
    }

    static std::shared_ptr<int> allocate_buf(size_t size) {
        return std::shared_ptr<int>(new int[size], free_buf);
    }

   public:
    Slice(size_t size, int initial = 0) {
        buf = allocate_buf(size * 2);
        base     = 0;
        length   = size;
        capacity = size * 2;
        int* p   = buf.get();
        for (size_t i = 0; i < size; ++i) {
            p[i] = initial;
        }
    }

    Slice(Slice const& other) {
        if (this == &other) {
            return;
        }
        buf = other.buf;
        base     = other.base;
        length   = other.length;
        capacity = other.capacity;
    }

    static Slice copy_from(Slice const& other) {
        Slice copy(other.length);
        memcpy(copy.buf.get(), other.buf.get() + other.base, other.length * sizeof(int));
        return copy;
    }

    void operator=(Slice const& other) {
        if (this == &other) {
            return;
        }
        buf = other.buf;
        base     = other.base;
        length   = other.length;
        capacity = other.capacity;
    }

    void append(int x) {
        if (base + length >= capacity) {
            size_t new_length            = length + 1;
            size_t new_capacity          = new_length * 2;
            std::shared_ptr<int> new_buf = allocate_buf(new_capacity);

            if (length > 0) {
                memcpy(new_buf.get(), buf.get() + base, length * sizeof(int));
            }

            new_buf.get()[length] = x;

            buf      = new_buf;
            base     = 0;
            capacity = new_capacity;
            length   = new_length;
        } else {
            buf.get()[base + length] = x;
            ++length;
        }
    }

    Slice operator[](std::pair<size_t, size_t> range) {
        size_t l = range.first;
        size_t r = range.second;

        if (l > r || base + r > capacity) {
            throw SliceOutOfBoundError();
        }

        return Slice(this->buf, this->base + l, r - l, this->capacity);
    }

    int& operator[](size_t pos) {
         if (pos >= length) {
            throw SliceOutOfBoundError();
        }
        return buf.get()[base + pos];
    }

    void print() {
        for (int i = 0; i < length; ++i) {
            std::cout << buf.get()[base + i] << ' ';
        }
        std::cout << '\n';
    }

    size_t len() {
        return length;
    }

    size_t cap() {
        return capacity;
    }
};

// 题目背景
// 切片(slice)
// 是一个比动态数组功能更强大的数据类型, 切片在保证数组可以动态扩容的前提下,
//     提供了更加强大的区间交互方式.

//     切片在诸多现代编程语言中都有对应的实现(比如golang, rust, python3等),
//     它们的内部细节各不相同, 我们的任务是实现一个功能类似于golang切片的数据结构,
//     它以 "零拷贝,高效率"著称.感兴趣的同学可以在课余时看这篇博文.但你不了解它也不会影响你完成实验,
//     下面的任务描述会给你详细的指引.

//     任务描述 我们给出了Slice类的定义(你在slice.hpp中可以看到完全相同的内容, 所有已经实现的函数不需要你进行修改)
//     :

//       class Slice {
//    private:
//     std::shared_ptr<int> buf;
//     size_t base;
//     size_t length;
//     size_t capacity;
//     static void free_buf(int* buf) {
//         delete[] buf;
//     }
//     static std::shared_ptr<int> allocate_buf(size_t size) {
//         return std::shared_ptr<int>(new int[size], free_buf);
//     }

//    public:
//     Slice(size_t size, int initial = 0) {
//         // TODO
//     }
//     Slice(Slice const& other) {
//         // TODO
//     }

//     static Slice copy_from(Slice const& other) {
//         // TODO
//     }
//     void operator=(Slice const& other) {
//         // TODO
//     }
//     void append(int x) {
//         // TODO
//     }
//     Slice operator[](std::pair<size_t, size_t> range) {
//         // TODO
//     }
//     int& operator[](size_t pos) {
//         // TODO
//     }
//     void print() {
//         for (int i = 0; i < length; ++i) {
//             std::cout << buf.get()[base + i] << ' ';
//         }
//         std::cout << '\n';
//     }
//     size_t len() {
//         return length;
//     }

//     size_t cap() {
//         return capacity;
//     }
// };
// 其中的成员变量的含义为:

// buf:这个智能指针指向当前切片所操作的底层数组,在需要申请内存的时候,你需要让buf指向你新申请的内存,然后利用buf进行操作.

// 使用智能指针的本意是简化同学们手动管理内存的心智负担,为了避免不熟悉std::shared_ptr的同学遇到太大的障碍,我们已经在Slice类的内部实现了申请空间返回指向这段空间的智能指针的操作.因此,当你需要申请空间时,可以放心地使用allocate_buf函数.

// 当你需要操作内部数组的内容时,可以利用buf.get()函数得到底层数组的裸指针,具体可以看print()函数的实现.

// base:指示切片对应的区间在底层数组中的第一个位置的下标.

// 如果之前没有用过切片可能会对这个成员变量产生疑惑,后面讲到需要实现的功能时应该就能明白它的作用了.print()函数也能帮助你理解这个变量.

// length:指示切片对应的区间的长度.

// capacity:指示底层数组的大小.

// 接下来是你需要实现的函数:

// 构造函数Slice(size_t size, int initial = 0)

// 参数size指示所创建的新切片的大小,initial指示切片中所有元素的初值.

// 为了减少append函数的扩容开销,我们约定:在初始化一个切片的时候,底层数组的容量为其大小的2倍.

// 拷贝构造函数Slice(Slice const &other)

// 从另一个切片构造新切片.

// 需要注意的是,切片遵循"零拷贝"原则,所以在这里构造出的新切片应该和原切片共享底层数组.

// 深拷贝函数static Slice copy_from(Slice const &other)

// 对原切片进行深拷贝,返回新的切片.

// 这个函数返回的切片与原切片不共享底层数组,我们约定新生成的切片的底层数组的容量大小为切片长度的2倍.

// 重载赋值void operator=(Slice const &other)

// 与拷贝构造函数相似,但区别在于赋值操作会使原来切片指向的内容不再被使用.

// 切片追加void append(int x)

// 在切片的尾部加入一个元素.

// 为了效率考虑,当底层数组的容量可以让切片直接在尾部追加新元素时,不需要扩容.

// 否则为当前切片新申请一个足够大小的空间,复制原来的内容到新空间中之后,再在尾部追加新元素.

// 为了减少append函数的扩容开销,我们约定:新申请的底层数组的容量为切片追加元素之后大小的2倍.

// "切片"操作Slice operator[](std::pair<size_t, size_t> range)

// 参数range指示新生成的切片对应于原切片的区间[l, r),返回一个对应于这个区间的切片.

// 注意:这里的l和r都是相对于base的偏移量;这里生成的新切片和拷贝构造函数一样,是和原切片共享底层数组的.

// 如果l和r不合法(l > r或者超出了底层数组的容量),此时需要抛出一个SliceOutOfBoundError(定义已经在slice.hpp中给出).

// 下标访问int& operator[](size_t pos)

// 获取切片中下标为pos的值的引用.

// 注意:这里的pos是相对于base的偏移量.

// 如果pos不合法(超出了切片的大小),此时需要抛出一个SliceOutOfBoundError(定义已经在slice.hpp中给出).

// 举例
// 下标访问与"切片"操作:

// int main() {
//     int n = 10;
//     Slice s1(n);
//     for (int i = 0; i < n; ++i) {
//         s1[i] = i;
//     }
//     Slice s2 = s1[{2, 9}];
//     s1.print();
//     s2.print();

//     return 0;
// }
// 这里我们初始化了一个长度为10的切片s1,然后再用循环初始化s1,之后我们再用"切片"操作获取s1下标区间[2,9)的内容并用它初始化s2,最后打印两个切片的内容.

// 这段代码的期望输出如下:

// 0 1 2 3 4 5 6 7 8 9
// 2 3 4 5 6 7 8
// 赋值操作

// int main() {
//     int n = 10;
//     Slice s1(n);
//     for (int i = 0; i < n; ++i) {
//         s1[i] = i;
//     }
//     Slice s2(1);
//     s2.print();
//     s2 = s1;
//     s2.print();
//     s1[3] = 114514;
//     s2.print();

//     return 0;
// }
// 这里我们初始化了一个长度为10的切片s1,然后再用循环初始化s1,之后初始化了另一个切片s2,后面又用s1给s2进行赋值,这时两个切片应该共享底层数组,所以s1的操作s2是可见的.

// 这段代码的期望输出如下:

// 0
// 0 1 2 3 4 5 6 7 8 9
// 0 1 2 114514 4 5 6 7 8 9
// 切片追加

// int main() {
//     Slice s1(2);
//     std::cout << s1.cap() << '\n';
//     s1.append(1);
//     s1.append(1);
//     s1.append(1);
//     std::cout << s1.cap() << '\n';
//     s1.print();
//     return 0;
// }
// 这里我们初始化了一个长度为2的切片,随后连续追加了3次元素,导致其自动进行了一次扩容.

// 这段代码的期望输出如下:

// 4
// 10
// 0 0 1 1 1
// 抛出异常

// int main() {
//     Slice s1(10);

//     try {
//         s1[{3, 100}].print();
//     } catch (SliceOutOfBoundError e) {
//         std::cout << "slice operation out of bound!\n";
//     }
//     try {
//         s1[114514] = 1919810;
//     } catch (SliceOutOfBoundError e) {
//         std::cout << "aaaaaaa!\n";
//     }
//     return 0;
// }
// 这里进行"切片"操作和下标访问的时候都发生了越界,所以需要抛出对应的异常.

// 这段代码的期望输出如下:

// slice operation out of bound!
// aaaaaaa!
// 深拷贝

// int main() {
//     Slice s1(10);
//     for (int i = 0; i < 10; ++i) {
//         s1[i] = i;
//     }
//     Slice s2 = Slice::copy_from(s1);
//     s2[0]    = 114514;
//     s1.print();
//     s2.print();
//     return 0;
// }
// 明确深拷贝的两个切片是不会共享底层数组的.

// 这段代码的期望输出如下:

// 0 1 2 3 4 5 6 7 8 9
// 114514 1 2 3 4 5 6 7 8 9
// 补充说明
// 本题主要考察对应的功能实现,不追求极致的实现效率.

// 因为部分特性(比如扩容)的实现是灵活的,为了便于测评,我们对这些特性进行了约定,部分测试样例会基于我们约定的特性进行测评,所以遇到有时候感觉自己实现没有问题但是过不了样例时可以再读一遍题目要求.

// 另外,顺利实现这个数据结构之后,将其作为动态扩容容器的模板或许可以极大地方便你后续数据结构课程的学习.