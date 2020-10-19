#pragma once
#include <map>
#include <vector>
#include <string>

class Grammar {
public:
	Grammar();
	~Grammar();

	class iterator {
	public:
		iterator(std::vector<std::pair<std::string, std::string> >::iterator it);
		iterator(const iterator& iter);
		~iterator();
		std::string& getKey() const;
		std::string& getValue() const;
		iterator& operator=(const iterator& iter);
		bool operator==(const iterator& iter);
		bool operator!=(const iterator& iter);
		iterator operator++(int n);
		iterator& operator++();
		iterator operator--(int n);
		iterator& operator--();
		iterator operator+(int n);
		iterator operator-(int n);
		iterator& operator+=(int n);
		iterator& operator-=(int n);
		std::pair<std::string, std::string>& operator*();

	private:
		std::vector<std::pair<std::string, std::string> >::iterator it;
	};

	Grammar::iterator begin();
	Grammar::iterator end();
	int size() const;
	void insert(const std::string& S, const std::string& a);

private:
	std::vector<std::pair<std::string, std::string> > data;
};