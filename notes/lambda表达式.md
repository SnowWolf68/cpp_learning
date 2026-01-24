### lambda表达式语法

lambda表达式书写格式：

```cc
[capture-list] (parameters) mutable -> return-type { statement }
```

lambda表达式各部分说明

  * `[capture-list]` : 捕捉列表，该列表总是出现在lambda函数的开始位置，编译器根据[]来判断接下来的代码是否为lambda函数，捕捉列表能够捕捉上下文中的变量供lambda函数使用。
  * `(parameters)`：参数列表。与普通函数的参数列表一致，如果不需要参数传递，则可以连同()一起省略
  * `mutable`：默认情况下，lambda函数总是一个const函数，mutable可以取消其常量性。使用该修饰符时，参数列表不可省略(即使参数为空)。

  * `->returntype`：返回值类型。用追踪返回类型形式声明函数的返回值类型，没有返回值时此部分可省略。返回值类型明确情况下，也可省略，由编译器对返回类型进行推导。
  * `{statement}`：函数体。在该函数体内，除了可以使用其参数外，还可以使用所有捕获到的变量。

> 注意：
> 在lambda函数定义中，参数列表和返回值类型都是可选部分，而捕捉列表和函数体可以为空。因此C++11中最简单的lambda函数为：[]{}; 该lambda函数不能做任何事情。

```cc
int main() {
    
    // 最简单的lambda表达式, 该lambda表达式没有任何意义
    [] {};
    
    // 省略参数列表和返回值类型，返回值类型由编译器推导为int
    int a = 3, b = 4;
    [=] { return a + 3; };
    
    // 省略了返回值类型，无返回值类型
    auto fun1 = [&](int c) { b = a + c; };
    fun1(10) cout << a << " " << b << endl;
    
    // 各部分都很完善的lambda函数
    auto fun2 = [=, &b](int c) -> int { return b += a + c; };
    cout << fun2(10) << endl;
    
    // 复制捕捉x
    int x = 10;
    auto add_x = [x](int a) mutable {
        x *= 2;
        return a + x;
    };
    cout << add_x(10) << endl;
    return 0;
}
```

### 捕获列表

通过上述例子可以看出，lambda表达式实际上可以理解为无名函数，该函数无法直接调用，如果想要直接调用，可借助auto将其赋值给一个变量。

捕获列表说明

**捕获列表描述了上下文中那些数据可以被lambda使用**，以及使用的方式**传值**还是**传引用**。

- `[var]`: 表示值传递方式捕捉变量var
- `[=]`: 表示值传递方式捕捉所有父作用域中的变量(包括this)
- `[&var]`: 表示引用传递捕捉变量var
- `[&]`: 表示引用传递捕捉所有父作用域中的变量(包括this)
- `[this]`: 表示值传递方式捕捉当前的this指针

> 注意：
>
> a. 父作用域指包含lambda函数的语句块
>
> b. 语法上捕捉列表可由多个捕捉项组成，并以逗号分割。
>
> ​	比如：`[=, &a, &b]`: 以引用传递的方式捕捉变量a和b，值传递方式捕捉其他所有变量
>
> ​	`[&, a, this]`: 值传递方式捕捉变量a和this，引用方式捕捉其他变量
>
> c. 捕捉列表不允许变量重复传递，否则就会导致编译错误。
>
> ​	比如：`[=, a]`: =已经以值传递方式捕捉了所有变量，捕捉a重复
>
> d. 在块作用域以外的lambda函数捕捉列表必须为空。
>
> e. 在块作用域中的lambda函数仅能捕捉父作用域中局部变量，捕捉任何非此作用域或者非局部变量都
>
> 会导致编译报错。
>
> f. lambda表达式之间不能相互赋值，即使看起来类型相同
>
> ```cc
> void (*PF)();
> int main() {
>     auto f1 = [] { cout << "hello world" << endl; };
>     auto f2 = [] { cout << "hello world" << endl; };
>     // 此处先不解释原因，等lambda表达式底层实现原理看完后，大家就清楚了
>     // f1 = f2; // 编译失败--->提示找不到operator=()
>     // 允许使用一个lambda表达式拷贝构造一个新的副本
>     auto f3(f2);
>     f3();
>     // 可以将lambda表达式赋值给相同类型的函数指针
>     PF = f2;
>     PF();
>     return 0;
> }
> ```

### 函数对象与lambda表达式

函数对象，又称为仿函数，即可以想函数一样使用的对象，就是在类中重载了operator()运算符的类对象。  

