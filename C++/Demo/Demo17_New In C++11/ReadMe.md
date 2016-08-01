
New In C++11
===========

**C++11类型转换校验表**

```cpp
1.如果有一个操作数类型为long double，则将另一个操作数转换为long double。
2.否则，如果有一个操作数类型为double，则将另一个操作数转换为double。
3.否则，如果有一个操作数类型为float，则将另一个操作数转换为float。
4.否则，说明操作数都为整型，因此执行整型提升。
5.在这种情况下，如果两个操作数都是有符号或无符号的，且其中一个操作数的级别比另一个低，则转换为级别高的类型。
6.如果一个操作数为有符号的，另一个操作数为无符号的，且无符号操作数的级别比有符号操作数高，则将有符号操作数转换为无符号操作数所属的类型。
7.否则，如果有符号类型可表示无符号类型的所有可能取值，则将无符号操作数转换为有符号操作数所属类型。
8.否则，两个操作数都转换为有符号类型的无符号版本。
  
int x = (int)7.124F; //Old C Language.
int y = int(7.124F); //New C++ Language, Use as Function Model.

//强制类型转换运算符
static_cast<typename> (value) //converts value to typeName type.
```

**C++11中的auto声明**

```cpp
auto i = 1; //i is int.
auto d = 1.5; //d is double.
auto y = 1.3E15L //y is long double.
auto z = 0 //OOP, z is int, because 0 is int.
```

**C++11数组初始化列表 - 新增**

```cpp
//允许初始化数组时省略等号=
double earnings[4] {1.2e4, 1.6e4, 1.1e4, 1.7e4};
//初始化大括号内不包含任何数据，则把所有元素初始化为零
int counts[10] = { };
int counts[10] { };
//数组初始化列表禁止隐式转换
int counts[10] {5, 6, 7.1, 2.4} //Error!
```

**C++11原始字符串raw**

```cpp
cout << R"(Jim "King" Tutt uses "\n" instead of endl.)" << '\n';
```

**新增模板类array**

```cpp
#include <array>
array<typeName, n_elem> arr;
//C++11中可用初始化列表为vector和array赋值
array<typeName, n_elem> arr = { elem1, elem2, ..., elemN };
```

**线程存储持续性**

```cpp
C++11新增面向多核处理器的变量存储模式，生命周期与所属线程一样长。
让程序能够将计算放在可并行处理的不同线程中，使用关键字thread_local声明。
```

**新增关键字constexpr**

**说明符和限定符**

```cpp
const
volatile //硬件可修改被修饰的内存单元
mutable //即使结构（或类）变量为const，其修饰的成员变量也可以被修改
```

**C++11特殊成员函数**

```cpp
移动构造函数（Move Constructor）
移动赋值运算符（Move Assignment Operator）
```



