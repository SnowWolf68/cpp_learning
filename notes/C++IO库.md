### 1. IO继承家族类

- C++语言不直接处理输入输出，而是通过一族定义在标准库中的类型在处理IO。这些类型支持从设备中读取数据和向设备中写入数据的IO操作，设备可以是文件、控制台窗口等。
- 到目前为止，我们已经使用过的IO类型和对象都是操纵char数据的，默认情况下这些对象都是关联到用户的控制台窗口。但是实际中IO类不仅仅是从控制台窗口控制输入输出，还支持文件和string类的IO操作。其次IO类型使用模板实现的，还支持对wchar_t数据的输入输出。
- 通过下图1-1和1-2可以看到C++IO类型设计的是一个继承家族，通过继承家族类解决控制台/文件/string的IO操作。
- https://legacy.cplusplus.com/reference/
- https://zh.cppreference.com/w/cpp/io

![image-20260126182243317](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_C++IO库\image-20260126182243317.png)

![image-20260126175821539](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_C++IO库\image-20260126175821539.png)

### 2. IO流状态

- IO操作的过程中，可能会发生各种错误，IO流对象中给了四种状态标识错误，可以参考下图2-1和2-2进行理解。goodbit表示流没有错误/eofbit表示流到达文件结束/failbit表示IO操作失败了/badbit表示流崩溃了出现了系统级错误。
- 一个常见的IO流错误是cin>>i，i是一个int类型的对象，如果我们在控制台输入一个字符，cin对象的failbit状态位就会被设置，cin就进入错误状态，一个流一旦发生错误，后续的IO操作都会失败，我们可以调用cin.clear()函数来恢复cin的状态为goodbit。
- badbit表示系统级错误，如不可恢复的读写错误，通常情况下，badbit一旦被设置了，流就无法再使用了。
- failbit表示一个逻辑错误，如期望读取一个整形，但是却读取到一个字符，failbit被设置了，流是可以恢复的，恢复以后可以继续使用。
- 如果到达文件结束位置eofbit和failbit都会被置位。如果想再次读取当前文件，可以恢复一下流的状态，同时重置一个文件指针位置。
- goodbit表示流未发生错误。
- 也可以用setstate和rdstate两个函数来控制流状态，eofbit/failbit/badbit/goodbit是ios_base基类中定义的静态成员变量，可以直接使用的，并且是他们是可以组合的位运算值，具体使用细节可以参考文档。

![image-20260126180459734](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_C++IO库\image-20260126180459734.png)

![image-20260126180504245](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_C++IO库\image-20260126180504245.png)

```cc
#include <iostream>
using namespace std;

int main() {

	cout << "good: " << cin.good() << endl;
	cout << "eof: " << cin.eof() << endl;
	cout << "bad: " << cin.bad() << endl;
	cout << "fail: " << cin.fail() << endl << endl;

	int i = 0;
	// 输入一个字符或多个字符，cin读取失败，流状态被标记为failbit
	cin >> i;
	cout << i << endl;
	cout << "good: " << cin.good() << endl;
	cout << "eof: " << cin.eof() << endl;
	cout << "bad: " << cin.bad() << endl;
	cout << "fail: " << cin.fail() << endl << endl;

	if (cin.fail()) {
		// clear可以恢复流状态位goodbit
		cin.clear();
		// 我们还要把缓冲区中的多个字符都读出来，读到数字停下来，否则再去cin>>i还是会失败
		char ch = cin.peek();
		while (!(ch >= '0' && ch <= '9')) {
			ch = cin.get();
			cout << "ch: " << ch << endl;;
			ch = cin.peek();
		}
		cout << endl;
	}

	cout << "good: " << cin.good() << endl;
	cout << "eof: " << cin.eof() << endl;
	cout << "bad: " << cin.bad() << endl;
	cout << "fail: " << cin.fail() << endl << endl;

	cin >> i;
	cout << "i: " << i << endl;

	return 0;
}
```

