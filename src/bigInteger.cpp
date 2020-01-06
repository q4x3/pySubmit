#include "bigInteger.h"

bool operator==(const bigInteger &i1, const bigInteger &i2)
{
    if (i1.s == i2.s && i1.sig == i2.sig)
        return 1;
    return 0;
}

bool operator!=(const bigInteger &i1, const bigInteger &i2)
{
    if (i1 == i2) return 0;
    return 1;
}

bool operator<(const bigInteger &i1, const bigInteger &i2)
{
    if (i1.sig < i2.sig) return 1;
    if (i1.sig > i2.sig) return 0;
    if (i1.L < i2.L) return 1;
    if (i1.L > i2.L) return 0;
    string s = i1.s, t = i2.s;
    reverse(s.begin(),s.end());
    reverse(t.begin(),t.end());
    return (s < t);
}

bool operator>(const bigInteger &i1, const bigInteger &i2)
{
    if (i1 < i2 || i1 == i2) return 0;
    return 1;
}

bool operator>=(const bigInteger &i1, const bigInteger &i2)
{
    if (i1 == i2 || i1 > i2) return 1;
    return 0;
}

bool operator<=(const bigInteger &i1, const bigInteger &i2)
{
    if (i1 == i2 || i1 < i2) return 1;
    return 0;
}

bigInteger::operator int() const
{
    int tmp = 0, k = 1;
    for (int i = 0;i < L;++ i) {
        tmp += k * (s[i]-'0');
        k *= 10;
    }
    if (sig == -1) tmp = -tmp;
    return tmp;
}

bigInteger::operator long long() const
{
    long long int tmp = 0, k = 1;
    for (int i = 0;i < L;++ i) {
        tmp += k * (s[i]-'0');
        k *= 10;
    }
    if (sig == -1) tmp = -tmp;
    return tmp;
}

bigInteger::operator double() const
{
    double tmp = 0, k = 1;
    for (int i = 0;i < L;++ i) {
        tmp += k * (s[i]-'0');
        k *= 10;
    }
    if (sig == -1) tmp = -tmp;
    return tmp;
}

bigInteger &bigInteger::operator=(const bigInteger &right)
{
    L = right.L;
    s = right.s;
    sig = right.sig;
    return *this;
}

bigInteger &bigInteger::operator=(const long long int &right)
{
    s = std::to_string(right);
    if (s[0] == '-') s = std::string(s, 1, s.length() - 1);
    L = s.length();
    if (right > 0) sig = 1;
    if (right == 0) sig = 0;
    if (right < 0) sig = -1;
    return *this;
}

bigInteger &bigInteger::operator=(const int &right)
{
    s = std::to_string(right);
    if (s[0] == '-') s = std::string(s, 1, s.length() - 1);
    L = s.length();
    if (right > 0) sig = 1;
    if (right == 0) sig = 0;
    if (right < 0) sig = -1;
    return *this;
}

istream& operator>>(istream &in, bigInteger &obj)
{
    in >> obj.s;
    obj.sig = 1;
    if (obj.s[0] == '-') {
        obj.s.erase(0,1);
        obj.sig = -1;
    }
    if (obj.s[0] == '0') obj.sig = 0;
    obj.L = obj.s.length();
    reverse(obj.s.begin(),obj.s.end());
    return in;
}

ostream& operator<<(ostream &os,const bigInteger &obj)
{
    if (obj.sig == -1) os << '-';
    string s;
    s.assign(obj.s.rbegin(), obj.s.rend());
    os << s;
    return os;
}

bigInteger operator+(const bigInteger &i1, const bigInteger &i2)
{
    bigInteger tmp;
    if (!i1.sig) return i2;
    if (!i2.sig) return i1;
    if (i1.sig == 1) {
        if (i2.sig == 1) tmp.sig = 1;
        else {
            bigInteger tmp1 = i2;
            tmp1.sig = 1;
            return i1 - tmp1;
        }
    }
    else {
        if (i2.sig == 1) {
            bigInteger tmp1 = i1;
            tmp1.sig = 1;
            return i2 - tmp1;
        }
        else {
            bigInteger tmp1 = i1;
            bigInteger tmp2 = i2;
            tmp1.sig = 1;
            tmp2.sig = 1;
            bigInteger tmp3 = tmp1 + tmp2;
            tmp3.sig = -1;
            return tmp3;
        }
    }
    string s1, s2;
    if (i1.L <= i2.L) {
        s1 = i1.add_zeros(i2.L - i1.L);
        s2 = i2.s;
        tmp.L = i2.L;
    }
    else {
        s1 = i1.s;
        s2 = i2.add_zeros(i1.L - i2.L);
        tmp.L = i1.L;
    }
    int carry = 0;
    tmp.s.resize(tmp.L);
    for (int i = 0;i < tmp.L;++ i) {
        int sum = s1[i] - '0' + s2[i] - '0' + carry;
        tmp.s[i] = sum%10 + '0';
        carry = sum / 10;
    }
    if (carry) {
        ++ tmp.L;
        tmp.s = tmp.s + string(1,'1');
    }
    return tmp;
}

