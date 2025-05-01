#include "class.hpp"

// --- 辅助函数：计算祖先深度 ---
// 返回 ancestor 是 descendant 的祖先深度 (0=自身, 1=父, ...), 或 -1 如果不是祖先
int get_ancestor_depth(Class const& ancestor, Class const& descendant) {
    Class const* current = &descendant;
    int depth            = 0;
    while (current != nullptr) {
        // 使用指针比较来检查是否是继承链上的同一个对象实例
        if (current == &ancestor) {
            return depth;
        }
        current = current->parent;
        depth++;
    }
    return -1;  // 不是祖先
}

void Class::add_function(Function const& f) {
    funcs.push_back(f);
}

void Class::inherit(Class const& c) {
    parent = &c;
}

// --- 修改后的 call 函数 ---
bool call(CallSite const& cs) {  // 参数改为 const&
    Class const* current = &cs.base;
    while (current != nullptr) {
        Function const* exact_match         = nullptr;
        Function const* best_ancestor_match = nullptr;
        // 初始化为最大值，以便任何有效深度都能替换它
        int min_ancestor_depth = std::numeric_limits<int>::max();
        bool found_name        = false;

        // 在当前类 'current' 中查找最佳匹配
        for (auto const& f : current->funcs) {
            if (f.name == cs.function_name) {
                found_name = true;  // 在当前类找到同名函数
                if (f.arg == cs.arg) {
                    // 精确匹配永远是最佳选择
                    exact_match = &f;
                    break;  // 找到精确匹配，无需再在此类中查找
                } else {
                    // 检查是否为祖先匹配，并计算深度
                    int depth = get_ancestor_depth(f.arg, cs.arg);
                    if (depth != -1) {  // 是祖先 (或自身)
                        // 如果是第一个祖先匹配，或者比当前最佳祖先匹配更近（深度更小）
                        if (best_ancestor_match == nullptr || depth < min_ancestor_depth) {
                            best_ancestor_match = &f;
                            min_ancestor_depth  = depth;
                        }
                    }
                }
            }
        }

        // 根据在 'current' 类中的查找结果做决定
        if (exact_match != nullptr) {
            // 优先使用精确匹配
            std::cout << current->get_name() << "::" << exact_match->name << "(" << exact_match->arg.get_name() << ")"
                      << std::endl;
            return true;
        } else if (best_ancestor_match != nullptr) {
            // 没有精确匹配，但找到了最佳祖先匹配
            std::cout << current->get_name() << "::" << best_ancestor_match->name << "("
                      << best_ancestor_match->arg.get_name() << ")" << std::endl;
            return true;
        } else if (found_name) {
            // 在当前类找到了名字，但没有匹配的参数（名称隐藏）
            std::cout << "Function not found." << std::endl;
            return false;  // 查找失败，不再向上查找
        }

        // 在当前类中未找到该函数名，继续去父类查找
        current = current->parent;
    }

    // 遍历完整个继承链仍未找到函数名
    std::cout << "Function not found." << std::endl;
    return false;
}

bool Class::is_ancestor(Class const& c) const {
    // 这个函数现在可以基于 get_ancestor_depth 实现，或者保持原样
    // return get_ancestor_depth(*this, c) != -1;
    Class const* current_c = &c;
    while (current_c != nullptr) {
        if (current_c == this) {
            return true;
        }
        current_c = current_c->parent;
    }
    return false;
}

/*
Naive测试，结果等价于下列代码:
class A{};
class X{
public:
    void func(A){cout<<"X::func(A)"<<endl;};
};
int main(){
    X x;
    x.func(A());
    return 0;
}
输出:
X::func(A)
*/
void test1() {
    Class X("X");
    Class A{"A"};
    X.add_function({.name = "func", .arg = A});
    call({.base = X, .function_name = "func", .arg = A});
}

void test3() {
    Class A("A");
    Class B("B");
    B.inherit(A);
    Class C("C");
    C.inherit(B);

    Class X("X");
    X.add_function({"func", A});
    X.add_function({"func", B});
    Class Y("Y");
    Y.add_function({"func", A});
    call({X, "func", A});
    call({X, "func", B});
    call({Y, "func", A});
    call({Y, "func", B});
}

int main() {
    test3();
}