```shell
good: 1
eof: 0
bad: 0
fail: 0

a
0
good: 0
eof: 0
bad: 0
fail: 1

ch: a
ch: 

1

good: 1
eof: 0
bad: 0
fail: 0

i: 1
```

### 3. 管理输出缓冲区

- 任何输出流都管理着一个缓冲区，用来保存程序写的数据。如果我们执行 `os<<"hello world"`字符串可能立即输出，也可能被操作系统保存在缓冲区中，随后再输出。有了缓冲区机制，操作系统就可能将多个输出操作组合成为一个单一的系统级写操作。因为设备的写操作通常是很耗时的，允许操作系统将多个输出操作组合为单一的设备写操作肯可能带来很大的性能提升。
- 会触发缓冲区刷新，将数据真正的写到输出设备或文件的原因有很多，如：`<1>`程序正常结束；`<2>`缓冲区满了；`<3>`输出了操纵符endl 或 flush会立即刷新缓冲区`<4>`我们使用了操纵符unitbuf设置流的内部状态，来清空缓冲区，cerr就设置了unitbuf，所以cerr输出都是立即刷新的。`<5>`一个输出流关联到另一个流时，当这个流读写时，输出流会立即刷新缓冲区。例如默认情况下cerr和cin都被关联到cout，所以读cin或写cerr时，都会导致cout缓冲区会被立即刷新。

- tie可以支持跟其他流绑定和解绑，具体参考文档 https://legacy.cplusplus.com/reference/ios/ios/tie/

```cc
#include <fstream>
#include <iostream>
using namespace std;
void func(ostream& os) {
	os << "hello world";
	os << "hello bit";
	// "hello world"和"hello bit"是否输出不确定
	system("pause");
	// 遇到endl，"hello world"和"hello bit"一定刷新缓冲区输出了
	 os << endl;
	// os << flush;
	// int i;
	// cin >> i;
	os << "hello cat";
	// "hello cat"是否输出不确定
	system("pause");
}
int main() {
	ofstream ofs("test.txt");
	// func(cout);

	// unitbuf设置后，ofs每次写都直接刷新
	// ofs << unitbuf;
	// cin绑定到ofs，cin进行读时，会刷新ofs的缓冲区
	// cin.tie(&ofs);
	func(ofs);
	return 0;
}
```

