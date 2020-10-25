#pragma once
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
using namespace std;
//贾济源：消除左递归、左因子、判断是否存在左递归等
class Grammar2
{
public:
	Grammar2();
	Grammar2(const Grammar2& g);
	~Grammar2();

	set<char>Vn;//非终结符集
	set<char>Vt;//终结符集
	char S;//开始符
	map<char, set<string> > P;//规则集


	class iterator
	{
	public:
		iterator(std::set<std::pair<std::string, std::string> >::iterator it);
		iterator(const iterator& iter);
		~iterator();
		std::string getKey() const;
		std::string getValue() const;
		bool operator==(const iterator& iter);
		bool operator!=(const iterator& iter);
		iterator operator++(int n);
		iterator& operator++();
		iterator operator--(int n);
		iterator& operator--();

	private:
		std::set<std::pair<std::string, std::string> >::iterator it;
	};

	iterator begin();
	iterator end();

	size_t size() const;
	void insert(const std::string& S, const std::string& a);
	void erase(const std::string& S, const std::string& a);

	Grammar2 autoSplitOr();
	void autoSplitOrInPlace();

	//从txt文件初始化
	void initialization(string filename);

	//初始化的辅助函数
	void add(string s);

	//判断是否含有左递归
	bool if_left_recursion();

	//消除所有左递归
	void remove_left_recursion();

	//消除直接左递归
	void remove_left_direct(char c);

	//做提取左因子的初始化操作
	void left_factor_initialization();

	//判断是否含有左因子
	int if_left_factor();

	//提取左因子
	int remove_left_factor();


private:
	std::set<std::pair<std::string, std::string> > data;
};