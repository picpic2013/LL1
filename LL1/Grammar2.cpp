#include "Grammar2.h"

Grammar2::Grammar2() {
}

Grammar2::Grammar2(const Grammar2& g) {
	this->S = g.S;
	this->data = g.data;
	this->P = g.P;
	this->Vn = g.Vn;
	this->Vt = g.Vt;
}




Grammar2::~Grammar2() {
}

Grammar2::iterator::iterator(std::set<std::pair<std::string, std::string> >::iterator it) {
	this->it = it;
}

Grammar2::iterator::iterator(const iterator& iter) {
	this->it = iter.it;
}

Grammar2::iterator::~iterator() {}

std::string Grammar2::iterator::getKey() const {
	return this->it->first;
}

std::string Grammar2::iterator::getValue() const {
	return this->it->second;
}

bool Grammar2::iterator::operator==(const iterator& iter) {
	return this->it == iter.it;
}

bool Grammar2::iterator::operator!=(const iterator& iter) {
	return this->it != iter.it;
}

Grammar2::iterator Grammar2::iterator::operator++(int n) {
	iterator tmp = *this;
	++this->it;
	return tmp;
}

Grammar2::iterator& Grammar2::iterator::operator++() {
	++this->it;
	return *this;
}

Grammar2::iterator Grammar2::iterator::operator--(int n) {
	iterator tmp = *this;
	--this->it;
	return tmp;
}

Grammar2::iterator& Grammar2::iterator::operator--() {
	--this->it;
	return *this;
}


Grammar2::iterator Grammar2::begin() {
	return Grammar2::iterator(this->data.begin());
}

Grammar2::iterator Grammar2::end() {
	return Grammar2::iterator(this->data.end());
}

size_t Grammar2::size() const {
	return this->data.size();
}

void Grammar2::insert(const std::string& S, const std::string& a) {
	this->data.insert(std::make_pair(S, a));
}

void Grammar2::erase(const std::string& S, const std::string& a) {
	auto it = this->data.find(std::make_pair(S, a));
	if (it != this->data.end()) {
		this->data.erase(it);
	}
}

Grammar2 Grammar2::autoSplitOr() {
	Grammar2 tmp = Grammar2(*this);
	tmp.autoSplitOrInPlace();
	return tmp;
}

void Grammar2::autoSplitOrInPlace() {
	std::set<std::pair<std::string, std::string> > temp;
	for (auto it = this->data.begin(); it != this->data.end(); ++it) {
		std::string tempString = it->second;
		while (tempString.find("|") != std::string::npos) {
			temp.insert(std::make_pair(it->first, tempString.substr(0, tempString.find("|"))));
			tempString = tempString.substr(tempString.find("|") + 1, tempString.size());
		}
		temp.insert(std::make_pair(it->first, tempString));
	}
	this->data = temp;
}


//消除所有左递归
void Grammar2::remove_left_recursion()
{
	string tempVn = "";
	//将非终结符集转为string
	for (auto it = Vn.begin(); it != Vn.end(); it++)
	{
		tempVn += *it;
	}
	//遍历非终结符集
	for (unsigned int i = 0; i < tempVn.length(); i++)
	{
		//取出一个非终结符
		char pi = tempVn[i];
		set<string>NewPRight;
		//map P的key为char：是表达式的左侧；value为set<string>：是表达式的右侧
		//对每个非终结符，求它的所有右侧
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			bool isget = 0;
			//记录当前表达式的右侧
			string right = *it;
			//消除间接左递归
			for (unsigned int j = 0; j < i; j++)
			{
				//从第一个非终结符开始取
				char pj = tempVn[j];
				//判断前面的非终结符是否和当前表达式的右侧第一个字符相同
				if (pj == right[0])
				{
					isget = 1;
					//遍历前面的非终结符的右侧
					for (auto it1 = P[pj].begin(); it1 != P[pj].end(); it1++)
					{
						string s = *it1 + right.substr(1);
						NewPRight.insert(s);
					}
				}
			}

			//如果当前表达式的右侧尚未处理，就将其加入
			if (isget == 0)
			{
				NewPRight.insert(right);
			}
		}
		if (i != 0)
			P[pi] = NewPRight;

		//传递当前正在处理的非终结符
		remove_left_direct(pi);
	}
}