> 这段代码主要想表达的是**C++ iostream 的缓冲与刷新机制**，以及一些“什么时候会自动刷新”的规则。核心知识点大概有这些：
>
> ---
>
> ##### 1) `ostream&` 参数：同一套输出代码可用于不同输出目标
> ```cpp
> void func(ostream& os) { ... }
> ```
> - `func` 接收的是 `std::ostream` 的引用，所以它既可以接收：
>   - `cout`（输出到控制台）
>   - `ofstream`（输出到文件）
>   - `ostringstream`（输出到字符串缓冲区）
> - 体现了 iostream 体系的“多态/抽象接口”思想：**对外只依赖 `ostream`，不关心具体流类型**。
>
> ---
>
> ##### 2) 缓冲区（buffer）：`<<` 不一定立刻把内容写到设备/文件
> ```cpp
> os << "hello world";
> os << "hello bit";
> system("pause");
> ```
> - `operator<<` 往往只是把数据写进 **流的缓冲区**，不保证立刻“可见”：
>   - 对 `ofstream`：通常是**全缓冲**，没 flush 前可能只在内存里，文件里暂时看不到。
>   - 对 `cout`：很多实现对终端是**行缓冲/或在某些情况下更频繁刷新**，但标准层面也不能保证“你一定立刻看到输出”。
>
> 所以你注释里说的“是否输出不确定”，想表达的就是：**没刷新时，写入可能还停留在缓冲区**，暂停时不一定已经落到外部设备。
>
> ---
>
> ##### 3) `std::endl` / `std::flush`：强制刷新缓冲区
> ```cpp
> // os << endl;   // 输出 '\n' 并 flush
> // os << flush;  // 仅 flush
> ```
> - `std::endl` = 写一个换行符 + `flush`
> - `std::flush` = 只刷新不换行
> - 一旦 flush，之前缓冲区里的 `"hello world"`、`"hello bit"` 就**一定**会被提交到对应的底层输出（文件/终端）。
>
> （顺带：如果只是 `'\n'`，它不等价于 `endl`，**不保证 flush**。）
>
> ---
>
> ##### 4) `std::unitbuf`：设置“每次输出都自动 flush”
> ```cpp
> // ofs << unitbuf;
> ```
> - 给输出流设置 `unitbuf` 标志后，**每次 `<<` 都会立刻刷新**。
> - 这能让“暂停时文件里一定看得到刚输出的内容”，代价是性能变差（频繁 flush）。
>
> ---
>
> ##### 5) `tie` 机制：输入流在读之前会刷新它绑定的输出流
> ```cpp
> // cin.tie(&ofs);
> ```
> - `tie` 的语义是：**当对该输入流（这里是 `cin`）执行输入操作前，会先 flush 被绑定的输出流**。
> - 默认情况下：`cin` 是 `tie(&cout)` 的，所以你在等待 `cin >> x` 前，`cout` 的提示信息一般会自动刷新出来。
> - 你这里把 `cin` 绑到 `ofs` 的意思是：**只要发生 `cin` 输入，就会先把 `ofs` 刷新**（确保文件输出不“悬着”）。
>
> > 注意：你当前代码里输入语句被注释了，所以 `tie` 这点只是用来说明机制。
>
> ---
>
> ##### 6) 析构/关闭也会刷新：但发生在作用域结束时
> 在你的程序里：
> ```cpp
> ofstream ofs("test.txt");
> func(ofs);
> return 0; // main 结束，ofs 析构 -> close -> flush
> ```
> - 即使你从不手动 flush，正常退出时 `ofstream` 析构通常会 `close()`，从而把缓冲刷到文件。
> - 但你在 `func` 里 `system("pause")` 暂停时，**`ofs` 还没析构**，所以那一刻文件内容可能还没更新——这就是你用 pause 观察“刷新前/刷新后差异”的目的。
>
> ---
>
> ###### 总结一句话
> 这段代码想说明：**输出流默认有缓冲，写入不等于立刻可见；`endl/flush/unitbuf/tie/析构 close` 都会影响“何时把缓冲真正写出去”。**
>
> 如果你希望我把“对 cout 和对文件 ofs 的缓冲策略差异”也按标准与常见实现分别讲清楚，我也可以补充。

```cc
#include<iostream>
using namespace std;
int main()
{
	// 在io需求比较高的地方，如部分大量输入的竞赛题中，加上以下几行代码可以提高C++IO效率
	// 并且建议用'\n'替代endl，因为endl会刷新缓冲区

	// 关闭标准 C++ 流是否与标准 C 流在每次输入/输出操作后同步。
	ios_base::sync_with_stdio(false);

	// 关闭同步后，以下程序可能顺序为b a c
	 std::cout << "a\n";
	 std::printf("b\n");
	 std::cout << "c\n";

	// 解绑cin和cout关联绑定的其他流
	cin.tie(nullptr);
	cout.tie(nullptr);

	return 0;
}
```

### 4. 标准IO流

- C++标准IO流前面已经使用得比较多了，C++标准IO流默认是关联到控制台窗口的。cin是istream类型全局对象，cout/cerr/clog是ostream类型的全局对象，内置类型这两个类都直接进行了重载实现，所以可以直接使用，自定义类型就需要我们自己重载<<和>>运算符。
- ostream和istream是不支持拷贝的，只支持移动(外部不能使用，因为是保护成员)。
- istream的cin对象支持转换为bool值，进行条件逻辑判断，一旦被设置了badbit或failbit标志位，就返回false，如果是goodbit就返回true。
- ostream和istream还有不少其他接口，实践中相对用得比较少，需要时大家查查文档。

