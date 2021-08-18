# analysis

## T(n)

- worst time
- average time: weighted average
- best time(bogus): cheat

## time

1. computer
- relative speed(same computer)
- absolute speed(different computer)


# asymptotic analysis

- (渐进分析)，忽略依赖机器的常量，不检查实际运行时间，关注运行时间的增长

- &theta; --去低价，常量
- O
- &omega;


- n-> , 

# sorting

1. insertion sort algorithm

        for j <- [1-n];
            key = a[j];
            i = j - 1;
            while(i >= 0)
                a[i+1] = a[i];
                if(a[i--] < key)
                    break;
            a[i+1] = key;

            while(i >= 0 && a[i] > key)
                a[i+1] = a[i];
                i--;
            a[i+1] = key;

- &theta;(n^2)

2. merge sort
    - n = 1, break;  &theta;(1) just a constant
    - recursively sort a[0, n/2(向上取整)] and a[n/2 + 1(向下取整), n];  2T(n/2)
    - merge subroutine. &theta;(n)

        j


- T(n) = 2T(n/2) + cn = 2(2T(n/4) +cn/2) + cn = ...
- recursive tree, height is lgn, leaves are n
- T(n) = (cn)lgn + &theta;(n) = &theta;(nlgn)
