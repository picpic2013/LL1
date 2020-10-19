#include "Grammar.h"

Grammar::Grammar() {
}

Grammar::~Grammar() {
}

Grammar::iterator::iterator(std::vector<std::pair<std::string, std::string> >::iterator it) {
	this->it = it;
}

Grammar::iterator::iterator(const iterator& iter) {
	this->it = iter.it;
}

Grammar::iterator::~iterator() {}

std::string& Grammar::iterator::getKey() const {
	return this->it->first;
}

std::string& Grammar::iterator::getValue() const {
	return this->it->second;
}

Grammar::iterator& Grammar::iterator::operator=(const iterator& iter) {
	this->it = iter.it;
	return *this;
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

Grammar::iterator Grammar::iterator::operator+(int n) {
	return Grammar::iterator(this->it + n);
}

Grammar::iterator Grammar::iterator::operator-(int n) {
	return Grammar::iterator(this->it - n);
}

Grammar::iterator& Grammar::iterator::operator+=(int n) {
	this->it += n;
	return *this;
}

Grammar::iterator& Grammar::iterator::operator-=(int n) {
	this->it -= n;
	return *this;
}

std::pair<std::string, std::string>& Grammar::iterator::operator*() {
	return *this->it;
}


Grammar::iterator Grammar::begin() {
	return iterator(this->data.begin());
}

Grammar::iterator Grammar::end() {
	return iterator(this->data.end());
}

int Grammar::size() const {
	return this->data.size();
}

void Grammar::insert(const std::string& S, const std::string& a) {
	this->data.push_back(std::pair<std::string, std::string>(S, a));
}
