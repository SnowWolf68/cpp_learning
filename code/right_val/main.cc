// Test.cpp
#include "List.hpp"
#include "string.hpp"
int main() {
	bit::list<bit::string> lt;
	cout << "*************************" << endl;

	bit::string s1("111111111111111111111");
	lt.push_back(s1);
	cout << "*************************" << endl;

	lt.push_back(bit::string("22222222222222222222222222222"));
	cout << "*************************" << endl;

	lt.push_back("3333333333333333333333333333");
	cout << "*************************" << endl;

	lt.push_back(move(s1));
	cout << "*************************" << endl;
    
	return 0;
}
// g++ main.cc -o main -fno-elide-constructors