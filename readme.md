# 程序使用说明

## 生成密钥

+ 运行程序，进入欢迎界面，输入想要生成的密钥位数，输入后键入回车结束。
  + 如果输入非数字会进行输入错误提示，提示用户重新输入，此处不再展示。

<img src="https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111161620528.png" alt="image-20221111161620528"  />

+ 生成密钥，显示密钥的生成时间，并展示菜单栏。

  ![image-20221111162157139](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111162157139.png)



## 展示密钥

+ 在菜单栏中键入数字5，键入回车，可展示密钥。

  ![image-20221111162529576](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111162529576.png)



## 加密

+ 在菜单栏中键入数字1，键入回车，开始加密过程。
+ 输入想要加密的明文（**字母、数字或符号**），键入回车后进行加密。

![image-20221111162733507](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111162733507.png)



## 解密

+ 在菜单栏中键入数字2，键入回车，开始解密过程。
+ 输入想要解密的明文（**十六进制数字**），**可输入回车换行，连续键入两次回车后进行解密**。
  + 此处用上一步中生成的密文进行解密展示。

![image-20221111163156481](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111163156481.png)



## 数字签名

+ 在菜单栏中键入数字3，键入回车，开始数字签名过程。

+ 输入想要进行数字签名的明文（**字母、数字或符号**），键入回车后进行数字签名。

  ![image-20221111163415983](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111163415983.png)



## 验证签名

+ 在菜单栏中键入数字4，键入回车，开始验证签名过程。
+ 输入想要验证的签名（**十六进制数字**），**可输入回车换行，连续键入两次回车后进行验证**。
  + 此处用上一步中生成的数字签名进行验证展示。

![image-20221111163638248](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111163638248.png)



## 退出系统

- 在菜单栏中键入数字0，键入回车，退出系统。

  ![image-20221111163826700](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111163826700.png)





# 算法实现中的亮点

## 大数类设计

+ 在大数类BigNumber的设计中，模仿Java中BigInteger类的设计，采用数组对大数进行存储。其中，数组的每个位为一个u_int32_t的无符号32位二进制整数，代替传统的十进制。
+ 在此基础上编写大数的加减乘除取模移位等运算。



## 除法与取模实现

+ 大数的运算与RSA的实现中，除法与取模是最耗时的步骤。在除法与取模运算的实现上，学习了 Donald E. Knuth 著作的 The Art of Computer Programming 中的相关内容，借鉴了相关博文的代码进行了实现了knuth除法。

+ 其算法步骤描述如下（摘自相关博文）。

  > 假设被除数表示为：$u=u_nu_{n-1}...u_1u_0$，除数表示为$v=v_{n-1}...v_1v_0$，$u$和$v$为$2^{32}$进制的整数，$u = q * v + r$，其中$0<=q <2^{32}$，$0<=r <v$。如果不考虑效率的话，枚举也好，二分也好，总能算出q的值，但显然这样算还是太慢了。Knuth在TAOCP中给出一个定理：如果$𝑣_{𝑛−1}≥𝑏𝑎𝑠𝑒/2$，且$𝑞̂ =𝑚𝑖𝑛((𝑢_n∗𝑏𝑎𝑠𝑒+𝑢_{𝑛−1})/𝑣_{𝑛−1},𝑏𝑎𝑠𝑒−1)$，则$𝑞̂ –2≤𝑞≤𝑞̂$ ，其中$𝑞̂$ 是$q$的估计值。这个定理表明，最多只需要计算三次乘法，就能得到商的准确值。
  >
  > 有了计算一位数除法的方法，就可以按照手工除法的方式计算大整数除法，具体计算流程如下：
  > 给定非负整数$𝑢=𝑢_{𝑚+𝑛−1}⋯𝑢_1𝑢_0,𝑣=𝑣_{𝑛−1}⋯𝑣_1𝑣_0,𝑣_{𝑛−1}>0,𝑛>1$。
  >
  > 1. 规格化
  >    定理要求$𝑣_{𝑛−1}≥𝑏𝑎𝑠𝑒/2$，置$𝑑=𝑏𝑎𝑠𝑒/(𝑣_{𝑛−1}+1)$，然后令$u=u*d$，$v=v*d$。若进制$base$为2的幂次，可以直接使用移位操作完成这项工作。
  > 2. 初始化循环变量$j = m$，第一次循环实际上是$𝑢_{𝑗+𝑛}⋯𝑢_{𝑗+1}𝑢_𝑗$除以$v$。
  > 3. 计算$q$的估计值
  >    $𝑞̂ =(𝑢_{𝑗+𝑛}×𝑏𝑎𝑠𝑒+𝑢_{𝑗+𝑛−1})/𝑣_{𝑛−1}$
  > 4. 判断$(𝑢_{𝑗+𝑛}⋯𝑢_𝑗)-𝑞̂ ∗𝑣$是否大于$v$，若大于则$𝑞̂$ 减一。最后令$(𝑢_{𝑗+𝑛}⋯𝑢_𝑗)$为$(𝑢_{𝑗+𝑛}⋯𝑢_𝑗)-𝑞×𝑣$。
  > 5. $j$减一，若$j$不小于0则返回第三步
  > 6. 每一步得到的q组合起来就得到商，而余数通过对$(𝑢_{𝑛−1}⋯𝑢_0)$进行与第一步相反的规格化得到。