```cc
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int main() {
	int i = 0, j = 1;
	// 持续的输入，要结束需要输入Ctrl+Z换行，Ctrl+Z用于告诉程序输入已经完成，类似于在文件末尾添加一个标记。
	// istream& operator>>(int i),
    // >>运算符重载的返回值是istream对象，istream对象可以调用operator bool转换为bool值
	// 本质在底层是将cin的eofbit和failbit标志位设置了，cin调用operator bool函数语法逻辑上实现转换为bool值
	while (cin >> i >> j) {
		cout << i << ":" << j << endl;
	}
	cout << cin.good() << endl;
	cout << cin.eof() << endl;
	cout << cin.bad() << endl;
	cout << cin.fail() << endl << endl;
	// 流一旦发生错误就不能再用了，清理重置一下再能使用
	cin.clear();
	string s;
	while (cin >> s) {
		cout << s << endl;
	}
}
```

### 5. 文件IO流

- ofstream是输出文件流，也就是写文件的流，ofstream是ostream的派生类；ifstream是输入文件流，也就是读文件的流，ifstream是istream的派生类；fstream是ifstream和ofstream的派生类，既可以读也可以写。
- https://legacy.cplusplus.com/reference/fstream/
- https://zh.cppreference.com/w/cpp/io
- 文件流对象可以在构造时打开文件，也可以调用open函数打开文件，打开文件的mode有5-1图中的几种。in为读打开；out为写打开；binary以二进制模式打开；ate打开后立即寻位到流结尾；app每次写入前先寻位到流结尾；trunc在打开时舍弃流的内容；这些值是ios_base中定义的成员变量继承下来的，并且他们也是组合的独立二进制位值，需要组合时，可以或到一起。他们之间的区别，具体参考下面的代码演示。
- 文件流打开后如果需要可以主动调用close函数关闭，也可以不关闭，因为流对象析构函数中会关闭。
- 文件流打开文件失败或读写失败，也会使用IO流状态标记，我们调用operator bool或operator!判断即可。
- ifstream文件流的读数据主要可以使用get/read/>>重载，ofstream文件流写数据主要可以使用put/write/<<重载，具体主要参考下面代码的演示。

- 相比c语言文件读写的接口，C++fstream流功能更强大方便，使用<<和>>进行文件读写很方便，尤其是针对自定义类型对象的读写。

![image-20260126195902590](E:\cpp_linux_learning\C++\learning\cpp_learning\notes\img_C++IO库\image-20260126195902590.png)

```cc
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
int main() {
	ofstream ofs("test.txt");

	// 字符和字符串的写
	ofs.put('x');
	ofs.write("hello\nworld", 11);
	// 使用<<进行写
	ofs << "22222222" << endl;
    ofs << "hello" << endl;

	int x = 111;
	double y = 1.11;
	ofs << x << endl;
	ofs << y << endl;
	ofs.close();

	// app和ate都是尾部追加，不同的是app不能移动文件指针，永远是在文件尾写
	// ate可以移动文件指针，写到其他位置
	ofs.open("test.txt", ios_base::out | ios_base::app);
	ofs << "1111111" << endl;
	ofs.seekp(0, ios_base::beg);
	ofs << x << " " << y << endl;
	ofs.close();

	ofs.open("test.txt", ios_base::out | ios_base::ate);
	ofs << "1111111" << endl;
	ofs.seekp(0, ios_base::beg);
	ofs << x << " " << y << endl;
	ofs.close();

	// out和 out|trunc都会先把数据清掉，再写数据（官方文档也明确是这样写的）
	// https://en.cppreference.com/w/cpp/io/basic_filebuf/open
	// 那么trunc存在的意义是什么呢？out|trunc更明确的表达了文件中有内容时要清除掉内容
	// 对于代码维护者和阅读者来说能清晰地理解这个行为，在一些复杂的文件系统环境或不同的
	// C++文件流实现库中,out行为不完全等同于截断内容的情况(虽然当前主流实现基本一致),
	// out|trunc更明确的表要清除内容的行为
	ofs.open("test.txt", ios_base::out);
	// ofs.open("test.txt", ios_base::out | ios_base::trunc);
	ofs << "xxxx";
	ofs.close();
    
	return 0;
}
```