bigInteger operator-(const bigInteger &i1, const bigInteger &i2)
{
    bigInteger tmp;
    if (!i1.sig) {
        tmp = i2;
        tmp.sig = -i2.sig;
        return tmp;
    }
    if (!i2.sig) return i1;
    if (i1.s == i2.s && i1.sig == i2.sig) {
        tmp.L = 1;
        tmp.sig = 0;
        tmp.s = string(1,'0');
        return tmp;
    }
    if (i1.sig == 1) {
        if (i2.sig == -1) {
            bigInteger tmp1 = i2;
            tmp1.sig = 1;
            return i1 + tmp1;
        }
        else {
            if (i1 < i2) {
                tmp = i2 - i1;
                tmp.sig = -1;
                return tmp;
            }
            else {
                tmp.s = string(i1.L,'0');
                tmp.L = i1.L;
                tmp.sig = 1;
                string s1, s2;
                s1 = i1.s;
                s2 = i2.add_zeros(tmp.L - i2.L);
                for (int i = 0;i < tmp.L;++ i) {
                    if (s1[i] > s2[i]) tmp.s[i] += s1[i] - s2[i];
                    if (s1[i] == s2[i]) {
                        if (tmp.s[i]-'0' == -1) {
                            tmp.s[i] = '9';
                            --tmp.s[i+1];
                        }
                        else tmp.s[i] = '0';
                    }
                    if (s1[i] < s2[i]) {
                        tmp.s[i] += s1[i] - s2[i] + 10;
                        tmp.s[i+1] = -1 + '0';
                    }

                }
                tmp.delete_zeros();
                return tmp;
            }
        }
    }
    else {
        if (i2.sig == 1) {
            bigInteger tmp1 = i1;
            tmp1.sig = 1;
            tmp = tmp1 + i2;
            tmp.sig = -1;
            return tmp;
        }
        else {
            bigInteger tmp1 = i1;
            bigInteger tmp2 = i2;
            tmp1.sig = 1;
            tmp2.sig = 1;
            return tmp2 - tmp1;
        }
    }
}

bigInteger operator*(const bigInteger &i1, const bigInteger &i2)
{
    bigInteger tmp;
    if (!i1.sig || !i2.sig) {
        tmp.L = 1;
        tmp.sig = 0;
        tmp.s = string(1,'0');
        return tmp;
    }
    if (i1.sig * i2.sig > 0) tmp.sig = 1;
    else tmp.sig = -1;
    tmp.L = i1.L + i2.L;
    tmp.s = string(tmp.L,'0');
    for (int i = 0;i < i1.L;++ i)
        for (int j = 0;j < i2.L;++ j) {
            int sum = tmp.s[i+j] - '0' + (i1.s[i]-'0') * (i2.s[j]-'0');
            tmp.s[i+j+1] += sum / 10;
            tmp.s[i+j] = sum % 10 + '0';
        }
    for (int i = 0;i < i1.L + i2.L;++ i){
        tmp.s[i+1] += (tmp.s[i]-'0')/10;
        tmp.s[i] = (tmp.s[i]-'0')%10 + '0';
    }
    tmp.delete_zeros();
    return tmp;
}

bigInteger operator/(const bigInteger &i1, const bigInteger &i2)
{
    bigInteger tmp;tmp.sig = 1;
    tmp.sig = 0;
    if (i1.sig * i2.sig < 0) {
        bigInteger tmp1 = i1, tmp2 = i2, tmp3, tmp4;
        tmp3.L = 1;tmp3.s = string(1,'1');tmp3.sig = 1;
        tmp1.sig = 1;
        tmp2.sig = 1;
        tmp = tmp1 / tmp2;
        tmp4 = tmp1 % tmp2;
        if (tmp4.s == "0") tmp.sig = -1;
        else tmp = tmp + tmp3;
        tmp.sig = -1;
        return tmp;
    }
    if (i1.sig * i2.sig > 0) tmp.sig = 1;
    if (!i1.sig || i1 < i2) {
        tmp.L = 1;
        tmp.s = string(1,'0');
        return tmp;
    }
    if (i1.s == i2.s) {
        tmp.s = string(1,'1');
        tmp.L = 1;
        return tmp;
    }
    bigInteger tmp1 = i1;
    string t = i1.s.substr(i1.L - i2.L);
    reverse(t.begin(),t.end());
    string tt = i2.s;
    reverse(tt.begin(),tt.end());
    if (t < tt) tmp.L = i1.L - i2.L;
    else tmp.L = i1.L - i2.L + 1;
    tmp.s = string(tmp.L, '0');
    int cnt = tmp.L - 1;
    for (int i = cnt;i >= 0;-- i) {
        bigInteger div = i2.add_zeros2(i);
        char m = '0';
        while (1) {
            if (tmp1 < div) break;
            tmp1 = tmp1 - div;
            m = m + 1;
        }
        tmp.s[i] = m;
    }
    return tmp;
}

bigInteger operator%(const bigInteger &i1, const bigInteger &i2)
{
    return i1 - (i1 / i2)* i2;
}