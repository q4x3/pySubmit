#ifndef bigInteger
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstring>
#include <algorithm>

using namespace std;

class bigInteger
{
private:
    string s;
    int L;
    int sig;//+为1，-为-1
public:
    bigInteger(string ss = string(1,'0'), int LL = 0, int sigg = 0):s(ss), L(LL), sig(sigg) {};
    bigInteger(long long int tmp) {
        s = std::to_string(tmp);
        if (s[0] == '-') s = std::string(s, 1, s.length() - 1);
        reverse(s.begin(),s.end());
        L = s.length();
        if (tmp > 0) sig = 1;
        if (tmp == 0) sig = 0;
        if (tmp < 0) sig = -1;
    }
    bigInteger(int tmp) {
        s = std::to_string(tmp);
        if (s[0] == '-') s = std::string(s, 1, s.length() - 1);
        reverse(s.begin(),s.end());
        L = s.length();
        if (tmp > 0) sig = 1;
        if (tmp == 0) sig = 0;
        if (tmp < 0) sig = -1;
    }
    //赋值运算符重载
    bigInteger &operator=(const bigInteger &right);
    bigInteger &operator=(const long long int &right);
    bigInteger &operator=(const int &right);
    operator double () const;
    operator int () const;
    operator long long () const;
    operator string () const {
        string ss = s;
        reverse(ss.begin(),ss.end());
        return ss;
    }
    operator bool () const {
        if (!sig) return false;
        else return true;
    }
    string add_zeros(int len) const{
        return s + std::string(len, '0');
    }
    bigInteger add_zeros2(int len) const{
        bigInteger tmp;
        tmp.sig = sig;
        tmp.L = len + s.length();
        tmp.s = std::string(len, '0') + s;
        return tmp;
    }
    void delete_zeros() {
        int i;
        for (i = L - 1;i >= 0;-- i)
            if (s[i] != '0') break;
        if (i == -1) {
            s = "0";
            L = 1;
        }
        else {
            s = s.substr(0, i + 1);
            L = i + 1;
        }
    }
    //输入输出重载函数
    friend istream& operator>>(istream &in, bigInteger &obj);
    friend ostream& operator<<(ostream &os,const bigInteger &obj);
    //+-*/运算符重载
    friend bigInteger operator+(const bigInteger &i1, const bigInteger &i2);
    friend bigInteger operator-(const bigInteger &i1, const bigInteger &i2);
    friend bigInteger operator*(const bigInteger &i1, const bigInteger &i2);
    friend bigInteger operator/(const bigInteger &i1, const bigInteger &i2);
    friend bigInteger operator%(const bigInteger &i1, const bigInteger &i2);
    friend bool operator<(const bigInteger &i1, const bigInteger &i2);
    friend bool operator>(const bigInteger &i1, const bigInteger &i2);
    friend bool operator<=(const bigInteger &i1, const bigInteger &i2);
    friend bool operator>=(const bigInteger &i1, const bigInteger &i2);
    friend bool operator==(const bigInteger &i1, const bigInteger &i2);
    friend bool operator!=(const bigInteger &i1, const bigInteger &i2);
};
#endif