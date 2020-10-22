#include "Grammar.h"
#include <algorithm>
#include <iostream>

Grammar::Grammar() {
	this->hasChanged = false;
}

Grammar::Grammar(const Grammar& g) {
	this->data = g.data;
	
	if (g.hasChanged) {
		updateV();
	}
	else {
		this->v = g.v;
		this->vn = g.vn;
		this->vt = g.vt;
	}
	this->hasChanged = false;
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

void Grammar::insert(const std::string& S, const std::string& a, bool autoUpdateV) {
	this->data.insert(std::make_pair(S, a));
	if (autoUpdateV) {
		updateV();
	}
	this->hasChanged = true;
}

void Grammar::erase(const std::string& S, const std::string& a, bool autoUpdateV) {
	auto it = this->data.find(std::make_pair(S, a));
	if (it != this->data.end()) {
		this->data.erase(it);
	}
	if (autoUpdateV) {
		updateV();
	}
	this->hasChanged = true;
}

Grammar::iterator Grammar::find(const std::string& S, const std::string& a) {
	return iterator(this->data.find(std::make_pair(S, a)));
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

std::set<char> Grammar::getV(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->v;
}

std::set<char> Grammar::getVN(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->vn;
}

std::set<char> Grammar::getVT(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->vt;
}

void Grammar::updateV() {
	this->v.clear();
	this->vn.clear();
	this->vt.clear();

	for (auto it = this->begin(); it != this->end(); ++it) {
		if (it.getKey().size() == 1 && it.getKey()[0] != '|') {
			this->v.insert(it.getKey()[0]);
			this->vn.insert(it.getKey()[0]);
		}
		for (char i : it.getValue()) {
			if (i != '|') {
				this->v.insert(i);
			}
		}
	}

	std::set_difference(this->v.begin(), this->v.end(), this->vn.begin(), this->vn.end(), inserter(this->vt, this->vt.begin()));
	
	this->hasChanged = false;
}

std::set<char> Grammar::getFirst(std::string str) {
	std::set<char> res;

	if (str.size() == 0) {
		return res;
	}

	if (str.size() == 1 && (str[0] == '\0' || str[0] == '@')) {
		res.insert('@');
		return res;
	}

	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		if (this->vt.find(*it) != this->vt.end()) {
			res.insert(*it);
			return res;
		}
		std::set<char> tempSet = getFirst(str.substr(it - str.begin() + 1, str.size()));

		for (char i : tempSet) {
			res.insert(i);
		}

		if (tempSet.find('@') == tempSet.end()) {
			break;
		}
	}

	return res;
}