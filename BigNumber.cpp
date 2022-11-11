//
// Created by nodiff on 2022/11/4.
//
#include "BigNumber.h"
#include "iostream"
#include <map>
#include "math.h"

typedef std::size_t size_t;

BigNumber BigNumber::Zero(0);
BigNumber BigNumber::One(1);
BigNumber BigNumber::Two(2);

//重载!=号
bool operator!=(const BigNumber &a, const BigNumber &b) {
    if (a == b) {
        return false;
    } else {
        return true;
    }
}

//重载=号
bool operator==(const BigNumber &a, const BigNumber &b) {
    if (a.data == b.data && a.isNegative == b.isNegative) {
        return true;
    } else {
        return false;
    }
}


static char hash[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

//重载输出流
ostream &operator<<(ostream &outstream, const BigNumber &input) {
    BigNumber::base_t F = 0x0F;
    std::string result;

    //4bit全1，即15的二进制表示
    for (auto it = input.data.begin(); it != input.data.end(); ++it) {
        BigNumber::base_t element = (*it);
        for (int j = 0; j < BASE_CHAR; ++j) {
            //每四位输出一个十六进制位
            char c = hash[element & (F)];
            element = element >> 4;
            result.push_back(c);
        }
    }
    reverse(result.begin(), result.end());

    //去除冗余的0
    size_t index = 0;
    while (index != (result.size() - 1)) {
        if (result[index] == '0') {
            index++;
        } else {
            break;
        }
    }
    result = result.substr(index);
    if (input.isNegative) {
        result = "-" + result;
    }
    outstream << result;

    return outstream;
}

//去除冗余的0
BigNumber &BigNumber::adjust() {
    // 记录数组中为0的元素数量
    int cnt = 0;
    //检查不为0的元素的数量
    for (vector<base_t>::reverse_iterator iter = data.rbegin(); iter != data.rend(); ++iter)
        if ((*iter) == 0) {
            cnt++;
        } else {
            break;
        }
    if (cnt == data.size()) {
        //只有零的情况要保留一个0
        cnt--;
    }
    for (int i = 0; i < cnt; ++i) {
        data.pop_back();
    }
    return *this;
}

//通过long类型初始化
void BigNumber::initByLong(const long input) {
    long tmp = input;

    //判断是否为0
    if (tmp == 0) {
        isNegative = false;
        data.push_back(0);
    }
    //判断是否为负数
    if (tmp < 0) {
        isNegative = true;
        tmp = -tmp;
    }

    while (tmp) {
        base_t element = (tmp & BASE_MAXIMUM);
        data.push_back(element);
        tmp = tmp >> BASE_BIT_NUM;
    }
}

//字符对应十六进制字典
static std::map<char, char> hashHex = {
        {'0', 0x00},
        {'1', 0x01},
        {'2', 0x02},
        {'3', 0x03},
        {'4', 0x04},
        {'5', 0x05},
        {'6', 0x06},
        {'7', 0x07},
        {'8', 0x08},
        {'9', 0x09},
        {'A', 0x0a},
        {'B', 0x0b},
        {'C', 0x0c},
        {'D', 0x0d},
        {'E', 0x0e},
        {'F', 0x0f},
        {'X', 0x00},
};

//将字符转为16进制数
char BigNumber::char_to_hex(char c) {
    if (islower(c)) {
        c = toupper(c);
    }
    return hashHex[c];
}

//从十六进制字符串初始化
void BigNumber::initByHexString(const string &s) {
    string copy(s);
    if (copy.length() && copy[0] == '-') {
        if (copy.length() > 1) {
            isNegative = true;
        }
        copy = copy.substr(1);
    }
    int complete = 8;
    //将字符串长度补齐为8的倍数
    int count = (complete - (copy.length() % complete)) % complete;
    std::string tmp;

    for (int i = 0; i < count; ++i) {
        tmp.push_back(0);
    }

    copy = tmp + copy;

    for (int i = 0; i < copy.length(); i += BASE_CHAR) {
        base_t sum = 0;
        for (int j = 0; j < BASE_CHAR; ++j) {
            char c = copy[i + j];

            c = char_to_hex(c);
            sum = ((sum << 4) | (c));
        }
        data.push_back(sum);
    }
    reverse(data.begin(), data.end());
}

//绝对值比较
bool BigNumber::abs_lt(const BigNumber &b) const {
    if (data.size() < b.data.size()) {
        return true;
    } else if (data.size() > b.data.size()) {
        return false;
    } else {
        for (auto iterB = b.data.rbegin(), iterA = data.rbegin(); iterB != b.data.rend(); ++iterB, ++iterA) {
            if ((*iterB) != (*iterA)) {
                return (*iterA) < (*iterB);
            }
        }
        return false;//相等
    }
}

bool BigNumber::abs_eq(const BigNumber &b) const {
    return b.data == data;
}

bool BigNumber::abs_le(const BigNumber &b) const {
    return this->abs_eq(b) || this->abs_lt(b);
}

//重载<运算符
bool operator<(const BigNumber &a, const BigNumber &b) {
    if (b.isNegative != a.isNegative) {
        if (a.isNegative) {
            return false;
        } else {
            return true;
        }
    }
    if (b.isNegative == a.isNegative) {
        if (a.isNegative) {
            return !a.abs_le(b);
        } else {
            return a.abs_lt(b);
        }
    }
}

//重载<=运算符
bool operator<=(const BigNumber &a, const BigNumber &b) {
    if (b.isNegative != a.isNegative) {
        if (a.isNegative) {
            return false;
        } else {
            return true;
        }
    }
    if (b.isNegative == a.isNegative) {
        if (a.isNegative) {
            return !a.abs_lt(b);
        } else {
            return a.abs_le(b);
        }
    }
}

//重载>运算符
bool operator>(const BigNumber &a, const BigNumber &b) {
    return !(a <= b);
}

//重载+运算符
BigNumber operator+(const BigNumber &a, const BigNumber &b) {
    BigNumber copy(a);
    return copy.add(b);
}

//加法实现
BigNumber &BigNumber::add(const BigNumber &b) {
    //异号的情况
    if (isNegative != b.isNegative) {
        bool isNeg;
        if (abs_lt(b)) {
            //a绝对值小于b
            isNeg = b.isNegative;
        } else if (abs_eq(b)) {
            //a绝对值等于b
            isNeg = false;
        } else {
            //绝对值大于b
            isNeg = isNegative;
        }
        isNegative = b.isNegative;
        *this = *this - b;
        isNegative = isNeg;
    } else if (isNegative == b.isNegative) // 同号的情况
    {
        //引用
        int difflen = b.data.size() - data.size();
        vector<base_t> &result = data;

        while (difflen > 0) {
            //高位补0
            result.push_back(0);
            difflen--;
        }

        base_t carry = 0;//进位
        for (size_t i = 0; i < b.data.size(); i++) {
            uint64_t r = (uint64_t) carry + (uint64_t) result[i] + (uint64_t) b.data[i];
            result[i] = r;
            if (r > BASE_MAXIMUM) {
                carry = 1;
            } else {
                carry = 0;
            }
        }
        for (size_t i = b.data.size(); i < data.size() && carry != 0; ++i) {
            uint64_t r = (uint64_t) carry + (uint64_t) result[i];
            result[i] = r;
            if (r > BASE_MAXIMUM) {
                carry = 1;
            } else {
                carry = 0;
            }
        }

        if (carry != 0) {
            result.push_back(carry);
        }

        adjust();
    }
    return *this;
}

//重载-运算符
BigNumber operator-(const BigNumber &a, const BigNumber &b) {
    BigNumber copy(a);
    return copy.sub(b);
}

//实现减法运算
BigNumber &BigNumber::sub(const BigNumber &b) {
    //异号的情况
    if (isNegative != b.isNegative) {
        bool record = isNegative;
        isNegative = b.isNegative;
        *this = *this + b;
        isNegative = record;
    }
        //同号的情况
    else {
        vector<base_t> result;
        //如果a绝对值大于b
        if (!(abs_lt(b))) {
            int carry = 0;
            for (size_t i = 0; i < b.data.size(); i++) {
                base_t temp = data[i] - b.data[i] - carry;
                //判断是否进位
                if (data[i] < b.data[i]) {
                    carry = 1;
                } else {
                    carry = 0;
                }
                result.push_back(temp);
            }
            for (size_t i = b.data.size(); i < data.size(); i++) {
                base_t temp = data[i] - carry;
                if (data[i] < carry) {
                    carry = 1;
                } else {
                    carry = 0;
                }
                result.push_back(temp);
            }
            this->data = result;
            adjust();
        } else {
            data = (b - (*this)).data;
            isNegative = !isNegative;
        }
    }

    return *this;
}


//重载左移<<运算符
BigNumber operator<<(const BigNumber &a, unsigned int n) {
    BigNumber copy(a);
    return copy.l_move(n);
}

//左移操作
BigNumber &BigNumber::l_move(BigNumber::base_t n) {

    base_t partMove = n % BASE_BIT_NUM;
    base_t wholeMove = n / BASE_BIT_NUM;
    base_t bits = BASE_BIT_NUM * (data.size() - 1);//计算一共有多少二进制位
    base_t largestNum = data[data.size() - 1];
    while (largestNum > 0) {
        largestNum = largestNum >> 1;
        bits++;
    }

    //返回大于等于
    double temp = (double) (bits + n) / BASE_BIT_NUM;
    base_t newSize = ceil(temp);
    //数组扩容，扩出来的部分补0
    data.resize(newSize, 0);


    for (long long i = newSize - 1; i >= 0; i--) {
        if (i < wholeMove) {
            data[i] = 0;
        } else {
            if (i == wholeMove) {
                data[i] = data[i - wholeMove] << partMove;
            } else {
                base_t right_result;
                if (partMove == 0) {
                    right_result = 0;
                } else {
                    right_result = data[i - wholeMove - 1] >> (BASE_BIT_NUM - partMove);
                }
                data[i] = (data[i - wholeMove] << partMove) + right_result;
            }
        }
    }
    adjust();
    return *this;
}

//重载右移运算符
BigNumber operator>>(const BigNumber &a, unsigned int n) {
    BigNumber copy(a);
    return copy.r_move(n);
}

//右移操作实现
BigNumber &BigNumber::r_move(BigNumber::base_t n) {
    base_t bits = BASE_BIT_NUM * (data.size() - 1);//计算一共有多少二进制位
    base_t largestNum = data[data.size() - 1];
    while (largestNum > 0) {
        largestNum = largestNum >> 1;
        bits++;
    }

    if (bits <= n) {
        data.clear();
        data.push_back(0);
        return *this;
    }

    base_t partMove = n % BASE_BIT_NUM;
    base_t wholeMove = n / BASE_BIT_NUM;

    for (base_t i = 0; wholeMove < data.size(); i++, wholeMove++) {
        if ((wholeMove + 1 < data.size()) && (partMove > 0)) {
            base_t right_result;
            right_result = data[wholeMove + 1] << (BASE_BIT_NUM - partMove);
            data[i] = (data[wholeMove] >> partMove) + right_result;
        } else {
            data[i] = data[wholeMove] >> partMove;
        }
    }
    double temp = (double) (bits - n) / BASE_BIT_NUM;
    base_t newSize = ceil(temp);
    data.resize(newSize, 0);
    adjust();
    return *this;
}


BigNumber operator*(const BigNumber &a, const BigNumber &b) {
    BigNumber copy1(a);
    BigNumber copy2(b);

    return copy1.mul(copy2);

}

BigNumber &BigNumber::mul(const BigNumber &b) {
    //如果有一方为0
    if (data == Zero || b == Zero) {
        *this = Zero;
        return *this;
    }
    //判断符号
    bool isNeg;
    if (isNegative == b.isNegative) {
        isNeg = false;
    } else {
        isNeg = true;
    }
    isNegative = isNeg;

    vector<base_t> result(data.size() + b.data.size(), 0);
    base_t carry;
    uint64_t temp;
    base_t left, right;
    for (size_t i = 0; i < data.size(); i++) {
        carry = 0;

        left = data[i];
        for (size_t j = 0; j < b.data.size(); j++) {

//            temp[i + j] = temp[i + j] +  ((u_int64_t) data[i] * (u_int64_t) b.data[j]);
            right = b.data[j];
            temp = (u_int64_t) result[i + j] + (u_int64_t) left * (u_int64_t) right +
                   (u_int64_t) carry;
            carry = (base_t) (temp >> 32);
            result[i + j] = (base_t) temp;
        }
        if (carry) {
            result[i + b.data.size()] = carry;
        }

    }
    this->data = result;
//    for (size_t i = 0; i < temp.size() - 1; i++) {
//        temp[i + 1] += (temp[i] >> BASE_BIT_NUM);
//        temp[i] = (temp[i] & BASE_MAXIMUM);
//
//    }
//
//    while ((temp[temp.size() - 1] >> BASE_BIT_NUM)) {
//        u_int64_t top = temp[temp.size() - 1] >> BASE_BIT_NUM;
//        temp[temp.size() - 1] = (temp[temp.size() - 1] & BASE_MAXIMUM);
//
//        temp.push_back(top);
//    }
//    vector<base_t> d(temp.begin(), temp.end());
//    data = d;
    adjust();
    return *this;

}


std::pair<BigNumber, BigNumber>
BigNumber::div_and_remainder(const BigNumber &b) {

    //判断符号
    bool isNeg;
    if (isNegative == b.isNegative) {
        isNeg = false;
    } else {
        isNeg = true;
    }

    if (b.data.size() == 1) {
        std::pair<BigNumber, BigNumber> pair = this->div_and_remainder_baset(b.data[0]);
        BigNumber &p = pair.first;
        BigNumber &r = pair.second;
        p.isNegative = isNeg;
        r.isNegative = isNeg;
        return std::make_pair(std::move(p), BigNumber(r));
    }
    // 被除数绝对值小于除数
    if (this->abs_lt(b)) {
        BigNumber p = BigNumber();
        BigNumber r = *this;
        return std::make_pair(p, r);
    }

    // 实现knuth算法
    BigNumber remainder;
    BigNumber dvsr = b;
    BigNumber dvid = *this;

    //保存quot
    vector<base_t> quot;

    //正规化
    size_t l_move_steps = 0;
    base_t backmove = dvsr.data.back();
    uint64_t base = uint64_t(1) << base_t_len;
    while (backmove < base / 2) {
        ++l_move_steps;
        backmove = (backmove << 1);
    }
    dvsr = (dvsr << l_move_steps);
    dvid = (dvid << l_move_steps);
    vector<base_t> &dividend_data = dvid.data;
    const vector<base_t> &divisor_data = dvsr.data;
    dividend_data.push_back(0);
    //除数的size
    size_t dvsr_len = divisor_data.size();

    //循环
    quot.resize(dividend_data.size() - divisor_data.size());
    const base_t &m = divisor_data[dvsr_len - 1], &n = divisor_data[dvsr_len - 2];
    size_t quot_size = quot.size() - 1;
    for (size_t i = quot_size; i != (size_t) -1; --i) {
        // 得到quot的估计值
        base_t &u_2 = dividend_data[i + dvsr_len - 2];
        base_t &u_0 = dividend_data[i + dvsr_len];
        base_t &u_1 = dividend_data[i + dvsr_len - 1];
        double_t quot_h;
        quot_h = (u_0 * base + u_1) / m;
        if (base * (u_0 * base + u_1 - quot_h * m) + u_2 < quot_h * n) {
            --quot_h;
        }

        // 乘法与减法操作
        double_t carry = 0;
        int64_t borow = 0;
        for (size_t j = 0; j < dvsr_len; ++j) {
            borow = borow + dividend_data[i + j];
            carry = carry + quot_h * divisor_data[j];
            borow -= base_t(carry);
            dividend_data[i + j] = base_t(borow);
            borow = (borow >> base_t_len);
            carry = (carry >> base_t_len);
        }
        borow = borow + dividend_data[i + dvsr_len];
        borow -= carry;
        dividend_data[i + dvsr_len] = base_t(borow);
        borow = (borow >> base_t_len);
        while (borow < 0) {
            assert(borow == -1);
            carry = 0;
            for (size_t j = 0; j < dvsr_len; ++j) {
                carry = (carry + dividend_data[i + j] + divisor_data[j]);
                dividend_data[i + j] = base_t(carry);
                carry = (carry >> base_t_len);
            }
            carry = (carry + dividend_data[i + dvsr_len]);
            dividend_data[i + dvsr_len] = base_t(carry);
            carry = (carry >> base_t_len);
            borow = borow + carry;
            --quot_h; // don't forget
        }
        assert(borow == 0);
        quot[i] = base_t(quot_h);

    }
    // 3. get quotient and real remainder
    BigNumber quotient(std::move(quot));
    quotient.isNegative = isNeg;
    dvid.isNegative = isNeg;
    dvid.adjust();
    dvid = dvid.r_move(l_move_steps);
    return std::make_pair(std::move(quotient), std::move(dvid));
}

std::pair<BigNumber, BigNumber>
BigNumber::div_and_remainder_baset(const base_t b) {
    if (b == 1) {
        BigNumber r = BigNumber(base_t(0));
        return std::make_pair(BigNumber(*this), r);
    } else if (b == 0) {
        throw std::runtime_error("被除数不能为0!");
    } else {
        double_t n = 0;
        double_t m = double_t(b);
        BigNumber temp = *this;
        vector<base_t> &data_reference = temp.data;
        for (size_t i = data_reference.size() - 1; i != (size_t) -1; --i) {
            n = n << base_t_len;
            n = n + data_reference[i];
            base_t tmp = base_t(n / m);
            data_reference[i] = tmp;
            n = n % m;
        }
        temp.adjust();
        return std::make_pair(std::move(temp), BigNumber(base_t(n)));
    }
}


//重载/运算符
BigNumber operator/(const BigNumber &a, const BigNumber &b) {
    BigNumber copy1(a);
    BigNumber copy2(b);

    const std::pair<BigNumber, BigNumber> &pair = copy1.div_and_remainder(copy2);
    BigNumber result = pair.first;
    return result;
}

//重载%运算符
BigNumber operator%(const BigNumber &a, const BigNumber &b) {
    BigNumber copy1(a);
    BigNumber copy2(b);

    const std::pair<BigNumber, BigNumber> &pair = copy1.div_and_remainder(copy2);
    BigNumber result = pair.second;
    return result;
}

//快速幂模运算
BigNumber BigNumber::powerMod(const BigNumber &input, const BigNumber &c) const {
    BigNumber ans(1);
    BigNumber a(*this);
    BigNumber b(input);
    a = a % c;
    while (b > BigNumber::Zero) {
        if (b % BigNumber::Two == BigNumber::One) {
//            ans = (ans * a) % c;
//            BigNumber temp1 = ans * a;
//            BigNumber temp2 = BigNumber(ans).mul2(a);
//            if (temp1 != temp2) {
//                std::cout << "ans:" << ans << std::endl;
//                std::cout << "a:" << a << std::endl;
//                std::cout <<"temp1:"<< temp1 << std::endl;
//                std::cout <<"temp2:"<< temp2 << std::endl;;
//                assert(temp1 == temp2);
//            }
            ans = (ans * a) % c;

        }
        b = b / BigNumber::Two;
//        BigNumber temp1 = ans * a;
//        BigNumber temp2 = BigNumber(ans).mul2(a);
//        if (temp1 != temp2) {
//            std::cout << "a:" << a << std::endl;
//            std::cout <<"temp1:"<< temp1 << std::endl;
//            std::cout <<"temp2:"<< temp2 << std::endl;;
//            assert(temp1 == temp2);
//        }
        a = (a * a) % c;
    }
    return ans;
}

BigNumber BigNumber::make_random_smaller_number(const BigNumber &a) {
    unsigned long temp = 0;
    while (temp == 0) {
        temp = rand();
    }

    BigNumber copy(temp);
    BigNumber result = copy % a;
    if (result == BigNumber::Zero) {
        result = a - BigNumber::One;
    }
    return result;
}

//米勒拉宾素数判定法
bool BigNumber::isPrime_by_miller_rabin(const BigNumber &n, const unsigned int k) {
    if (n == BigNumber::Two) {
        return true;
    }
    if (n == BigNumber::One) {
        return false;
    }
    assert(n != BigNumber::Zero);

    vector<long> real_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
                        101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193,
                        197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307,
                        311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                        431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547,
                        557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
                        661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
                        809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929,
                        937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 1039,
                        1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151, 1153,
                        1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279,
                        1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409,
                        1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499,
                        1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613,
                        1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741,
                        1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873,
                        1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999,
                        2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113,
                        2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251,
                        2267, 2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371,
                        2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473, 2477,
                        2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617, 2621, 2633, 2647,
                        2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731,
                        2741, 2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857,
                        2861, 2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001,
                        3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089, 3109, 3119, 3121, 3137, 3163,
                        3167, 3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257, 3259, 3271, 3299,
                        3301, 3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407,
                        3413, 3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539,
                        3541, 3547, 3557, 3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643, 3659,
                        3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733, 3739, 3761, 3767, 3769, 3779, 3793,
                        3797, 3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919,
                        3923, 3929, 3931, 3943, 3947, 3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051,
                        4057, 4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177, 4201,
                        4211, 4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297, 4327,
                        4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409, 4421, 4423, 4441, 4447, 4451, 4457, 4463,
                        4481, 4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591, 4597, 4603,
                        4621, 4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733,
                        4751, 4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871, 4877, 4889, 4903,
                        4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003, 5009,
                        5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153,
                        5167, 5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281, 5297, 5303,
                        5309, 5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441,
                        5443, 5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521, 5527, 5531, 5557, 5563, 5569,
                        5573, 5581, 5591, 5623, 5639, 5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701,
                        5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791, 5801, 5807, 5813, 5821, 5827, 5839, 5843,
                        5849, 5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981, 5987,
                        6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131,
                        6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221, 6229, 6247, 6257, 6263, 6269,
                        6271, 6277, 6287, 6299, 6301, 6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373,
                        6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553,
                        6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673, 6679, 6689, 6691,
                        6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829,
                        6833, 6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967,
                        6971, 6977, 6983, 6991, 6997, 7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109,
                        7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237, 7243, 7247,
                        7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411, 7417, 7433, 7451,
                        7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559,
                        7561, 7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687,
                        7691, 7699, 7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829, 7841,
                        7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951, 7963, 7993,
                        8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147,
                        8161, 8167, 8171, 8179, 8191, 8209, 8219, 8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287,
                        8291, 8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387, 8389, 8419, 8423, 8429, 8431,
                        8443, 8447, 8461, 8467, 8501, 8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597, 8599,
                        8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677, 8681, 8689, 8693, 8699, 8707, 8713, 8719,
                        8731, 8737, 8741, 8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831, 8837, 8839, 8849,
                        8861, 8863, 8867, 8887, 8893, 8923, 8929, 8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007,
                        9011, 9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109, 9127, 9133, 9137, 9151, 9157,
                        9161, 9173, 9181, 9187, 9199, 9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283, 9293,
                        9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377, 9391, 9397, 9403, 9413, 9419, 9421, 9431,
                        9433, 9437, 9439, 9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533, 9539, 9547, 9551,
                        9587, 9601, 9613, 9619, 9623, 9629, 9631, 9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721,
                        9733, 9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811, 9817, 9829, 9833, 9839, 9851,
                        9857, 9859, 9871, 9883, 9887, 9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973};
    for (int i = 0; i < real_primes.size(); ++i) {
        if ((n % real_primes[i]) == BigNumber::Zero) {
            return false;
        } else if (n < BigNumber(real_primes[i])) {
            return false;
        } else if (n == BigNumber(real_primes[i])) {
            return true;
        }
    }

    BigNumber s = n - BigNumber::One;
    BigNumber origin_s(s);
    size_t r = 0;
    while ((s.data[0] & 1) == 0) {
        s = s >> 1;
        r++;
    }

    for (size_t i = 0; i < k; ++i) {
        //生成随机数
        BigNumber a = make_random_smaller_number(origin_s);//随机数
        if (n == a) {
            return true;
        }
        bool miller_rabin = false;
        BigNumber k = a.powerMod(s, n);
        if (k == BigNumber::One) {
            miller_rabin = true;
        }
        for (size_t j = 0; j < r; ++j, k = ((k * k) % n)) {
            if (k == origin_s) {
                miller_rabin = true;
            }
        }
        if (miller_rabin == false) {
            return false;
        }

    }
    return true;


}

