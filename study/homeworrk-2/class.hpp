// 请在测评文件的Class.cpp中完成你的实现
#include <iostream>
#include <string>

/* 加入你需要的头文件 */
#include <limits>  // 需要包含 limits 以使用 numeric_limits
#include <vector>

using namespace std;

class Class;
struct Function;
struct CallSite;
bool call(CallSite const& cs);

struct Function {
    std::string name;
    Class const& arg;                           // 仍然需要前向声明的 Class
    bool operator==(Function const& f_) const;  // 声明，定义可以在 cpp 中或后面
    bool operator<(Function const& f_) const;   // 声明，定义可以在 cpp 中或后面
};

struct CallSite {
    Class const& base;
    string function_name;
    Class const& arg;
};

class Class {
   private:
    string name;
    Class const* parent;
    /* 你可以使用其他的数据结构来存储成员函数，也可以添加其他成员变量 */
    vector<Function> funcs;

   public:
    Class(string const& name_) {
        name   = name_;
        parent = nullptr;
    }
    bool operator==(Class const& c) const {
        return name == c.name;
    }
    string get_name() const {
        return name;
    }

    /* 需要实现的接口,不可修改 */
    void add_function(Function const& f);
    // 实验保证任何子类的 inherit 操作都会在类构造之后立刻进行，且不会多次调用
    void inherit(Class const& c);
    friend bool call(CallSite const& cs);

    // 仅供参考，不需要实现的话注释掉即可
    bool is_ancestor(Class const& c) const;
    friend int get_ancestor_depth(Class const& ancestor, Class const& descendant);
};

inline bool Function::operator==(Function const& f_) const {
    // 注意：这里需要 Class::get_name()，它在 Class 定义中已经声明
    return name == f_.name && arg.get_name() == f_.arg.get_name();
}

inline bool Function::operator<(Function const& f_) const {
    // 注意：这里需要 Class::get_name()
    return name < f_.name || (name == f_.name && arg.get_name() < f_.arg.get_name());
}
// 任务描述
// 小蓝鲸正在复习高级程序设计课程中的类成员函数静态绑定相关知识，小蓝鲸想知道，如何系统化、程序化地确认在某个调用点应该调用哪个类的哪个函数呢？

// class X {
//    public:
//     void func(string);
// };
// class Y : public X {};
// int main() {
//     Y y;
//     y.func("hello");
//     y.func(10);  //error !
// }
// 对上述代码，当我们敲下y.func("hello");
// 时，我们知道它将会调用X::func(string)，而当我们敲下y.func(10);
// 时，你的IDE就会在下面划上红色波浪线，不需要真正执行这段代码，我们就能确定这些调用点将要执行的函数，这就是静态绑定。

// 于是，小蓝鲸打算使用CPP写一个简单的CPP模拟器，模拟静态绑定的过程。

// 代码框架
// 类的类
// 首先我们需要一个类来表示“类”。

// class Class {
//    private:
//     string name;
//     Class const* parent;
//     /* 你可以使用其他的数据结构来存储成员函数，也可以添加其他成员变量 */
//     vector<Function> funcs;

//    public:
//     /* 需要实现的接口,不可修改 */
//     void add_function(Function const& f);
//     void inherit(Class const& c);  // 在构造Class时,如果有父类,那么inherit是第一步
//     // bool is_ancestor(const Class &c) const;
//     friend bool call(CallSite cs);
// };
// 这里的Class类就是CPP中的所有class的集合，Class的每个对象都表示CPP中的一个类。

//     例如，对于这样的代码 :

//     Class X("X");
// Class Y("Y");
// 就表示在我们的模拟器世界中，存在两个类X, Y，他们的名字也是
//     "X"
//     "Y"，等价于下面的CPP代码

//     class X {};
// class Y {};
// 继承
// 当我们调用一个Class对象(例如X) 的inherit函数时，需要传入另一个Class对象c作为参数，表示在模拟器世界中，类X的基类是c(
//     inherit函数在样例中会紧跟着该类的构造函数，当我们构造一个类时，首先确定它的基类)。 例如，对于下面的CPP代码，

//     class A {};
// class B : public A {};
// class C : public B {};
// 使用inherit函数可以在我们的模拟器世界中达成一样的效果 :

//     Class A("A");
// Class B("B");
// B.inherit(A);
// Class C("C");
// C.inherit(B);
// 注意，本题中不涉及多继承和虚继承。

// 成员函数
// 你可能注意到，Class类中有这样一个接口void add_function(Function const& f);
// ，其中Function是一个结构体 :

//     struct Function {
//     string name;
//     Class const& arg;
// };
// 由于CPP中的函数重载和成员函数覆盖均不涉及返回值，所以我们只用函数名和参数类型来标识一个函数。并且本题中所有函数有且只有一个参数，参数的类型也是Class的一个对象。
//     例如，对于函数void
//     func(A a);
// ，在我们的模拟器世界中，我们用下面的代码来代替。

//     Class A("A");
// Function f = {"func", A};
// 当我们需要为模拟器中的类添加成员函数时，我们需要调用add_function接口。例如，对于下面的CPP类 :

//     class A {};
// class X {
//    public:
//     void func(A) {
//         cout << "X::func(A)" << endl;
//     }
// };
// 在模拟器中用如下代码构造 :

