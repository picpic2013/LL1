#include "Grammar.h"
#include <algorithm>
#include <iostream>

Grammar::Grammar(char beginSign) {
	this->hasChanged = false;
	this->beginSign = beginSign;
}

Grammar::Grammar(const Grammar& g) {
	this->beginSign = g.beginSign;
	this->data = g.data;
	this->firstSetCache = g.firstSetCache;
	this->followSetCache = g.followSetCache;
	
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
	this->hasChanged = true;
	if (autoUpdateV) {
		updateV();
	}
}

void Grammar::erase(const std::string& S, const std::string& a, bool autoUpdateV) {
	auto it = this->data.find(std::make_pair(S, a));
	if (it != this->data.end()) {
		this->data.erase(it);
	}
	this->hasChanged = true;
	if (autoUpdateV) {
		updateV();
	}
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

std::set<char> Grammar::getFirst(const std::string inputString, bool autoSplitOr) {
	if (this->hasChanged) {
		this->firstSetCache.clear();
		updateV();
	}

	Grammar newGrammar = *this;
	if (autoSplitOr) {
		newGrammar.autoSplitOrInPlace();
	}

	return newGrammar.getFirstWithoutAutoSplit(inputString);
}

std::set<char> Grammar::getFirst(Grammar& g, const std::string inputString, bool autoSplitOr) {
	return g.getFirst(inputString, autoSplitOr);
}

std::set<char> Grammar::getFollow(const char inputString, bool autoSplitOr) {
	std::map<char, int> stringInStack;
	
	Grammar newGrammar = *this;
	if (autoSplitOr) {
		newGrammar.autoSplitOrInPlace();
	}
	return this->getFollowWithoutAutoSplit(inputString, stringInStack);
}

std::set<char> Grammar::getFollow(Grammar& g, const std::string inputString, bool autoSplitOr) {
	return g.getFirst(inputString, autoSplitOr);
}

std::set<char> Grammar::getSelect(const std::string S, const std::string a, bool autoSplitOr) {
	if (this->hasChanged) {
		this->firstSetCache.clear();
		updateV();
	}

	Grammar newGrammar = *this;
	if (autoSplitOr) {
		newGrammar.autoSplitOrInPlace();
	}
	
	return newGrammar.getSelectWithoutautoSplit(S, a);
}

bool Grammar::isVn(char ch) {
	if (this->hasChanged) {
		updateV();
	}
	return this->vn.find(ch) != this->vn.end();
}

bool Grammar::isVt(char ch) {
	if (this->hasChanged) {
		updateV();
	}
	return this->vt.find(ch) != this->vt.end();
}

std::set<std::string> Grammar::getAllValues(const std::string key) {
	std::set<std::string> res;
	for (auto it : this->data) {
		if (it.first == key) {
			res.insert(it.second);
		}
	}
	return res;
}

std::set<std::string> Grammar::getAllValues(const char key_) {
	std::string key;
	key += key_;
	std::set<std::string> res;
	for (auto it : this->data) {
		if (it.first == key) {
			res.insert(it.second);
		}
	}
	return res;
}

bool Grammar::isLL1(bool autoSplitOr) {
	if (autoSplitOr) {
		Grammar newGrammar = *this;
		return newGrammar.isLL1(false);
	}
	std::map<std::string, std::set<char> > m;
	for (auto i : this->data) {
		if (m.find(i.first) == m.end()) {
			std::set<char> tmp;
			for (char ele : getSelect(i.first, i.second, true)) {
				tmp.insert(ele);
			}
			m[i.first] = tmp;
		}
		else {
			std::set<char>& tmp = m[i.first];
			for (char ele : getSelect(i.first, i.second, true)) {
				if (tmp.find(ele) != tmp.end()) {
					return false;
				}
				tmp.insert(ele);
			}
		}
	}
	return true;
}

PredictionMatrix Grammar::getPredictionMatrix() {
	PredictionMatrix m;
	if (!isLL1()) {
		return m;
	}
	for (auto i : this->data) {
		for (auto ele : getSelect(i.first, i.second)) {
			std::string tmp = "" + ele;
			m.add(i.first, tmp, i.second);
		}
	}
}

std::set<char> Grammar::getFirstWithoutAutoSplit(const std::string inputString) {
	if (this->firstSetCache.find(inputString) != this->firstSetCache.end()) {
		return this->firstSetCache[inputString];
	}

	std::string str = inputString;
	if (str[str.size() - 1] != Grammar::EMPTY_CHAR) {
		str += Grammar::EMPTY_CHAR;
	}

	std::set<char> res;

	for (int index = 0; index < str.size(); ++index) {
		char ch = str[index];
		if (isVt(ch) || ch == Grammar::EMPTY_CHAR) {
			res.insert(ch);
			return res;
		}
		bool hasNull = false;
		for (auto i : getAllValues(ch)) {
			std::string newStr = i + str.substr(static_cast<long long>(index) + 1, str.size());
			for (auto ele : getFirstWithoutAutoSplit(newStr)) {
				if (ele == Grammar::EMPTY_CHAR) {
					hasNull = true;
				}
				else {
					res.insert(ele);
				}
			}
		}
		if (!hasNull) {
			return res;
		}
	}

	this->firstSetCache.insert(std::make_pair(inputString, res));
	return res;
}

std::set<char> Grammar::getFollowWithoutAutoSplit(const char inputString, std::map<char, int>& stringInStack) {
	if (this->followSetCache.find(inputString) != this->followSetCache.end()) {
		return this->followSetCache[inputString];
	}

	std::set<char> res;

	if (inputString == this->beginSign) {
		res.insert('#');
	}

	if (stringInStack.find(inputString) != stringInStack.end()) {
		return res;
	}

	bool isInStack = stringInStack[inputString]++ != 0;

	for (auto i : this->data) {
		std::string afterString = i.second;
		while (afterString.find(inputString) != std::string::npos) {
			afterString = afterString.substr(afterString.find(inputString) + 1, afterString.size());
			if (afterString.size() == 0) {
				for (char ii : getFollowWithoutAutoSplit(i.first[0], stringInStack)) {
					res.insert(ii);
				}
				break;
			}
			if (isVt(afterString[0])) {
				res.insert(afterString[0]);
			}
			else {
				bool hasEmpty = false;
				for (char ii : getFirstWithoutAutoSplit(afterString)) {
					if (ii == Grammar::EMPTY_CHAR) {
						hasEmpty = true;
					}
					else {
						res.insert(ii);
					}
				}
				if (hasEmpty && !isInStack) {
					for (char ii : getFollowWithoutAutoSplit(i.first[0], stringInStack)) {
						res.insert(ii);
					}
				}
			}
		}
	}

	--stringInStack[inputString];

	this->followSetCache.insert(std::make_pair(inputString, res));
	return res;
}

std::set<char> Grammar::getFollowWithoutSplit(const char inputString) {
	std::map<char, int> stringInStack;
	return this->getFollowWithoutAutoSplit(inputString, stringInStack);
}

std::set<char> Grammar::getSelectWithoutautoSplit(const std::string& S, const std::string& a) {
	std::set<char> res;
	auto it = this->data.find(std::make_pair(S, a));
	if (S.size() != 1) {
		return res;
	}
	bool hasEmpty = false;
	for (char i : getFirstWithoutAutoSplit(a)) {
		if (i != Grammar::EMPTY_CHAR) {
			res.insert(i);
		}
		else {
			hasEmpty = true;
		}
	}
	if (hasEmpty) {
		for (char i : getFollowWithoutSplit(S[0])) {
			res.insert(i);
		}
	}
	return res;
}

std::istream& operator>>(std::istream& in, Grammar& g) {
	std::string input;
	while (in >> input) {
		size_t idx = input.find("->");
		if (idx != std::string::npos && idx == 1 && input.size() > 4 && idx < input.size() - 3) {
			g.insert(input.substr(0, idx), input.substr(idx), false);
		}
	}
	g.updateV();
	return in;
}

std::ostream& operator<<(std::ostream& out, Grammar& g) {
	for (auto it = g.begin(); it != g.end(); it++) {
		out << it.getKey() << " " << it.getValue() << std::endl;
	}
	return out;
}