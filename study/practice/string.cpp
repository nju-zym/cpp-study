#include <iostream>
#include <string>

class string{
    private:
    char* str;
    
    public:
    string() : str(nullptr){
    }
    explicit string (const char* s) : str(new char[strlen(s) + 1]){
        strcpy(str, s);
    }
    string (const string& s) : str(new char[strlen(s.str) + 1]){
        strcpy(str, s.str);
    }
    ~string(){
        delete[] str;
    }
    string& operator=(const string& s){
        if (this == &s) {
            return *this;
        }
        delete[] str;
        str = new char[strlen(s.str) + 1];
        strcpy(str, s.str);
        return *this;
    }
    friend string operator+(const string& s1, const string& s2){
        string s;
        s.str = new char[strlen(s1.str) + strlen(s2.str) + 1];
        strcpy(s.str, s1.str);
        strcat(s.str, s2.str);
        return s;
    }
    friend std::ostream& operator<<(std::ostream& os, const string& s){
        os << s.str;
        return os;
    }
};
int main() {
    string s1("Hello, ");
    string s2("World!");
    string s3 = s1 + s2;  // 调用 operator+
    std::cout << s3 << std::endl;
    return 0;
}