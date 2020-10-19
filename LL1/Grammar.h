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
	void insert(const std::string& S, const std::string& a);
	void erase(const std::string& S, const std::string& a);

	Grammar autoSplitOr();
	void autoSplitOrInPlace();

private:
	std::set<std::pair<std::string, std::string> > data;
};