//扩展的欧几里得定理求乘法逆元
BigNumber BigNumber::ext_gcd(const BigNumber a, const BigNumber b, BigNumber &x, BigNumber &y) {
    if (b == BigNumber::Zero) {
        x = BigNumber::One, y = BigNumber::Zero;
        return a;
    }
    BigNumber d = ext_gcd(b, a % b, y, x);
    y = y - ((a / b) * x);
    return d;
}

//求逆
BigNumber BigNumber::get_inverse(const BigNumber a, const BigNumber b) {
    BigNumber x(BigNumber::Zero);
    BigNumber y(BigNumber::Zero);
    BigNumber d = ext_gcd(a, b, x, y);
    if (d == BigNumber::One) {
        //处理负数
        return ((x % b) + b) % b;
    }
    return BigNumber(-1);
}

//生成素数
BigNumber BigNumber::make_a_prime(size_t n, int k) {

    static std::map<int, char> hash_table = {
            {0,  '0'},
            {1,  '1'},
            {2,  '2'},
            {3,  '3'},
            {4,  '4'},
            {5,  '5'},
            {6,  '6'},
            {7,  '7'},
            {8,  '8'},
            {9,  '9'},
            {10, 'A'},
            {11, 'B'},
            {12, 'C'},
            {13, 'D'},
            {14, 'E'},
            {15, 'F'},
    };

    n = n / 4;
    BigNumber prime;
    if (n) {
        string result = "";
        for (size_t i = 0; i < n - 1; ++i) {
            result += hash_table[rand() % 16];
        }
        result += hash_table[1];
        prime = BigNumber(result);
    } else {
        prime = BigNumber(BigNumber::Zero);
    }

    while (!isPrime_by_miller_rabin(prime, k)) {
        prime = prime + BigNumber::Two;
    }
    return prime;
}


