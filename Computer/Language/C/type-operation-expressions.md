# 变量

- 必须先声明后使用
- 声明通常放在函数起始处，在任何可执行语句之前．

1. 全局变量
    - 所有函数体的外部定义的，程序的所有部分都可以使用。一个文件中使用extern关键字来声明另一个文件中存在的全局变量，那么这个文件可以使用这个变量。
2. 局部变量
    - 局限于一个函数的。自动变量，关键字auto局部变量默认为auto，函数的参数都局部变量。在某个函数里，返回数组的地址或者指针之类，必须定义成static,或动态申请内存，这样函数结束返回时才不会释放掉那块内存空间。
    - 寄存器变量也是一种局部变量。register来修饰，通过存放在寄存器中来实现快速访问。

3. 静态变量
    - 生命期和程序生命期是一样的
    - 静态局部变量, 加static后, 静态局部变量属于静态存储方式，静态局部变量只会被初始化一次，下次使用依据上一次保存的值
    - (1)静态局部变量在函数内定义, 生存期为整个源程序，但是其作用域仍与自动变量相同，只能在定义该变量的函数内使用该变量. 作用域不变，生存周期变了
    - (2)允许对构造类静态局部量赋初值 ，例如数组，若未赋以初值，则由系统自动赋以0值。（而非静态局部变量未赋初始值，系统会给个随机值）
    - (3)对基本类型的静态局部变量，系统自动赋予0值。自动变量是随机值。

- 静态局部变量是一种生存期为整个源程序的量。虽然离开定义它的函数后不能使用，但如再次调用定义它的函数时，它又可继续使用， 而且保存了上次被调用后留下的值。 因此，当多次调用一个函数且要求在调用之间保留某些变量的值时，可考虑采用静态局部变量。虽然用全局变量也可以达到上述目的，但全局变量有时会造成意外的副作用，因此仍以采用局部静态变量为宜。

    - 静态全局变量
    - static全局变量只初使化一次, 在存储方式上并无不同。非静态的全局变量在各个源文件中都是有效, 只在定义该变量的源文件内有效. 

4. 常量
    - #define
    - const

# 类型

- int：整数　１６位或３２　
- float：浮点数（可带小数）　３２位，至少６位有效数字
- 

> 并且大多数 C/C++编译器规定,如果原符号位为 1,则右移时右边高位就补充 1,原符号位为 0,则右移时高位就补充 0。
> 实际上,右移 n 位,就相当于左操作数除以 2 ,并且将结果往小里取整。

- 字符串常量/字符数组存放的字符串 : '\0'

- "%f" is the (or at least one) correct format for a double. There is no format for a float, because if you attempt to pass a float to printf, it'll be promoted to double before printf receives it1. "%lf" is also acceptable under the current standard -- the l is specified as having no effect if followed by the f conversion specifier (among others).

- Note that this is one place that printf format strings differ substantially from scanf (and fscanf, etc.) format strings. For output, you're passing a value, which will be promoted from float to double when passed as a variadic parameter. For input you're passing a pointer, which is not promoted, so you have to tell scanf whether you want to read a float or a double, so for scanf, %f means you want to read a float and %lf means you want to read a double (and, for what it's worth, for a long double, you use %Lf for either printf or scanf).


# 运算符

- /：整数除法将执行舍位，无小数


> 位运算符有: &(按位与)、|(按位或)、^(按位异或)、~ (按位取反)

# 表达式
