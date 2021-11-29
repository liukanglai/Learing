- 基本思路：生成一系列随机数 0-1，没有负坐标，相当于 1/4 个原图，比例没有影响

# rand() with srand()

- rand(): generate random numbers in the range [0, RAND\_MAX]

`https://www.runoob.com/cprogramming/c-function-srand.html#:~:text=srand%20%E5%87%BD%E6%95%B0%E6%98%AF%E9%9A%8F%E6%9C%BA%E6%95%B0,%E4%BD%BF%E7%94%A81%20%E6%9D%A5%E5%88%9D%E5%A7%8B%E5%8C%96%E7%A7%8D%E5%AD%90%E3%80%82`

- 计算机并不能产生真正的随机数，而是已经编写好的一些无规则排列的数字存储在电脑里，把这些数字划分为若干相等的 N 份，并为每份加上一个编号用 srand()函数获取这个编号，然后 rand()就按顺序获取这些数字，当 srand()的参数值固定的时候，rand()获得的数也是固定的，所以一般 srand 的参数用 time(NULL)，因为系统的时间一直在变，所以 rand()获得的数，也就一直在变，相当于是随机数了。只要用户或第三方不设置随机种子，那么在默认情况下随机种子来自系统时钟。如果想在一个程序中生成随机数序列，需要至多在生成随机数之前设置一次随机种子。

#

- 总核数 = 物理 CPU 个数 X 每颗物理 CPU 的核数
- 总逻辑 CPU 数 = 物理 CPU 个数 X 每颗物理 CPU 的核数 X 超线程数
- 查看物理 CPU 个数
  cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l
- 查看每个物理 CPU 中 core 的个数(即核数)
  cat /proc/cpuinfo| grep "cpu cores"| uniq
- 查看逻辑 CPU 的个数
  cat /proc/cpuinfo| grep "processor"| wc -l

- mycomputer:

![liunx](./linux)

# run

- gcc pthread.c -lpthread
- ./a.out

- 本机：

![10000000](./10000000)

![100000000](./100000000)

- 在学校的服务器上（八个 cpu）：

![10000000](./100000001)

![100000000](./1000000001)

# 问题

- 可能我只有一个 cpu，达不到多线程并行的效果，
- 我用 top 看，cpu 也已占到了 700% 左右

- 学校的 cpu(八个核) 可以达到并行的效果，但是 pi 不准确，估计是 rand, srand 时间的问题？

# 影响效率的因素

- 线程间的同步，对全局的访问
- I/O 的操作，总线的占用，计组原理的东西。。。
- cpu 调度问题，优先级，耗时之类的
- ...
