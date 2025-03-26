#include <iostream>
using std::cin;
using std::cout;
using std::endl;

class rectangle {
    private:
    double breath;
    double length;

    public:
    rectangle(){
        breath = 1;
        length = 1;
    }
    rectangle(double b, double l):breath(b) ,length(l){}

    double area(){
        return breath * length;
    }

    rectangle operator*(const double &times){
        return rectangle(breath * times, length * times);
    }

    friend std::ostream& operator<<(std::ostream& os, rectangle const& rect) {
        os << "Rectangle(breath: " << rect.breath << ", length: " << rect.length << ")";
        return os;
    }
};
