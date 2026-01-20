##### 传值返回会产生临时对象

###### 调用两次拷贝构造

>   CMake 禁用编译器优化选项
>
>   ```
>   cmake_minimum_required(VERSION 3.27)
>   project(cpp_learning)
>   
>   set(CMAKE_CXX_STANDARD 11)
>   add_compile_options(-fno-elide-constructors)
>   
>   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
>   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
>   
>   add_executable(cpp_learning main.cpp
>           func.c)
>   
>   ```

```c++
#include <iostream>
class A {
public:
    A(int a, int b) : _a(a), _b(b) {
        std::cout << "构造..." << std::endl;
    }

    ~A() {}

    A(const A &a)
            : _a(a._a), _b(a._b) {
        std::cout << "拷贝构造..." << std::endl;
    }

    int _a;
    int _b;
};

A test() {
    A a(1, 1);
    return a;
}

int main() {
//    const A &aa = test();
    A aa = test();
    std::cout << aa._a << " " << aa._b << std::endl;
    return 0;
}

```

```
构造...
拷贝构造...
拷贝构造...
1 1
```

传值返回会产生临时对象, 并且需要调用两次拷贝构造

<img src="/Users/zhuan/C++_Linux_learning/cpp_learning/notes/img_C++/Snipaste_2026-01-16_14-31-16.png" alt="Snipaste_2026-01-16_14-31-16" style="zoom:80%;" />

###### 临时对象具有常性

如果使用引用接收, 那么必须是`const`引用才行

```c++
A test() {
    A a(1, 1);
    return a;
}

int main() {
    A &aa = test();
//    A aa = test();
//    std::cout << aa._a << " " << aa._b << std::endl;
    return 0;
}
```

```
/Users/zhuan/CLionProjects/cpp_learning/main.cpp:25:8: error: non-const lvalue reference to type 'A' cannot bind to a temporary of type 'A'
    A &aa = test();
       ^    ~~~~~~
1 error generated.
```

```c++
const A &aa = test();	// 这样才对
```













## C++ 11

### 包装器

#### function

```cc
template <class T>
class function; // undefined

template <class Ret, class... Args>
class function<Ret(Args...)>;
```

- std::function 是一个类模板，也是一个包装器。 std::function 的实例对象可以包装存储其他的可以调用对象，包括函数指针、仿函数、 lambda 、 bind 表达式等，存储的可调用对象被称为 std::function 的目标。若 std::function 不含目标，则称它为空。调用空 std::function 的目标导致抛出 std::bad_function_call 异常。
- 以上是 function 的原型，他被定义`<functional>`头文件中。std::function - cppreference.com 是function的官方文件链接。
- 函数指针、仿函数、 lambda 等可调用对象的类型各不相同， std::function 的优势就是统一类型，对他们都可以进行包装，这样在很多地方就方便声明可调用对象的类型，下面的第二个代码样例展示了 std::function 作为map的参数，实现字符串和可调用对象的映射表功能。

```cc
#include<iostream>
#include <functional>
using namespace std;
int f(int a, int b) { return a + b; }
struct Functor {
  public:
    int operator()(int a, int b) { return a + b; }
};
class Plus {
  public:
    Plus(int n = 10) : _n(n) {}
    static int plusi(int a, int b) { return a + b; }
    double plusd(double a, double b) { return (a + b) * _n; }

  private:
    int _n;
};
int main() {
    // 包装各种可调用对象
    function<int(int, int)> f1 = f;
    function<int(int, int)> f2 = Functor();
    function<int(int, int)> f3 = [](int a, int b) { return a + b; };
  
    cout << f1(1, 1) << endl;
    cout << f2(1, 1) << endl;
    cout << f3(1, 1) << endl;
  
  
    // 包装静态成员函数
    // 成员函数要指定类域并且前面加&才能获取地址
    function<int(int, int)> f4 = &Plus::plusi;
    cout << f4(1, 1) << endl;
  
  
    // 包装普通成员函数
    // 普通成员函数还有一个隐含的this指针参数，所以绑定时传对象或者对象的指针过去都可以
    function<double(Plus *, double, double)> f5 = &Plus::plusd;
    Plus pd;
    cout << f5(&pd, 1.1, 1.1) << endl;
  
    function<double(Plus, double, double)> f6 = &Plus::plusd;
    cout << f6(pd, 1.1, 1.1) << endl;
    cout << f6(pd, 1.1, 1.1) << endl;
  
    function<double(Plus &&, double, double)> f7 = &Plus::plusd;
    cout << f7(move(pd), 1.1, 1.1) << endl;
    cout << f7(Plus(), 1.1, 1.1) << endl;
  
    return 0;
}
```

