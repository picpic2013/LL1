#pragma once
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>

class Grammar {
public:
	Grammar();
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

	std::set<char> getFirst(const std::string str, bool autoSplitOr = true);
	static std::set<char> getFirst(Grammar& g, const std::string str, bool autoSplitOr = true);

	std::set<std::string> getAllValues(const std::string key);
	std::set<std::string> getAllValues(const char key);

	const static char EMPTY_CHAR = '@';

protected:
	std::set<std::pair<std::string, std::string> > data;
	std::set<char> vn, vt, v;
	std::map<std::string, std::set<char> > firstSetCache;
	bool hasChanged;

	bool isVn(char ch);
	bool isVt(char ch);
};