//     Class A("A");
// Class X("X");
// X.add_function({"func", A});
// is_ancestor 此接口仅作为提示，在样例中不会有任何考察。 该函数接收一个参数const Class& c，返回一个bool，表示参数类型 c
//     是否是当前类型的祖先。 你可以随意更改此接口，比如将返回值修改为 int 类型，表示两个类型之间的层数。
//         一个可能的实现结果是：

//             Class X("X");
// Class A("A");
// Class B("B");
// B.inherit(A);
// Class C("C");
// C.inherit("B");
// //C继承B，B继承A
// C.is_ancestor(B);  // true
// C.is_ancestor(A);  // true
// X.is_ancestor(A);  // false
// call 这是本题中你需要实现的核心函数，它的参数是一个CallSite结构体cs，表示一个具体的调用点，结构如下 :

//     struct CallSite {
//     Class const& base;
//     string function_name;
//     Class const& arg;
// };
// 一个调用点由三部分组成：调用成员函数的对象的类型、函数名和函数唯一的参数的类型（调用时不会出现类名限定，例如x.Y::func()
//     这种形式）。 例如，对于下面的CPP代码 :

//     class A {};
// class X {
//    public:
//     void func(A) {
//         cout << "X::func(A)" << endl;
//     }
// };
// int main() {
//     X x;
//     x.func(A());  // CallSite cs
//     return 0;
// }
// 我们使用如下的代码模拟 :

//     Class A("A");
// Class X("X");
// X.add_function({"func", A});
// CallSite cs = {X, "func", A};
// call(cs);  //等价于x.func(A());
// // 输出: X::func(A)
// 值得注意的是，本题中的Class只模拟类的成员函数，不关心类的成员变量（甚至不关心成员函数的内容，只关心它所属的类、函数名和参数类型），所以
//     X x;
// 不需要进行模拟。
// 在调用 call(cs)
// 时，我们只需要知道“有一个X类型的对象调用了成员函数 func，并且参数类型是 A”，然后找到 X::func(A)
//     具体绑定到哪个类(X或X的父类) 的哪个成员函数，然后输出对应的信息。 因为X类中添加了名为
//     "func"，参数类型为 A 的成员函数，所以该 CallSite cs 静态绑定到函数 X::func(A)。 需要在 call 中输出的信息是
//     : "<静态绑定到的成员函数定义所在类>::<函数名>(<该函数形参的类型>)"。如果找不到 cs 调用的函数，那需要输出
//     : Function not found.

//       注意：call函数的输出末尾需要换行。

//       call 函数的返回值不会在 OJ 中进行检查，你可以随意定义返回值，或者修改函数的返回类型。

//       任务要求和实现思路 你需要至少实现以下三个接口，你可以在 Class 类中任意添加你需要的函数或者成员变量。

//       /* 需要实现的接口,不可修改 */
//       void add_function(Function const& f);
// void inherit(Class const& c);
// friend bool call(CallSite cs);
// 在实现 add_function 和 inherit 时，你可以简单地将成员函数加入 Class 中的 vector，将 parent
//     指针指向父类，也可以在这两个函数中维护你自己添加的数据结构。

//         在实现 call 函数实现时，一个推荐的思路是，在cs.base中搜索函数名符合cs.function_name、函数参数类型符合 cs
//             .arg 的成员函数，如果搜索不到，那么向cs
//             .base的parent指针指向的类中搜索，进行循环迭代，如果最终搜索不到，则输出
//     "Function not found."。 作为提示，不妨考虑函数匹配时的几个情况：如果有多个可以匹配的函数应该绑定哪一个？子类定义基类同名函数会对搜索过程产生什么影响？

//         样例 以下两个样例中，我们默认存在一个全局的类层次关系 :

//     // A->B->C
//     样例1
//     /*
// Naive测试，结果等价于下列代码:
// class A{};
// class X{
// public:
//     void func(A){cout<<"X::func(A)"<<endl;};
// };
// int main(){
//     X x;
//     x.func(A());
//     return 0;
// }
// 输出:
// X::func(A)
// */
//     void
//     test1() {
//     Class X("X");
//     X.add_function({"func", A});
//     call({X, "func", A});
// }
// 样例2
// /*
// 测试函数重载、继承:
// class A{};
// class B:public A{};
// class X{
// public:
//     void func(A){cout<<"X::func(A)"<<endl;};
//     void func(B){cout<<"X::func(B)"<<endl;};
// };
// class Y{
// public:
//     void func(A){cout<<"Y::func(A)"<<endl;}
// };
// int main(){
//     X x;
//     x.func(A());
//     x.func(B());
//     Y y;
//     y.func(A());
//     y.func(B());
//     return 0;
// }
// 输出:
// X::func(A)
// X::func(B)
// Y::func(A)
// Y::func(A)
// */
// void test3() {
//     Class A("A");
//     Class B("B");
//     B.inherit(A);
//     Class C("C");
//     C.inherit(B);

//     Class X("X");
//     X.add_function({"func", A});
//     X.add_function({"func", B});
//     Class Y("Y");
//     Y.add_function({"func", A});
//     call({X, "func", A});
//     call({X, "func", B});
//     call({Y, "func", A});
//     call({Y, "func", B});
// }
// side note 平台默认编译选项为 - std = gnu++ 14，请不要使用 c++ 14 后支持的特性 最后，祝你身体健康，再见。