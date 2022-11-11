//
// Created by nodiff on 2022/11/4.
//
#pragma once

#include<string>
#include<cassert>
#include<iostream>
#include<vector>
#include <algorithm>
#include <cstdint>

#define BASE_MAXIMUM 0xFFFFFFFF
#define BASE_CHAR 8
#define BASE_BIT_NUM 32 //每32bit一位
#define MOVE_BIT_CHAR 0x1F
#define MOVE_BIT 5
using std::ostream;
using std::vector;
using std::string;

class BigNumber {

public:
    typedef uint32_t base_t;
    typedef uint64_t double_t;
    static const size_t base_t_len = sizeof(base_t) * 8;
    static BigNumber Zero;
    static BigNumber One;
    static BigNumber Two;
private:
    //存储数据
    vector<base_t> data;
    //是否为负数
    bool isNegative;


public:

    //无参构造函数
    BigNumber() : isNegative(false) {
        data.clear();
        data.push_back(0);
    }

    //从BigInt实例初始化
    BigNumber(const vector<base_t> &data) : data(data), isNegative(false) {
        adjust();
    }

    BigNumber(const BigNumber &b, bool isNegative) : data(b.data), isNegative(isNegative) {

    }

    //从Long类型初始化
    BigNumber(const long number) : isNegative(false) {
        initByLong(number);
    }

    //从十六进制字符串初始化
    BigNumber(const string &numberHex) : data(), isNegative(false) {
        initByHexString(numberHex);
        adjust();
    }

    //重载=运算符
    BigNumber &operator=(const string &str) {
        isNegative = false;
        data.clear();
        initByHexString(str);
        adjust();
        return *this;
    }

    //重载=运算符
    BigNumber &operator=(const long number) {
        data.clear();
        initByLong(number);
        return *this;
    }



public:
    bool abs_lt(const BigNumber &b) const;//判断绝对值是否小于
    bool abs_le(const BigNumber &b) const;//判断绝对值是否小于相等
    bool abs_eq(const BigNumber &b) const;//判断绝对值是否相等

    BigNumber &add(const BigNumber &b);

    BigNumber &sub(const BigNumber &b);

    BigNumber &l_move(base_t n);

    BigNumber &r_move(base_t n);

    BigNumber &mul(const BigNumber &b);

    std::pair<BigNumber, BigNumber> div_and_remainder(const BigNumber &b);

    std::pair<BigNumber, BigNumber> div_and_remainder_baset(const base_t b);

    //大数幂模运算
    BigNumber powerMod(const BigNumber &input, const BigNumber &c) const;

    //扩展的欧几里得定理
    static BigNumber ext_gcd(const BigNumber a, const BigNumber b, BigNumber &x, BigNumber &y);

    //求逆
    static BigNumber get_inverse(const BigNumber a, const BigNumber b);

    //米勒拉宾素数判定法
    static bool isPrime_by_miller_rabin(const BigNumber &n, const unsigned int k);

    //创建小于a的随机数
    static BigNumber make_random_smaller_number(const BigNumber &a);

    //创建n位质数
    static BigNumber make_a_prime(size_t n, int k);

    //得到最大的32位数字
    base_t get_last_number();


    //从十六进制字符串初始化
    void initByHexString(const string &s);

    //将字符转为16进制数
    static char char_to_hex(char c);

    //通过Long类型初始化
    void initByLong(const long input);

public:

    friend BigNumber operator-(const BigNumber &a, const BigNumber &b);

    friend BigNumber operator+(const BigNumber &a, const BigNumber &b);

    friend BigNumber operator/(const BigNumber &a, const BigNumber &b);

    friend BigNumber operator*(const BigNumber &a, const BigNumber &b);

    friend BigNumber operator%(const BigNumber &a, const BigNumber &b);

    friend bool operator==(const BigNumber &a, const BigNumber &b);

    friend bool operator!=(const BigNumber &a, const BigNumber &b);

    friend bool operator<(const BigNumber &a, const BigNumber &b);

    friend bool operator>(const BigNumber &a, const BigNumber &b);


    friend bool operator<=(const BigNumber &a, const BigNumber &b);

    friend ostream &operator<<(ostream &out, const BigNumber &a);

    friend BigNumber operator<<(const BigNumber &a, unsigned int n);

    friend BigNumber operator>>(const BigNumber &a, unsigned int n);


    //去除冗余的0
    BigNumber &adjust();


};