```cc
class Rate {
  public:
    Rate(double rate) : _rate(rate) {}
    double operator()(double money, int year) { return money * _rate * year; }

  private:
    double _rate;
};
int main() {
    // 函数对象
    double rate = 0.49;
    Rate r1(rate);
    r1(10000, 2);
    // lambda
    auto r2 = [=](double monty, int year) -> double {
        return monty * rate * year;
    };
    r2(10000, 2);
    return 0;
}
```

从使用方式上来看，函数对象与lambda表达式完全一样。

函数对象将rate作为其成员变量，在定义对象时给出初始值即可，lambda表达式通过捕获列表可以直接将该变量捕获到。  

![image-20260124173806233](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_lambda表达式\image-20260124173806233.png)

实际在底层编译器对于lambda表达式的处理方式，完全就是按照函数对象的方式处理的，即：如果定义了一个lambda表达式，编译器会自动生成一个类，在该类中重载了operator()。  

### lambda表达式与函数指针

为什么**“无捕获 lambda”能赋给函数指针**？

语言标准里的**特例规则**

C++ 标准专门规定了一条：

> **无捕获 lambda（capture list 为空的 lambda）**  
> 可以**隐式转换**为一个**普通函数指针**，类型是：
> - 返回类型 = lambda 的返回类型  
> - 参数列表 = lambda 的参数列表

例如：

```cpp
using Func = int(*)(int);

Func f = [](int x) { return x + 1; };  // ✅ 允许
```

原因是标准在“闭包类型”里**等价地补了一条转换函数**，你可以想象编译器生成了类似这样的代码（伪代码）：

```cpp
struct __Lambda {
    static int invoke(int x) { return x + 1; }  // 静态函数，放 lambda 体

    int operator()(int x) const { 
        return invoke(x);
    }

    // 关键：提供到函数指针的转换
    using FP = int(*)(int);
    operator FP() const noexcept {
        return &__Lambda::invoke;
    }
};
```

于是：

```cpp
__Lambda lam;
Func f = lam;  // 调用上面的 operator FP()，得到函数指针
```

**这条转换规则只对“无捕获 lambda”成立**。











### misc

