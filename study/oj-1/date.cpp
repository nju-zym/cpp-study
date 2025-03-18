#include "date.hpp"

#include <iostream>
#include <memory>
using namespace std;

// 构造函数
Date::Date(int y, int m, int d) : year(y), month(m), day(d) {
    if (!isValidDate()) {
        //不需修改，我们希望看到某些异常日期测试用例对应的“-1”输出
    }
}

// 判断日期是否合法
bool Date::isValidDate() const {
    //TODO
    //这个函数需要被其他函数使用
    //未到日期也属于正常输入
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)) {
        return false;
    }
    return true;
}

// 判断是否是闰年
bool Date::isLeapYear(int y) const {
    //TODO
    if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) {
        return true;
    }
    return false;
}

// 返回该月的天数
int Date::daysInMonth(int m, int y) const {
    //TODO
    //错误返回0
    if (m < 0 || m > 12 || y < 1) {
        return 0;
    }
    if (m == 2) {
        return isLeapYear(y) ? 29 : 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11) {
        return 30;
    }
    return 31;
}

// 计算从公元1年1月1日到当前日期的天数
int Date::daysSinceEpoch() const {
    int totalDays = 0;
    // TODO: 计算当前年份中到当前日期的天数
    if (!isValidDate()) {
        return -1;
    }
    for (int i = 1; i < year; i++) {
        totalDays += isLeapYear(i) ? 366 : 365;
    }
    for (int i = 1; i < month; i++) {
        totalDays += daysInMonth(i, year);
    }
    totalDays += day;

    return totalDays;
    //如果非法，返回-1
}

// 计算从某个日期到另一个日期的天数差
int Date::calculateDaysBetween(Date const& other) const {
    //TODO
    //如果任意一个日期非法，返回-1
    if (!isValidDate() || !other.isValidDate()) {
        return -1;
    }
    return abs(daysSinceEpoch() - other.daysSinceEpoch());
}

// 显示日期
void Date::display() const {
    //TODO：按照yyyy-mm-dd格式输出,加一个换行
    //
    //如：cout<<2025-02-17<<endl;
    if (!isValidDate()) {
        cout << -1 << endl;
        return;
    }
    if (year < 10) {
        cout << "000" << year << "-";
    } else if (year < 100) {
        cout << "00" << year << "-";
    } else if (year < 1000) {
        cout << "0" << year << "-";
    } else {
        cout << year << "-";
    }
    if (month < 10) {
        cout << "0" << month << "-";
    } else {
        cout << month << "-";
    }
    if (day < 10) {
        cout << "0" << day << endl;
    } else {
        cout << day << endl;
    }
    //如果非法，输出-1
}

int main() {
    int y1, m1, d1, y2, m2, d2;
    // 输入第一个日期
    cin >> y1 >> m1 >> d1;
    // 输入第二个日期
    cin >> y2 >> m2 >> d2;
    // 创建两个 Date 对象
    Date date1(y1, m1, d1);
    Date date2(y2, m2, d2);
    // 显示两个日期
    date1.display();
    date2.display();
    // 计算并输出日期差
    int diff = date1.calculateDaysBetween(date2);
    cout << diff << endl;
    return 0;
}