//
// Created by nodiff on 2022/11/9.
//
#pragma once

#include "BigNumber.h"

class RSA {
private:
    BigNumber o;//欧拉数
    BigNumber p;
    BigNumber q;
public:
    BigNumber e;
    BigNumber n;
    BigNumber d;//私钥

public:
    RSA();

    ~RSA();

public:

    //大数加密
    BigNumber encrypt(const BigNumber &m);

    //大数解密
    BigNumber decrypt(const BigNumber &c);

    //数字签名
    BigNumber signature(const BigNumber &m);

    //数字签名验证
    BigNumber verify(const BigNumber &c);


    //字符串加密
    std::vector<BigNumber> encrypt_by_string(std::string &M, bool state);

    //字符串解密
    std::string decrypt_by_string(std::vector<BigNumber> &C, bool state);


    void print();

    void start(size_t digit);
};

RSA::RSA() {

}

RSA::~RSA() {

}

//初始化，生成p,q,o,n,e,d
void RSA::start(size_t digit) {
    srand(time(NULL));
    Clock::start();
    //生成p,q
    p = BigNumber::make_a_prime(digit / 2, 10);
    q = BigNumber::make_a_prime(digit / 2, 10);
    while (q == p) {
        q = BigNumber::make_a_prime(digit / 2, 10);
    }
    Clock::end();
    //计算欧拉值
    o = (p - 1) * (q - 1);
    //计算公钥n
    n = p * q;
    //生成公钥、私钥指数
    e = 65537;
    d = BigNumber::get_inverse(e, o);


}

BigNumber RSA::encrypt(const BigNumber &m) {
    BigNumber C = m.powerMod(e, n);
    return C;
}

BigNumber RSA::decrypt(const BigNumber &c) {
    BigNumber M = c.powerMod(d, n);
    return M;
}


void RSA::print() {
    std::cout << "======================================================================" << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "q = " << q << std::endl;
    std::cout << "n = " << n << std::endl;
    std::cout << "e = " << e << std::endl;
    std::cout << "d = " << d << std::endl;
    std::cout << "======================================================================" << std::endl;
}

//数字签名
BigNumber RSA::signature(const BigNumber &m) {
    BigNumber sign = m.powerMod(d, n);
    return sign;
}

//数字签名验证
BigNumber RSA::verify(const BigNumber &c) {
    BigNumber veri = c.powerMod(e, n);
    return veri;
}

std::vector<BigNumber> RSA::encrypt_by_string(std::string &M, bool state) {

    vector<BigNumber> string_to_bignumber;
    BigNumber::base_t temp = 0;


    size_t i;
    for (i = 0; i < M.size(); i++) {
        if ((i > 0) && (i % 4 == 0)) {
            string_to_bignumber.push_back(BigNumber(temp));
            temp = 0;
        }
        //逐字符移位
        temp = temp << 7;
        temp = temp + M[i];
    }
    if ((i == 0)||(temp > 0) ) {
        string_to_bignumber.push_back(BigNumber(temp));
    }

    vector<BigNumber> result;
    if (state == true) {
        for (int j = 0; j < string_to_bignumber.size(); ++j) {
            result.push_back(encrypt(string_to_bignumber[j]));
        }
    } else {
        for (int j = 0; j < string_to_bignumber.size(); ++j) {
            result.push_back(signature(string_to_bignumber[j]));
        }
    }

    return result;
}


std::string RSA::decrypt_by_string(std::vector<BigNumber> &C, bool state) {

    string mingwen_string;

    for (int i = 0; i < C.size(); ++i) {
        BigNumber::base_t number;
        if (state == true) {
             number = decrypt(C[i]).get_last_number();

        }else{
             number = verify(C[i]).get_last_number();
        }
        if ((number >> 21) > 0) {
            mingwen_string.push_back(127 & (number >> 21));
        }
        if ((127 & (number >> 14)) > 0) {
            mingwen_string.push_back(127 & (number >> 14));
        }
        if ((127 & (number >> 7)) > 0) {
            mingwen_string.push_back(127 & (number >> 7));
        }
        mingwen_string.push_back(number & 127);

    }
    return mingwen_string;
}