# 实现论文的相关信息

+ 实现的论文名称为《ＲＳＡ中大素数生成算法优化及电路实现》，作者为郑朝霞、吴旭峰、季媛媛、刘尹，发表在《华中科技大学学报（自然科学版）》。

+ RSA算法实现中，最耗时的一步为生成指定位数的两个大素数$p$和$q$，目前普遍的做法是随机生成一个指定位数的奇数，在此基础上不断加2，并对其进行素性检测判断其是否为素数，若是则进行输出。在进行素性检测时，原本采用的是朴素米勒拉宾素性检测法。

+ 借鉴了论文中首先使用小素数试除，再进行米勒拉宾检测的方法，算法的流程如下。

  ![image-20221111170237123](https://nodiff-image.oss-cn-beijing.aliyuncs.com/img/image-20221111170237123.png)

> 小素数试除运算即大奇数ｎ除以３，５，７等小素数，看余数是否为零．几乎一半的大奇数都可以通过简单的小素数试除来确定是否为素数。通过小素数试除，可以减少寻找素数５０％的时间 。若试除３余数为零，则不必再进行５和７的试除计算，这样就不必浪费时钟周期。

+ 在实现时，对小于10000的所有素数进行试除。实现算法的部分代码如下。

```c++
vector<long> real_primes = {素数列表}
for (int i = 0; i < real_primes.size(); ++i) {
        if ((n % real_primes[i]) == BigNumber::Zero) {
            return false;
        } else if (n < BigNumber(real_primes[i])) {
            return false;
        } else if (n == BigNumber(real_primes[i])) {
            return true;
        }
    }
```

+ 对算法进行100次RSA-768的对比实验，取平均值。得到实验结果如下。

  |         | 未采用小素数试除 | 采用小素数试除 |
  | :-----: | :--------------: | :------------: |
  | RSA-768 |    1689.03ms     |    374.43ms    |





# 性能评估

+ 分别对RSA-768，RSA-1024，RSA-2048密钥生成进行100次实验，取平均值，得到实验结果如下。

  |          |   耗时    |
  | :------: | :-------: |
  | RSA-768  | 323.81ms  |
  | RSA-1024 | 682.32ms  |
  | RSA-2048 | 5170.28ms |

+ RSA-768 与 RSA-1024的密钥均可在1s内生成，达到了实验的性能要求。





# 实验的感想和收获

+ 通过这次实验，我对于RSA的整体流程有了比较清晰的理解，对于RSA算法的有效性有了较深的体会。
+ 在本次实验中，我学到了一种新的进行大数运算的方法，之前学习的大数运算都是基于十进制字符串的加减乘除。虽然原理相同，但是采用更大的进制算是一种观念上的转变，也确实可以提高运算的效率。
+ 在本次实验中，学习到了效率更高的素数检验算法，之前一直采用的是素数筛法，现在明白了工业界常用的素数检验算法的高效性。
+ 回顾了C++的一些基本语言特性，在实现时碰到了不少因为语言特性产生的错误。



# 对课程的建议

+ 希望对于一些难以理解的算法可以多一些示例。
+ 对于密码体系的有效性可以进行稍微具体一点的说明。


