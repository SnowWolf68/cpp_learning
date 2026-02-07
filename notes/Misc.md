#### 继承与多态中的缺省参数

+ **绝不应该在派生类（子类）中重新定义虚函数的缺省参数值。**

C++ 的处理规则如下：

1. **虚函数（Virtual Functions）** 是 **动态绑定（Dynamic Binding）** 的。这意味着程序在**运行**时，根据对象的实际类型（RTTI）来决定调用哪个版本的函数代码。
2. **缺省参数（Default Arguments）** 是 **静态绑定（Static Binding）** 的。这意味着编译器在**编译**阶段，根据指针或引用的**静态类型**（即代码中声明的类型）来决定填入什么默认值。

```cc
#include <iostream>
using namespace std;

class Base {
public:
	// 父类定义缺省参数为 10
	virtual void display(int i = 10) {
		cout << "Base::display, i = " << i << endl;
	}
	virtual void display2(int i = 30) = 0;
};

class Derived : public Base {
public:
	// 子类重新定义缺省参数为 20 (这是一个糟糕的做法！)
	void display(int i = 20) override {
		cout << "Derived::display, i = " << i << endl;
	}
	void display2(int i = 40) override {
		cout << "Derived::display2, i = " << i << endl;
	}
};

int main() {
	Base *p = new Derived(); // 静态类型是 Base*, 动态类型是 Derived*

	// 发生了什么？
	p->display();
	p->display2();

	delete p;
	return 0;
}
```

![image-20260205210757682](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_Misc\image-20260205210757682.png)

**分析：**

1. **编译阶段：** 编译器看到 `p->display()`。因为 `p` 的声明类型是 `Base*`，编译器查看 `Base` 类中该函数的定义，发现缺省参数是 `10`。于是编译器将 `10` 压入参数栈中。代码在编译后实际上变成了 `p->display(10)`。
2. **运行阶段：** 程序执行到这一行，发现 `display` 是虚函数，于是通过虚表（vtable）查找。因为 `p` 实际指向的是 `Derived` 对象，所以找到了 `Derived::display` 的函数体。
3. **最终组合：** 执行的是 **`Derived` 的函数体**，但参数却是 **`Base` 定义的默认值 `10`**。