```cc
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
	// 实现一个图片文件的复制，需要用二进制方式打开读写，第一个参数可以给文件的绝对路径
	ifstream ifs("D:\\360MoveData\\Users\\xjh\\Desktop\\11-29.png",
				 ios_base::in | ios_base::binary);
	ofstream ofs("D:\\360MoveData\\Users\\xjh\\Desktop\\11-29-copy.png",
				 ios_base::out | ios_base::binary);
	int n = 0;
	while (ifs && ofs) {
		char ch = ifs.get();
		ofs << ch;
		++n;
	}
	cout << n << endl;
	return 0;
}
```

```cc
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class Date {
	friend ostream& operator<<(ostream& out, const Date& d);
	friend istream& operator>>(istream& in, Date& d);

public:
	Date(int year = 1, int month = 1, int day = 1)
		: _year(year), _month(month), _day(day) {}

private:
	int _year;
	int _month;
	int _day;
};
istream& operator>>(istream& in, Date& d) {
	in >> d._year >> d._month >> d._day;
	return in;
}
ostream& operator<<(ostream& out, const Date& d) {
	out << d._year << " " << d._month << " " << d._day << endl;
	return out;
}
struct ServerInfo {
	// 二进制读写时，这里不能用string，否则写到文件中的是string中指向字符数组的指针
	// 若string对象析构后再去文件中读取string对象，string中读到是一个野指针。
	char _address[32];
	// string _address;
	int _port;
	Date _date;
};
struct ConfigManager {
public:
	ConfigManager(const char* filename) : _filename(filename) {}
	// 二进制写
	// 内存中怎么存，囫囵吞枣，就怎么直接写出去
	void WriteBin(const ServerInfo& info) {
		ofstream ofs(_filename, ios_base::out | ios_base::binary);
		ofs.write((const char*)&info, sizeof(info));
	}
	// 二进制读
	// 将文件中的内容直接囫囵吞枣，直接读到内存中
	void ReadBin(ServerInfo& info) {
		ifstream ifs(_filename, ios_base::in | ios_base::binary);
		ifs.read((char*)&info, sizeof(info));
	}
	void WriteText(const ServerInfo& info) {
		ofstream ofs(_filename);
		ofs << info._address << " " << info._port << " " << info._date;
	}
	void ReadText(ServerInfo& info) {
		ifstream ifs(_filename);
		ifs >> info._address >> info._port >> info._date;
	}

private:
	string _filename; // 配置文件
};
void WriteBin() {
	ServerInfo winfo = {"192.0.0.1111111111111111111111", 80, {2025, 1, 10}};
	// 二进制读写
	ConfigManager cf_bin("test.bin");
	cf_bin.WriteBin(winfo);
}
void ReadBin() {
	// 二进制读写
	ConfigManager cf_bin("test.bin");
	ServerInfo rbinfo;
	cf_bin.ReadBin(rbinfo);
	cout << rbinfo._address << " " << rbinfo._port << " " << rbinfo._date
		 << endl;
}
void WriteText() {
	ServerInfo winfo = {"192.0.0.1", 80, {2025, 1, 10}};

	// 文本读写
	ConfigManager cf_text("test.txt");
	cf_text.WriteText(winfo);
}
void ReadText() {
	ConfigManager cf_text("test.txt");
	ServerInfo rtinfo;
	cf_text.ReadText(rtinfo);
	cout << rtinfo._address << " " << rtinfo._port << " " << rtinfo._date
		 << endl;
}
int main() {
	WriteBin();
	ReadBin();
	WriteText();
	ReadText();
	return 0;
}
```

### 6. string IO流
- ostringstream是string的的写入流，ostringstream是ostream的派生类；istringstream是string的的读出流，istringstream是istream的派生类；stringstream是ostringstream和istringstream的派生类，既可以读也可以写。这里使用stringstream会很方便。
- stringstream系列底层维护了一个string类型的对象用来保存结果，使用方法跟上面的文件流类似，只是数据读写交互的都是底层的string对象。

