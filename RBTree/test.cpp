#include<iostream>
#include<vector>
#include<string>
#include<ctime>
using namespace std;

#include"RBTree.h"


int main()
{
	const int N = 10000000;
	vector<int> v;
	v.reserve(N);
	srand(time(0));

	for (size_t i = 0; i < N; i++)
	{
		v.push_back(rand() + i);
		//cout << v.back() << endl;
	}

	size_t begin2 = clock();
	RBTree<int, int> t;
	for (auto e : v)
	{
		if (e == 29365)
		{
			int i = 0;
		}

		//cout << "Insert:" << e << "->";
		t.Insert(make_pair(e, e));
		//cout << t.IsBalance() << endl;
	}
	size_t end2 = clock();

	cout << "Insert:" << end2 - begin2 << endl;

	cout << t.IsBalance() << endl;
	cout << t.Height() << endl;
	cout << t.Size() << endl;

	size_t begin1 = clock();
	for (auto e : v)
	{
		t.Find(e);
	}

	for (size_t i = 0; i < N; i++)
	{
		t.Find((rand() + i));
	}

	size_t end1 = clock();

	cout << "Find:" << end1 - begin1 << endl;

	return 0;
}