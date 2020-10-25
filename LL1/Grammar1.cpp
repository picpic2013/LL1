#include "Grammar1.h"
#include <algorithm>
#include <iostream>

Grammar1::Grammar1(char beginSign) {
	this->hasChanged = false;
	this->beginSign = beginSign;
}

Grammar1::Grammar1(const Grammar1& g) {
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

Grammar1::Grammar1(const Grammar2& g) {
	this->beginSign = g.S;
	this->vn = g.Vn;
	this->vt = g.Vt;
	for (auto i : g.P) {
		for (auto j : i.second) {
			std::string tmp = ""; tmp = tmp + i.first;
			this->data.insert(make_pair(tmp, j));
		}
	}
	this->updateV();
	this->hasChanged = false;
}
Grammar1::~Grammar1() {
}

Grammar2 Grammar1::getGrammar2() {
	Grammar2 G2;
	G2.S = this->getBeginSign();
	G2.Vn = this->getVN();
	G2.Vt = this->getVT();
	for (auto it : this->getData()) {
		if (it.first.length() > 1) {
			cout << "转换出错" << endl;
			exit(1);
		}
		G2.P[it.first[0]].insert(it.second);
	}
	return G2;
}
Grammar1::iterator::iterator(std::set<std::pair<std::string, std::string> >::iterator it) {
	this->it = it;
}

Grammar1::iterator::iterator(const iterator& iter) {
	this->it = iter.it;
}

Grammar1::iterator::~iterator() {}

std::string Grammar1::iterator::getKey() const {
	return this->it->first;
}

std::string Grammar1::iterator::getValue() const {
	return this->it->second;
}

bool Grammar1::iterator::operator==(const iterator& iter) {
	return this->it == iter.it;
}

bool Grammar1::iterator::operator!=(const iterator& iter) {
	return this->it != iter.it;
}

Grammar1::iterator Grammar1::iterator::operator++(int n) {
	iterator tmp = *this;
	++this->it;
	return tmp;
}

Grammar1::iterator& Grammar1::iterator::operator++() {
	++this->it;
	return *this;
}

Grammar1::iterator Grammar1::iterator::operator--(int n) {
	iterator tmp = *this;
	--this->it;
	return tmp;
}

Grammar1::iterator& Grammar1::iterator::operator--() {
	--this->it;
	return *this;
}


Grammar1::iterator Grammar1::begin() {
	return Grammar1::iterator(this->data.begin());
}

Grammar1::iterator Grammar1::end() {
	return Grammar1::iterator(this->data.end());
}

size_t Grammar1::size() const {
	return this->data.size();
}

void Grammar1::insert(const std::string& S, const std::string& a, bool autoUpdateV) {
	this->data.insert(std::make_pair(S, a));
	this->hasChanged = true;
	if (autoUpdateV) {
		updateV();
	}
}

void Grammar1::erase(const std::string& S, const std::string& a, bool autoUpdateV) {
	auto it = this->data.find(std::make_pair(S, a));
	if (it != this->data.end()) {
		this->data.erase(it);
	}
	this->hasChanged = true;
	if (autoUpdateV) {
		updateV();
	}
}

Grammar1::iterator Grammar1::find(const std::string& S, const std::string& a) {
	return iterator(this->data.find(std::make_pair(S, a)));
}

Grammar1 Grammar1::autoSplitOr() {
	Grammar1 tmp = Grammar1(*this);
	tmp.autoSplitOrInPlace();
	return tmp;
}

void Grammar1::autoSplitOrInPlace() {
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

std::set<char> Grammar1::getV(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->v;
}

std::set<char> Grammar1::getVN(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->vn;
}

std::set<char> Grammar1::getVT(bool autoUpdateFirst) {
	if (autoUpdateFirst && this->hasChanged) {
		updateV();
	}
	return this->vt;
}

std::set<std::pair<std::string, std::string> > Grammar1::getData() {
	return this->data;
}

char Grammar1::getBeginSign() const{
	return this->beginSign;
}

void Grammar1::updateV() {
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

std::set<char> Grammar1::getFirst(std::string inputString, bool autoSplitOr) {
	if (this->hasChanged) {
		this->firstSetCache.clear();
		updateV();
	}

	Grammar1 newGrammar1 = *this;
	if (autoSplitOr) {
		newGrammar1.autoSplitOrInPlace();
	}

	return newGrammar1.getFirstWithoutAutoSplit(inputString);;
}


std::set<char> Grammar1::getFirst(Grammar1& g, const std::string inputString, bool autoSplitOr) {
	return g.getFirst(inputString, autoSplitOr);
}


std::set<char> Grammar1::getFollow(const char inputString, bool autoSplitOr) {
	std::map<char, int> stringInStack;
	
	Grammar1 newGrammar1 = *this;
	if (autoSplitOr) {
		newGrammar1.autoSplitOrInPlace();
	}
	return this->getFollowWithoutAutoSplit(inputString, stringInStack);
}

std::set<char> Grammar1::getFollow(Grammar1& g, const std::string inputString, bool autoSplitOr) {
	return g.getFirst(inputString, autoSplitOr);
}

std::set<char> Grammar1::getSelect(const std::string S, const std::string a, bool autoSplitOr) {
	if (this->hasChanged) {
		this->firstSetCache.clear();
		updateV();
	}

	Grammar1 newGrammar1 = *this;
	if (autoSplitOr) {
		newGrammar1.autoSplitOrInPlace();
	}
	
	return newGrammar1.getSelectWithoutautoSplit(S, a);
}

bool Grammar1::isVn(char ch) {
	if (this->hasChanged) {
		updateV();
	}
	return this->vn.find(ch) != this->vn.end();
}

bool Grammar1::isVt(char ch) {
	if (this->hasChanged) {
		updateV();
	}
	return this->vt.find(ch) != this->vt.end();
}

std::set<std::string> Grammar1::getAllValues(const std::string key) {
	std::set<std::string> res;
	for (auto it : this->data) {
		if (it.first == key) {
			res.insert(it.second);
		}
	}
	return res;
}

std::set<std::string> Grammar1::getAllValues(const char key_) {
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
/***************
*判断是否是一个左递归文法
*author: Wong DongXia
*/
bool Grammar1::isLeftRecursiveGrammar1() {
	for (auto it : this->vn) {
		std::set<std::string> values;
		values = this->getAllValues(it);
		for (auto itt : values) {
			if (itt[0] == it) {
				std::cout << "存在左递归" << std::endl;
				return true;
			}
		}
	}
	return false;
}

bool Grammar1::isLeftRecursiveGrammar1(Grammar1 Grammar1) {
	return Grammar1.isLeftRecursiveGrammar1();
}

bool Grammar1::isLL1(bool autoSplitOr) {
	if (autoSplitOr) {
		return this->autoSplitOr().isLL1(false);
	}
	if (this->isLeftRecursiveGrammar1()) {
		return false;
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

PredictionMatrix Grammar1::getPredictionMatrix() {
	PredictionMatrix m;
	if (!isLL1()) {
		return m;
	}
	for (auto i : this->data) {
		for (auto ele : getSelect(i.first, i.second)) {
			std::string tmp = "";
			tmp = tmp + ele;
			m.add(i.first, tmp, i.second);
		}
	}
	return m;
}


/****
*
*@author: Wong DongXia
*/
std::set<char> Grammar1::getFirstWithoutAutoSplit(const std::string inputString) {
	if (this->hasChanged) {
		this->firstSetCache.clear();
		updateV();
	}

	std::set<char> res;
	std::map<std::string, std::set<char> >::iterator it;

	it = this->firstSetCache.find(inputString);
	if (it != this->firstSetCache.end()) return (*it).second;

	if (inputString == "@") res.insert('@');

	else if (inputString.length() == 1) {
		if (this->isVt(inputString[0])) {
			res.insert(inputString[0]);
		}
		else if (this->isVn(inputString[0])) {
			std::string tmps = "";
			tmps += inputString[0];
			std::set<std::string> values = this->getAllValues(tmps);
			std::set<char> tmp;
			for (auto it = values.begin(); it != values.end(); it++) {
				tmp = this->getFirst(*it);
				for (auto it = tmp.begin(); it != tmp.end(); it++) {
					res.insert(*it);
				}
			}
		}
	}
	else if (inputString.length() > 1) {
		if (this->isVt(inputString[0])) {
			res.insert(inputString[0]);
		}
		else if (this->isVn(inputString[0])) {
			std::set<char> tmp;
			bool hasSpace = false;
			std::string tmps = "";
			tmps += inputString[0];
			tmp = this->getFirst(tmps);
			for (auto it = tmp.begin(); it != tmp.end(); it++) {
				if ((*it) == '@')
				{
					hasSpace = true;
					break;
				}
			}
			if (hasSpace) {
				res = tmp;
				res.erase('@');
				tmp = this->getFirst(inputString.substr(1));
				for (auto it = tmp.begin(); it != tmp.end(); it++) {
					res.insert(*it);
				}
			}
			else {
				res = tmp;
			}
		}
	}
	this->firstSetCache[inputString] = res;
	return res;
}

/*std::set<char> Grammar1::getFirstWithoutAutoSplit(const std::string inputString) {
	if (this->firstSetCache.find(inputString) != this->firstSetCache.end()) {
		return this->firstSetCache[inputString];
	}

	std::string str = inputString;
	if (str[str.size() - 1] != Grammar1::EMPTY_CHAR) {
		str += Grammar1::EMPTY_CHAR;
	}

	std::set<char> res;

	for (int index = 0; index < str.size(); ++index) {
		char ch = str[index];
		if (isVt(ch) || ch == Grammar1::EMPTY_CHAR) {
			res.insert(ch);
			return res;
		}
		bool hasNull = false;
		for (auto i : getAllValues(ch)) {
			std::string newStr = i + str.substr(static_cast<long long>(index) + 1, str.size());
			for (auto ele : getFirstWithoutAutoSplit(newStr)) {
				if (ele == Grammar1::EMPTY_CHAR) {
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
*/

std::set<char> Grammar1::getFollowWithoutAutoSplit(const char inputString, std::map<char, int>& stringInStack) {
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
					if (ii == Grammar1::EMPTY_CHAR) {
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

std::set<char> Grammar1::getFollowWithoutSplit(const char inputString) {
	std::map<char, int> stringInStack;
	return this->getFollowWithoutAutoSplit(inputString, stringInStack);
}

std::set<char> Grammar1::getSelectWithoutautoSplit(const std::string& S, const std::string& a) {
	std::set<char> res;
	auto it = this->data.find(std::make_pair(S, a));
	if (S.size() != 1) {
		return res;
	}
	bool hasEmpty = false;
	for (char i : getFirstWithoutAutoSplit(a)) {
		if (i != Grammar1::EMPTY_CHAR) {
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

std::istream& operator>>(std::istream& in, Grammar1& g) {
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

std::ostream& operator<<(std::ostream& out, Grammar1& g) {
	for (auto it = g.begin(); it != g.end(); it++) {
		out << it.getKey() << " " << it.getValue() << std::endl;
	}
	return out;
}

bool Grammar1::fitGrammar( std::string inputString) {
	auto matrix = this->getPredictionMatrix();
	inputString = inputString + '#';
	int cur = 0;
	std::stack<char> anayleStack;
	anayleStack.push('#');
	anayleStack.push(this->getBeginSign());

	while (!anayleStack.empty() && cur < inputString.size()) {
		while (inputString[cur] == '@')cur++;
		char tmpStackTop = anayleStack.top();
		if (this->isVn(tmpStackTop)) {
			std::string tmps = ""; tmps = tmps + tmpStackTop;
			std::string tmpss = ""; tmpss = tmpss + inputString[cur];
			if (matrix.has(tmps, tmpss)) {
				anayleStack.pop();
				std::string res = matrix.get(tmps, tmpss);
				for (int i = res.length() - 1; i >= 0; i--) {
					anayleStack.push(res[i]);
				}
			}
			else {
				return false;
			}
		}
		else {
			if (tmpStackTop == '@') {
				anayleStack.pop();
				continue;
			}
			else if (tmpStackTop != inputString[cur])
				return false;
			else {
				cur++;
				anayleStack.pop();
			}
		}
	}
	if (anayleStack.empty() && inputString.size() == cur)return true;
	 return false;
}