//得到最大的32位数字
BigNumber::base_t BigNumber::get_last_number() {
    return data[data.size() - 1];
}








//std::pair<BigNumber, BigNumber>
//BigNumber::div_and_remainder(const BigNumber &ubigint)
//{
//    if(ubigint.data.size() == 1)
//        return this->div_and_remainder_baset(ubigint.data[0]);
//    // dividend less than divisor
//    if(*this < ubigint)
//        return std::make_pair(BigNumber(base_t(0)), BigNumber(*this));
//
//    // implement the algorithm in Knuth[The Art of Computer Programming]
//    BigNumber dividend = *this, divisor = ubigint, rem;
//    vector<base_t> q; // save the quot
//    // 1. normalize
//    double_t base = double_t(1) << base_t_len;
//    size_t lshift = 0;
//    base_t back = divisor.data.back();
//    while(back < base/2) back<<=1, ++lshift;
//    dividend = dividend.l_move(lshift) ;
//    divisor  = divisor.l_move(lshift);
//    vector<base_t> &a = dividend.data;
//    const vector<base_t> &b = divisor.data;
//    size_t sz_b = b.size(); // size of divisor
//    a.push_back(base_t(0)); // for u0
//
//    // 2. loop
//    const base_t &v1 = b[sz_b-1], &v2 = b[sz_b-2];
//    q.resize(a.size() - b.size());
//    for(size_t i = q.size() - 1; i != (size_t)-1; --i)
//    {
//        // get the estimated value of q
//        double_t qh;
//        base_t &u0 = a[i+sz_b], &u1 = a[i+sz_b-1], &u2 = a[i+sz_b-2];
//        qh = (u0 * base + u1) / v1;
//        if(qh * v2 > base * (u0 * base + u1 - qh * v1) + u2) --qh;
//
//        // multi and subtract
//        int64_t borrow = 0;
//        double_t carry = 0;
//        for(size_t j = 0; j < sz_b; ++j)
//        {
//            carry += qh * b[j];
//            borrow += a[i+j];
//            borrow -= base_t (carry);
//            a[i+j] = base_t (borrow);
//            borrow >>= base_t_len;
//            carry >>= base_t_len;
//        }
//        borrow += a[i+sz_b];
//        borrow -= carry;
//        a[i+sz_b] = base_t (borrow);
//        borrow >>= base_t_len;
//        while(borrow < 0)
//        {
//            // add divisor back to dividend
//            assert(borrow==-1);
//            carry = 0;
//            for(size_t j = 0; j < sz_b; ++j)
//            {
//                carry += a[i+j];
//                carry += b[j];
//                a[i+j] = base_t (carry);
//                carry >>= base_t_len;
//            }
//            carry += a[i+sz_b];
//            a[i+sz_b] = base_t (carry);
//            carry >>= base_t_len;
//            borrow += carry;
//            --qh; // don't forget
//        }
//        assert(borrow == 0);
//        q[i] = base_t (qh);
//
//    }
//    // 3. get quotient and real remainder
//    BigNumber quotient(std::move(q));
//    dividend.adjust();
//    dividend.r_move(lshift);
//    return std::make_pair(std::move(quotient), std::move(dividend));
//}
//std::pair<BigNumber, BigNumber>
//BigNumber::div_and_remainder_baset(const base_t number)
//{
//    if(number == 0)
//        throw std::runtime_error("unsigned_bigint: divide by zero.");
//    if(number == 1)
//        return std::make_pair(BigNumber(*this), BigNumber(base_t (0)));
//    BigNumber tmp = *this;
//    vector<base_t> &a = tmp.data;
//    double_t x = uint64_t (number), y = 0;
//    size_t i = a.size() - 1;
//    for(; i != (size_t)-1; --i)
//    {
//        y <<= base_t_len;
//        y += a[i];
//        a[i] = base_t(y/x);
//        y %= x;
//    }
//    tmp.adjust();
//    return std::make_pair(std::move(tmp), BigNumber(base_t (y)));
//}






