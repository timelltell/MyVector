// SimpleSTL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include "VectorConstruct.h"
#include "VectorAllocator.h"
#include "MyVector.h"

int main()
{
	using namespace::CBVector;

	cbVector<int> test(2, 2);
	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	test.push_back(1);
	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	test.push_back(2);
	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	test.push_back(3);
	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	test.pop_back();
	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	cbVector<int>::iterator  a=test.begin();
	test.erase(a);

	cout << "size = " << test.size() << "\t";
	cout << "capacity = " << test.capacity() << endl;

	std::cout << test[2] << endl;// 返回指定位置处的元素，这里体现了vector迭代器的随机访问性质（random access iterators）

	test.erase(test.begin() + 1, test.begin() + 2);
	test.erase(test.begin());
	for (cbVector<int>::iterator it = test.begin(); it != test.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	system("pause");
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
