#include <iostream>
#include <sstream>
#include "BigNumber.h"
#include "Time.h"
#include <unistd.h>
#include "RSA.h"

using namespace std;

bool AllisNum(string str) {
    for (int i = 0; i < str.size(); i++) {
        int tmp = (int) str[i];
        if (tmp >= 48 && tmp <= 57) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

void show_menu() {
    cout << "======================================================================" << endl;
    cout << "菜单栏:                                                                " << endl;
    cout << "                               1.加密                                  " << endl;
    cout << "                               2.解密                                  " << endl;
    cout << "                               3.数字签名                               " << endl;
    cout << "                               4.验证签名                               " << endl;
    cout << "                               5.输出秘钥                               " << endl;
    cout << "                               6.重置密钥                               " << endl;
    cout << "                               0.退出系统                               " << endl;
    cout << "======================================================================" << endl;
    cout << "请输入您的选择(选项数字):" << endl;
}

int main() {


    cout << "==========================欢迎使用RSA加解密系统==========================" << endl;
    cout << "请输入公私钥位数:" << endl;
    string s;
    cin >> s;
//    int i = 0;
//    RSA rsa;

    while (!AllisNum(s)) {
        cout << "输入有误!请输入纯数字" << endl;
        cin >> s;
    }
    int digit = stoi(s);

//    Clock::start();
//    while (i++ != 100) {
//        rsa.start(digit);
//    }
//    Clock::end();



    RSA rsa;
    cout << "正在生成" << digit << "位公私钥，请稍候......" << endl;
    rsa.start(digit);
    while (true) {
        show_menu();

        char c;
        cin >> c;
        while (!(c >= '0' && c <= '6')) {
            cout << "输入有误!请重新输入!" << endl;
            cin >> c;
        }
        int choice = c - '0';
        if (choice == 1) {
            cout << "======================================================================" << endl;
            cout << "开始加密                                                                " << endl;
            cout << "请输入需要加密的明文(字母、数字或符号):                                       " << endl;
            string M;
            cin.ignore();
            getline(cin, M);
            vector<BigNumber> C = rsa.encrypt_by_string(M, true);
            cout << "加密完成,密文如下:                                                         " << endl;
            for (int i = 0; i < C.size(); ++i) {
                cout << C[i] << endl;
            }
//            cout << "======================================================================" << endl;
        } else if (choice == 2) {
            cout << "======================================================================" << endl;
            cout << "开始解密                                                                " << endl;
            cout << "请输入需要解密的密文(十六进制数字,可换行,连续输入两次回车结束输入):                " << endl;
            vector<BigNumber> C;
            string temp;
            cin.ignore();
            while (getline(cin, temp)) {
                if (temp == "") {
                    break;
                }
                BigNumber b(temp);
                C.push_back(b);
            }
            string M = rsa.decrypt_by_string(C, true);
            cout << "解密完成,明文如下:                                                         " << endl;
            cout << M << endl;


        } else if (choice == 3) {
            cout << "======================================================================" << endl;
            cout << "开始数字签名                                                             " << endl;
            cout << "请输入需要签名的明文(字母、数字或符号):                                       " << endl;
            string M;
            cin.ignore();
            getline(cin, M);
            vector<BigNumber> C = rsa.encrypt_by_string(M, false);
            cout << "签名完成,签名如下:                                                         " << endl;
            for (int i = 0; i < C.size(); ++i) {
                cout << C[i] << endl;
            }
        } else if (choice == 4) {
            cout << "======================================================================" << endl;
            cout << "开始验证签名                                                             " << endl;
            cout << "请输入需要验证的签名(十六进制数字,可换行,连续输入两次回车结束输入):                " << endl;
            vector<BigNumber> C;
            string temp;
            cin.ignore();
            while (getline(cin, temp)) {
                if (temp == "") {
                    break;
                }
                BigNumber b(temp);
                C.push_back(b);
            }
            string M = rsa.decrypt_by_string(C, false);
            cout << "验证完成,签名明文如下:                                                         " << endl;
            cout << M << endl;
        } else if (choice == 5) {
            cout << "======================================================================" << endl;
            cout << "公钥如下:                                                             " << endl;
            cout << "n = " << rsa.n << endl;
            cout << "e = " << rsa.e << endl;
            cout << "私钥如下:                                                             " << endl;
            cout << "d = " << rsa.d << endl;

        } else if (choice == 6) {
            cout << "======================================================================" << endl;
            cout << "开始重置密钥                                                             " << endl;
            cout << "请输入公私钥位数:" << endl;
            size_t digit;
            cin >> digit;
            cout << "正在生成" << digit << "位公私钥，请稍候......" << endl;
            rsa.start(digit);
        } else if (choice == 0) {
            cout << "==========================欢迎您下次使用,再见============================" << endl;
            return 0;
        }


    }

    return 0;
}


