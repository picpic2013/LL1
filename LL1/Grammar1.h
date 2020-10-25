#pragma once
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include "PredictionMatrix.h"
#include "Grammar2.h"
//刘黎明 and 王冬霞 ：求解First、Follow、Select集合
class Grammar1 {
public:
	Grammar1(char beginSign);
	Grammar1(const Grammar1& g);
	Grammar1(const Grammar2& g);
	~Grammar1();

	class iterator {
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
	void insert(const std::string& S, const std::string& a, bool autoUpdateV = true);
	void erase(const std::string& S, const std::string& a, bool autoUpdateV = true);
	iterator find(const std::string& S, const std::string& a);

	Grammar1 autoSplitOr();
	void autoSplitOrInPlace();

	void updateV();
	std::set<char> getV(bool autoUpdateFirst = true);
	std::set<char> getVN(bool autoUpdateFirst = true);
	std::set<char> getVT(bool autoUpdateFirst = true);
	std::set<std::pair<std::string, std::string> >getData();
	char getBeginSign()const;

	std::set<char> getFirst(const std::string inputString, bool autoSplitOr = true);
	static std::set<char> getFirst(Grammar1& g, const std::string inputString, bool autoSplitOr = true);
	std::set<char> getFollow(const char inputString, bool autoSplitOr = true);
	static std::set<char> getFollow(Grammar1& g, const std::string inputString, bool autoSplitOr = true);
	std::set<char> getSelect(const std::string S, const std::string a, bool autoSplitOr = true);

	std::set<std::string> getAllValues(const std::string key);
	std::set<std::string> getAllValues(const char key);

	bool isLeftRecursiveGrammar1();
	static bool isLeftRecursiveGrammar1(Grammar1 Grammar1);
	bool isLL1(bool autoSplitOr = true);
	PredictionMatrix getPredictionMatrix();

	const static char EMPTY_CHAR = '@';

	friend std::istream& operator>>(std::istream& in, Grammar1& g);
	friend std::ostream& operator<<(std::ostream& out, Grammar1& g);
	friend class PredictionMatrix;

	Grammar2 getGrammar2();
	bool fitGrammar(std::string inputString);


protected:
	std::set<std::pair<std::string, std::string> > data;
	std::set<char> vn, vt, v;
	std::map<std::string, std::set<char> > firstSetCache;
	std::map<char, std::set<char> > followSetCache;
	bool hasChanged;
	char beginSign;

	bool isVn(char ch);
	bool isVt(char ch);

	std::set<char> getFirstWithoutAutoSplit(const std::string inputString);
	std::set<char> getFollowWithoutAutoSplit(const char inputString, 
		std::map<char, int>& stringInStack);
	std::set<char> getFollowWithoutSplit(const char inputString);
	std::set<char> getSelectWithoutautoSplit(const std::string& S, const std::string& a);
};