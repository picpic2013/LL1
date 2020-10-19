#include "Grammar.h"

Grammar::Grammar() {
}

Grammar::Grammar(const Grammar& g) {
	this->data = g.data;
}

Grammar::~Grammar() {
}

Grammar::iterator::iterator(std::set<std::pair<std::string, std::string> >::iterator it) {
	this->it = it;
}

Grammar::iterator::iterator(const iterator& iter) {
	this->it = iter.it;
}

Grammar::iterator::~iterator() {}

std::string Grammar::iterator::getKey() const {
	return this->it->first;
}

std::string Grammar::iterator::getValue() const {
	return this->it->second;
}

bool Grammar::iterator::operator==(const iterator& iter) {
	return this->it == iter.it;
}

bool Grammar::iterator::operator!=(const iterator& iter) {
	return this->it != iter.it;
}

Grammar::iterator Grammar::iterator::operator++(int n) {
	iterator tmp = *this;
	++this->it;
	return tmp;
}

Grammar::iterator& Grammar::iterator::operator++() {
	++this->it;
	return *this;
}

Grammar::iterator Grammar::iterator::operator--(int n) {
	iterator tmp = *this;
	--this->it;
	return tmp;
}

Grammar::iterator& Grammar::iterator::operator--() {
	--this->it;
	return *this;
}


Grammar::iterator Grammar::begin() {
	return Grammar::iterator(this->data.begin());
}

Grammar::iterator Grammar::end() {
	return Grammar::iterator(this->data.end());
}

size_t Grammar::size() const {
	return this->data.size();
}

void Grammar::insert(const std::string& S, const std::string& a) {
	this->data.insert(std::make_pair(S, a));
}

void Grammar::erase(const std::string& S, const std::string& a) {
	auto it = this->data.find(std::make_pair(S, a));
	if (it != this->data.end()) {
		this->data.erase(it);
	}
}

Grammar Grammar::autoSplitOr() {
	Grammar tmp = Grammar(*this);
	tmp.autoSplitOrInPlace();
	return tmp;
}

void Grammar::autoSplitOrInPlace() {
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