> ref: gpt
>
> 可以把“lambda 表达式”想象成：**在当前作用域里，临时生成了一个小函数对象**。
> 而**捕获列表**（`[]` 中的东西）就是：  
>
> > 告诉编译器：这个“小函数对象”要把外面哪些变量“带进去”，以及**怎么带**。
>
> ---
>
>  1. **先理解：lambda 背后其实是一个“对象”**
>
> 比如：
>
> ```cpp
> int x = 10;
> auto f = [x]() { return x + 1; };
> ```
>
> 编译器大致会生成一个类似这样的结构（伪代码）：
>
> ```cpp
> struct __Lambda {
>     int x;           // 捕获的变量变成这个对象的成员
> 
>     int operator()() const {   // 调用时执行这里
>         return x + 1;
>     }
> };
> ```
>
> 然后：
>
> ```cpp
> __Lambda tmp{ x };   // 把当前 x 拷贝进去
> auto f = tmp;
> ```
>
> **关键点**：  
> - 捕获列表里的变量 → 变成这个“lambda 对象”的**成员变量**。
> - 将来调用 `f()` 时，其实是调用这个对象的 `operator()`。
>
> 所以：
>
> > 捕获列表 = “这个小对象在创建时要记住哪些外部变量，以及是拷贝一份还是保存引用”。
>
> ---
>
>  2. **几种常见捕获方式**
>
>  **2.1 不捕获：`[]`**
>
> ```cpp
> int x = 10;
> auto f = []() { return 1; };   // 不能用到 x
> ```
>
> - `[]` 表示：**不捕获任何局部变量**。
> - Lambda 体内只能用：
>   - 参数
>   - 静态变量 / 全局变量
>   - 不依赖局部变量的东西
>
> ---
>
>  **2.2 按值捕获：`[=]` / `[x]`**
>
> ```cpp
> int x = 10, y = 20;
> 
> auto f1 = [=]() { return x + y; };    // 捕获当前作用域中“所有用到的局部变量”，按值拷贝
> auto f2 = [x]() { return x; };        // 只捕获 x，按值拷贝
> ```
>
> 特点：
>
> - 捕获时把当前值**拷贝**进 lambda 对象里。
> - 后续外部变量再变，lambda 里看到的还是**原来的那份拷贝**。
>
> 例：
>
> ```cpp
> int x = 10;
> auto f = [x]() { return x; };  // 此时 x=10 被拷贝
> 
> x = 20;
> std::cout << f() << std::endl; // 输出 10
> ```
>
> 可以理解为：**lambda 把“当时的值”存下来了**。
>
> ---
>
>  **2.3 按引用捕获：`[&]` / `[&x]`**
>
> ```cpp
> int x = 10, y = 20;
> 
> auto f1 = [&]() { return x + y; };    // 用到的局部变量都按引用捕获
> auto f2 = [&x]() { x += 1; };         // 只按引用捕获 x
> ```
>
> 特点：
>
> - lambda 里保存的是**引用**，用的是同一块内存。
> - 外部变量变，lambda 里看到的也是变后的值；lambda 内修改，也会改到外面。
>
> 例：
>
> ```cpp
> int x = 10;
> auto f = [&]() { x += 5; };
> 
> f();
> std::cout << x << std::endl;  // 输出 15
> ```
>
> 但要小心**生命周期**问题：
>
> ```cpp
> auto make_lambda() {
>     int x = 10;
>     return [&]() { return x; };   // ⚠️ 返回了引用局部变量的 lambda
> }                                 // x 已经销毁，lambda 里引用悬空，未定义行为
> ```
>
> **结论**：  
> 如果 lambda 要“带出当前作用域”（比如作为函数返回值、存到容器里），避免引用捕获局部变量。
>
> ---
>
>  **2.4 混合捕获：`[=,&x]` / `[&,x]`**
>
> ```cpp
> int x = 1, y = 2, z = 3;
> 
> // 默认按值捕获，但 x 按引用
> auto f1 = [=, &x]() { x = 10; return x + y + z; };
> 
> // 默认按引用捕获，但 x 按值
> auto f2 = [&, x]() { y = 20; return x + y + z; };
> ```
>
> - `[=,&x]`：**默认 by value**，但对指定变量用引用。
> - `[&,x]`：**默认 by reference**，但对指定变量用值。
>
> 这是为了在复杂场景下更精细控制“谁拷贝、谁引用”。
>
> ---
>
>  3. **特殊捕获：`this`、`*this`**
>
>  **3.1 捕获 `this`：`[this]` / `[=]`（C++11 起）**
>
> ```cpp
> struct A {
>     int n;
>     void foo() {
>         auto f = [this]() { return n; };   // 捕获 this 指针
>     }
> };
> ```
>
> 等价于把 `this` 指针拷到 lambda 对象里。  
> 但注意：**拷的是指针，指向的对象不是拷贝！**
>
> 如果对象被销毁，而 lambda 还在用：
>
> ```cpp
> A* pa = new A{10};
> auto f = [pa]() { return pa->n; };  // 类似捕获 this
> delete pa;
> f();  // ⚠️ 未定义行为
> ```
>
>  **3.2 捕获 `*this`（C++17）**
>
> ```cpp
> struct A {
>     int n;
>     auto make_lambda() {
>         return [*this]() { return n; }; // 捕获当前对象的拷贝
>     }
> };
> ```
>
> - 把整个对象 **按值拷贝** 进 lambda。
> - 即使原对象销毁，lambda 里用的是自己的那份拷贝，安全很多。
>
> ---
>
>  4. **初始化捕获（C++14+）：`[x = expr]`**
>
> 有时你想捕获的是“表达式的结果”，而不是现有变量：
>
> ```cpp
> int a = 1, b = 2;
> auto f = [sum = a + b]() { return sum; };  // 捕获 a+b 的值到一个新成员 sum 里
> ```
>
> C++14 起可以这样写，叫 **init-capture / generalized capture**：
>
> - 在 lambda 对象里生成一个新的成员 `sum`；
> - 用 `a + b` 的结果初始化它；
> - lambda 体内可以直接用 `sum`。
>
> 也可以结合引用：
>
> ```cpp
> auto f = [&ref = some_object.member]() { ref.do_something(); };
> ```
>
> ---
>
>  5. **捕获和参数的区别**
>
> 再看这一对比：
>
> ```cpp
> int x = 10;
> 
> // 捕获 x
> auto f1 = [x]() { return x; };
> 
> // 参数传 x
> auto f2 = [](int x) { return x; };
> 
> // 调用
> f1();          // 已经在创建时捕获了 x=10
> f2(x);        // 调用时传当前的 x
> ```
>
> - **捕获**：在**lambda 创建那一刻**把外部变量放进对象（值或引用）。
> - **参数**：在**每次调用**时从外面传值（或引用）。
>
> 你可以理解为：
>
> - 捕获 = “构造闭包对象时的构造参数”
> - 参数 = “调用 operator() 时的参数”

