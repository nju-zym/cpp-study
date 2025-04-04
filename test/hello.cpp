#include <compare>
#include <iostream>
#include <string>

class Person {
   public:
    std::string name;
    int age;

    // 三方比较运算符重载
    std::strong_ordering operator<=>(Person const& other) const {
        // 先比较姓名
        if (auto cmp = name <=> other.name; cmp != 0) {
            return cmp;  // 姓名不同，返回比较结果
        }
        // 姓名相同，则比较年龄
        return age <=> other.age;
    }
};

int main() {
    Person alice{"Alice", 30};
    Person bob{"Bob", 25};
    Person anotherAlice{"Alice", 25};

    // 使用三方比较
    std::cout << (alice < bob) << std::endl;                  // 输出 1
    std::cout << (bob < anotherAlice) << std::endl;           // 输出 0
    std::cout << (alice <=> anotherAlice == 0) << std::endl;  // 输出 0 (false, because ages are different)

    return 0;
}