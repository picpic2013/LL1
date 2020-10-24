#pragma once
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include "PredictionMatrix.h"

class Grammar {
public:
	Grammar(char beginSign);
	Grammar(const Grammar& g);
	~Grammar();

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

	Grammar autoSplitOr();
	void autoSplitOrInPlace();

	void updateV();
	std::set<char> getV(bool autoUpdateFirst = true);
	std::set<char> getVN(bool autoUpdateFirst = true);
	std::set<char> getVT(bool autoUpdateFirst = true);

	std::set<char> getFirst(const std::string inputString, bool autoSplitOr = true);
	static std::set<char> getFirst(Grammar& g, const std::string inputString, bool autoSplitOr = true);
	std::set<char> getFollow(const char inputString, bool autoSplitOr = true);
	static std::set<char> getFollow(Grammar& g, const std::string inputString, bool autoSplitOr = true);
	std::set<char> getSelect(const std::string S, const std::string a, bool autoSplitOr = true);

	std::set<std::string> getAllValues(const std::string key);
	std::set<std::string> getAllValues(const char key);

	bool isLL1(bool autoSplitOr = true);
	PredictionMatrix getPredictionMatrix();

	const static char EMPTY_CHAR = '@';

	friend std::istream& operator>>(std::istream& in, Grammar& g);
	friend std::ostream& operator<<(std::ostream& out, Grammar& g);

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