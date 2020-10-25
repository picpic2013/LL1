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


//����������ݹ�
void Grammar2::remove_left_recursion()
{
	string tempVn = "";
	//�����ս����תΪstring
	for (auto it = Vn.begin(); it != Vn.end(); it++)
	{
		tempVn += *it;
	}
	//�������ս����
	for (unsigned int i = 0; i < tempVn.length(); i++)
	{
		//ȡ��һ�����ս��
		char pi = tempVn[i];
		set<string>NewPRight;
		//map P��keyΪchar���Ǳ��ʽ����ࣻvalueΪset<string>���Ǳ��ʽ���Ҳ�
		//��ÿ�����ս���������������Ҳ�
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			bool isget = 0;
			//��¼��ǰ���ʽ���Ҳ�
			string right = *it;
			//���������ݹ�
			for (unsigned int j = 0; j < i; j++)
			{
				//�ӵ�һ�����ս����ʼȡ
				char pj = tempVn[j];
				//�ж�ǰ��ķ��ս���Ƿ�͵�ǰ���ʽ���Ҳ��һ���ַ���ͬ
				if (pj == right[0])
				{
					isget = 1;
					//����ǰ��ķ��ս�����Ҳ�
					for (auto it1 = P[pj].begin(); it1 != P[pj].end(); it1++)
					{
						string s = *it1 + right.substr(1);
						NewPRight.insert(s);
					}
				}
			}

			//�����ǰ���ʽ���Ҳ���δ�����ͽ������
			if (isget == 0)
			{
				NewPRight.insert(right);
			}
		}
		if (i != 0)
			P[pi] = NewPRight;

		//���ݵ�ǰ���ڴ���ķ��ս��
		remove_left_direct(pi);
	}
}

//����ֱ����ݹ�
void Grammar2::remove_left_direct(char c)
{
	char NewVn;//����һ���µķ��ս��
	//���������ԭ����Vn����Ͳ�����
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

		//���������ݹ�
		if (right[0] == c)
		{
			isaddNewVn = 1;
			break;
		}
	}

	if (isaddNewVn)
	{
		set<string>NewPRight;//�����䶯�Ժ󣬵�ǰ���ս���µ��Ҳ���ʽ
		set<string>NewPNewVn;//�����ɵķ��ս�����Ҳ���ʽ

		for (auto it = P[c].begin(); it != P[c].end(); it++)
		{
			string right = *it;
			if (right[0] != c)
			{
				//�������ɵķ��ս���ӵ�ĩβ
				right += NewVn;
				NewPRight.insert(right);
			}
			else
			{
				//ɾȥ��һ���ַ�
				right = right.substr(1);
				//�������ɵķ��ս���ӵ�ĩβ
				right += NewVn;
				//�����ɵ����Ҳ���ʽ�����¼���
				NewPNewVn.insert(right);
			}
		}
		//�·��ս��������ʽ�ķ��ս����
		Vn.insert(NewVn);
		//���մ�����
		NewPNewVn.insert("@");
		//�����·��ս�����Ҳ���ʽ
		P[NewVn] = NewPNewVn;
		//���µ�ǰ���ս���ı��ʽ
		P[c] = NewPRight;
	}
}

//�ж��Ƿ�����ݹ�
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
				//�ж��Ƿ���ڼ����ݹ�
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
			//�ж��Ƿ����ֱ����ݹ�
			if (right[0] == pi)
			{
				isget = 1;
				break;
			}
		}
	}
	return isget;
}

//��ʼ��
void Grammar2::initialization(string filename)
{
	ifstream in(filename);
	if (!in.is_open())
	{
		cout << "�ķ�  �ļ���ʧ��" << endl;
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

//��������
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

//��ȡ������
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
			//0��ʾ������ս��û��������
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
			}//��������ս�����ҵ�����������
			string del[20];
			int delnum = 0;
			//��¼ɾȥ�Ĳ���ʽ������
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
				//ɾȥ���к������ӵĲ���ʽ
				char NewVn;
				//������ڣ����˳�
				for (int i = 0; i < 26; i++)
				{
					NewVn = i + 'A';
					if (!Vn.count(NewVn))
						break;
				}

				//�������Ӻ���Ļ�Ϊ�·��ս��

				//�·��ս��������ȡ��Ԫ��
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
				//�·��ս����Ӧ����ʽ
			}
		}
	}
	return flagg;
}

//����ȡ�����ӵĳ�ʼ������
void Grammar2::left_factor_initialization()
{
	string tempVn = "";
	//�����ս����תΪstring
	for (auto it = Vn.begin(); it != Vn.end(); it++)
		tempVn += *it;
	int flag = 0;
	string nv[10];
	int nvnum = 0;
	string dela[10];
	int deln = 0;
	//�������ս����
	for (unsigned int i = 0; i < tempVn.length(); i++)
	{
		//ȡ��һ�����ս��
		char pi = tempVn[i];
		set<string>NewPRight;
		//map P��keyΪchar���Ǳ��ʽ����ࣻvalueΪset<string>���Ǳ��ʽ���Ҳ�
		//��ÿ�����ս���������������Ҳ�
		for (auto it = P[pi].begin(); it != P[pi].end(); it++)
		{
			bool isget = 0;
			//��¼��ǰ���ʽ���Ҳ�
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

//�ж��Ƿ���������
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