//消除直接左递归
void Grammar2::remove_left_direct(char c)
{
	char NewVn;//定义一个新的非终结符
	//如果它不在原来的Vn集里，就采用它
	for (int i = 0; i < 26; i++)
	{
		NewVn = i + 'A';
		if (!Vn.count(NewVn))
		{
			break;
		}
	}

	bool isaddNewVn = 0;

	for (auto it = P[c].begin(); it != P[c].end(); it++)
	{
		string right = *it;

		//如果存在左递归
		if (right[0] == c)
		{
			isaddNewVn = 1;
			break;
		}
	}

	if (isaddNewVn)
	{
		set<string>NewPRight;//经过变动以后，当前非终结符新的右侧表达式
		set<string>NewPNewVn;//新生成的非终结符的右侧表达式

		for (auto it = P[c].begin(); it != P[c].end(); it++)
		{
			string right = *it;
			if (right[0] != c)
			{
				//将新生成的非终结符加到末尾
				right += NewVn;
				NewPRight.insert(right);
			}
			else
			{
				//删去第一个字符
				right = right.substr(1);
				//将新生成的非终结符加到末尾
				right += NewVn;
				//将生成的新右侧表达式加入新集合
				NewPNewVn.insert(right);
			}
		}
		//新非终结符加入正式的非终结符集
		Vn.insert(NewVn);
		//将空串加入
		NewPNewVn.insert("@");
		//定义新非终结符的右侧表达式
		P[NewVn] = NewPNewVn;
		//更新当前非终结符的表达式
		P[c] = NewPRight;
	}
}

//判断是否含有左递归
bool Grammar2::if_left_recursion()
{
	bool isget = 0;
	string tempVn = "";
	for (auto it = Vn.begin(); it != Vn.end(); it++)
	{
		tempVn += *it;
	}
	for (unsigned int i = 0; i < tempVn.length(); i++)
	{
		char pi = tempVn[i];
		set<string>NewPRight;
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			string right = *it;
			for (unsigned int j = 0; j < i; j++)
			{
				char pj = tempVn[j];
				//判断是否存在间接左递归
				if (pj == right[0])
					isget = 1;
			}
			if (isget == 0)
			{
				NewPRight.insert(right);
			}
		}
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			string right = *it;
			//判断是否存在直接左递归
			if (right[0] == pi)
			{
				isget = 1;
				break;
			}
		}
	}
	return isget;
}

//初始化
void Grammar2::initialization(string filename)
{
	ifstream in(filename);
	if (!in.is_open())
	{
		cout << "文法  文件打开失败" << endl;
		exit(1);
	}
	char* buffer = new char[1e4];
	in.getline(buffer, 1e4, '#');
	string temps = "";
	bool is_sethead = 0;


	for (unsigned int i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] == '\n' || buffer[i] == ' ')
			continue;
		if (buffer[i] == ';')
		{
			if (!is_sethead)
			{
				S = temps[0];
				is_sethead = 1;
			}
			this->add(temps);
			temps = "";
		}

		else
			temps += buffer[i];
	}


	delete buffer;
}

//辅助函数
void Grammar2::add(string s)
{
	char s1 = s[0];
	string s2 = "";
	unsigned int num = 0;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '>')
			num = i;
		if (num == 0)
			continue;
		if (i > num)
			s2 += s[i];
	}
	s2 += ';';
	Vn.insert(s1);
	string temp = "";
	for (char s : s2)
	{
		if (!isupper(s) && s != '|' && s != ';' && s != '@')
			Vt.insert(s);
		if (s == '|' || s == ';')
		{
			P[s1].insert(temp);
			temp = "";
		}
		else
		{
			temp += s;
		}
	}
}