- stringstream最常用的方式还是使用<<和>>重载，进行数据和string之间的IO转换。
- string流使用str函数获取底层的string对象，或者写入底层的string对象，具体细节参考下面代码理解。

```cc
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class Date {
	friend ostream& operator<<(ostream& out, const Date& d);
	friend istream& operator>>(istream& in, Date& d);

public:
	Date(int year = 1, int month = 1, int day = 1)
		: _year(year), _month(month), _day(day) {}

private:
	int _year;
	int _month;
	int _day;
};

istream& operator>>(istream& in, Date& d) {
	in >> d._year >> d._month >> d._day;
	return in;
}
ostream& operator<<(ostream& out, const Date& d) {
	out << d._year << " " << d._month << " " << d._day << endl;
	return out;
}

int main() {
	int i = 123;
	Date d = {2025, 4, 10};
	ostringstream oss;
	oss << i << endl;
	oss << d << endl;

	string s = oss.str();
	cout << s << endl;

    cout << "-----------------------------" << endl;

	// stringstream iss(s);
	// stringstream iss;
	// iss.str("100 2025 9 9");
	istringstream iss("100 2025 9 9");
	int j;
	Date x;
	iss >> j >> x;
	cout << j << endl;
	cout << x << endl;

    cout << "-----------------------------" << endl;

	int a = 1234;
	int b = 5678;
	string str;
	// 将一个整形变量转化为字符串，存储到string类对象中
	stringstream ss;
	ss << a << " " << b;
	ss >> str;
	cout << "str: " << str << endl;     // str: 1234
	cout << ss.fail() << endl;
	cout << ss.bad() << endl;

	// 注意多次转换时，必须使用clear将上次转换状态清空掉
	// stringstreams在转换结尾时(即最后一个转换后),会将其内部状态设置为badbit和failbit
	// 因此下一次转换是必须调用clear()将状态重置为goodbit才可以转换
	// 但是clear()不会将stringstreams底层字符串清空掉,str给一个空串可以清掉底层的字符串
	ss.clear();
	ss.str("");     // 把上一次剩下的5678清空
	double dd = 12.34;
	ss << dd;
	ss >> str;
	cout << str << endl;    // 12.34
	return 0;
}
```

```cc
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
class Date {
	friend ostream& operator<<(ostream& out, const Date& d);
	friend istream& operator>>(istream& in, Date& d);

public:
	Date(int year = 1, int month = 1, int day = 1)
		: _year(year), _month(month), _day(day) {}

private:
	int _year;
	int _month;
	int _day;
};
istream& operator>>(istream& in, Date& d) {
	in >> d._year >> d._month >> d._day;
	return in;
}
ostream& operator<<(ostream& out, const Date& d) {
	out << d._year << " " << d._month << " " << d._day << endl;
	return out;
}
struct ChatInfo {
	string _name; // 名字
	int _id;	  // id
	Date _date;	  // 时间
	string _msg;  // 聊天信息
};
int main() {
	// 结构信息序列化为字符串
	ChatInfo winfo = {"张三", 135246, {2022, 4, 10}, "晚上一起看电影吧"};
	ostringstream oss;
	oss << winfo._name << " " << winfo._id << " " << winfo._date << " "
		<< winfo._msg;
	string str = oss.str();
	cout << str << endl << endl;

	// 我们通过网络这个字符串发送给对象，实际开发中，信息相对更复杂，
	// 一般会选用Json、xml等方式进行更好的支持
	// 字符串解析成结构信息
	ChatInfo rInfo;
	istringstream iss(str);
	iss >> rInfo._name >> rInfo._id >> rInfo._date >> rInfo._msg;
	cout << "-------------------------------------------------------" << endl;
	cout << "姓名：" << rInfo._name << "(" << rInfo._id << ") ";
	cout << rInfo._date << endl;
	cout << rInfo._name << ":>" << rInfo._msg << endl;
	cout << "-------------------------------------------------------" << endl;
    
	return 0;
}
```

