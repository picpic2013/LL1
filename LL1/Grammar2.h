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
//�ּ�Դ��������ݹ顢�����ӡ��ж��Ƿ������ݹ��
class Grammar2
{
public:
	Grammar2();
	Grammar2(const Grammar2& g);
	~Grammar2();

	set<char>Vn;//���ս����
	set<char>Vt;//�ս����
	char S;//��ʼ��
	map<char, set<string> > P;//����


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

	//��txt�ļ���ʼ��
	void initialization(string filename);

	//��ʼ���ĸ�������
	void add(string s);

	//�ж��Ƿ�����ݹ�
	bool if_left_recursion();

	//����������ݹ�
	void remove_left_recursion();

	//����ֱ����ݹ�
	void remove_left_direct(char c);

	//����ȡ�����ӵĳ�ʼ������
	void left_factor_initialization();

	//�ж��Ƿ���������
	int if_left_factor();

	//��ȡ������
	int remove_left_factor();


private:
	std::set<std::pair<std::string, std::string> > data;
};