//提取左因子
int Grammar2::remove_left_factor()
{
	left_factor_initialization();
	char tt1 = 'z';
	int flagg = 1;
	for (int i = 0; i < 10; i++)
	{
		for (auto it = Vn.begin(); it != Vn.end(); it++)
		{
			string s;
			string s1;
			string s2;

			char c = *it;
			int flag = 0;
			//0表示这个非终结符没有左因子
			for (auto it1 = P[c].begin(); it1 != P[c].end(); it1++)
			{
				s = *it1;
				auto it2 = it1;
				it2++;
				while (it2 != P[c].end())
				{
					s1 = *it2;
					if (s[0] == s1[0])
					{
						tt1 = s[0];
						flag = 1;
						flagg = 0;
						break;
					}
					it2++;
				}
				if (flag == 1)
				{
					break;
				}
			}//对这个非终结符，找到公共左因子
			string del[20];
			int delnum = 0;
			//记录删去的产生式和数量
			if (flag == 1)
			{
				for (auto it3 = P[c].begin(); it3 != P[c].end(); it3++)
				{
					s2 = *it3;
					if (s2[0] == tt1)
					{
						del[delnum] = s2;
						delnum++;
					}
				}
				for (int i = 0; i < delnum; i++)
				{
					P[c].erase(del[i]);
				}
				//删去所有含左因子的产生式
				char NewVn;
				//如果不在，就退出
				for (int i = 0; i < 26; i++)
				{
					NewVn = i + 'A';
					if (!Vn.count(NewVn))
						break;
				}

				//将左因子后面的化为新非终结符

				//新非终结符集合中取出元素
				char sr[2];
				sr[0] = tt1;
				sr[1] = NewVn;
				string ssr0(sr, sr + 2);

				P[c].insert(ssr0);
				Vn.insert(NewVn);
				set<string> NPV;
				for (int i = 0; i < delnum; i++)
				{
					string ssr = del[i];
					string ssr1 = ssr.substr(1, ssr.length());
					if (ssr1.length() == 0)
						ssr1 = "@";
					NPV.insert(ssr1);
				}
				P[NewVn] = NPV;
				//新非终结符对应产生式
			}
		}
	}
	return flagg;
}

//做提取左因子的初始化操作
void Grammar2::left_factor_initialization()
{
	string tempVn = "";
	//将非终结符集转为string
	for (auto it = Vn.begin(); it != Vn.end(); it++)
		tempVn += *it;
	int flag = 0;
	string nv[10];
	int nvnum = 0;
	string dela[10];
	int deln = 0;
	//遍历非终结符集
	for (unsigned int i = 0; i < tempVn.length(); i++)
	{
		//取出一个非终结符
		char pi = tempVn[i];
		set<string>NewPRight;
		//map P的key为char：是表达式的左侧；value为set<string>：是表达式的右侧
		//对每个非终结符，求它的所有右侧
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			bool isget = 0;
			//记录当前表达式的右侧
			string right = *it;

			for (unsigned int j = 0; j < tempVn.length(); j++)
			{
				char pj = tempVn[j];
				if (pj == right[0] && pj != pi)
				{
					isget = 1;
					for (auto it1 = P[pj].begin(); it1 != P[pj].end(); it1++)
					{
						string s = *it1 + right.substr(1);
						NewPRight.insert(s);
						nv[nvnum] = s;
						nvnum++;
					}
					for (auto it3 = P[pi].begin(); it3 != P[pi].end(); it3++)
					{
						string itt3 = *it3;
						if (itt3[0] == pj)
						{
							dela[deln] = itt3;
							deln++;
						}
					}
					for (int p = 0; p < nvnum; p++)
					{
						P[pi].insert(nv[p]);
					}
					for (int i = 0; i < deln; i++)
					{
						P[pi].erase(dela[i]);
						flag = 1;
					}
				}
			}
			if (flag == 1)
				break;
		}
	}
}

//判断是否含有左因子
int Grammar2::if_left_factor()
{
	char tt1 = 'z';
	int flagg = 1;
	for (auto it = Vn.begin(); it != Vn.end(); it++)
	{
		char c = *it;
		for (auto it1 = P[c].begin(); it1 != P[c].end(); it1++)
		{
			string s = *it1;
			auto it2 = it1;
			it2++;
			while (it2 != P[c].end())
			{
				string s1 = *it2;
				if (s[0] == s1[0])
				{
					tt1 = s[0];
					flagg = 0;
					break;
				}
				it2++;
			}
			if (flagg == 0)
				break;
		}
		if (flagg == 0)
			break;
	}
	return flagg;
}

