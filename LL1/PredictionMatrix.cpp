#include "PredictionMatrix.h"

PredictionMatrix::PredictionMatrix() {
}

PredictionMatrix::PredictionMatrix(const PredictionMatrix& copy) {
	this->data = copy.data;
}

PredictionMatrix::~PredictionMatrix() {
}

bool PredictionMatrix::has(const std::string& S, const std::string& a) {
	if (this->data.find(S) == this->data.end()) {
		return false;
	}
	if (this->data[S].find(a) == this->data[S].end()) {
		return false;
	}
	return true;
}

std::string PredictionMatrix::get(const std::string& S, const std::string& a) {
	if (!this->has(S, a)) {
		return "";
	}
	return this->data[S][a];
}

void PredictionMatrix::add(const std::string& S, const std::string& a, const std::string& exp) {
	this->data[S][a] = exp;
}

std::ostream& operator<<(std::ostream& out, PredictionMatrix& p) {
	std::map<std::string, int> len;
	for (auto i : p.data) {
		int& oldLen = len[i.first];
		int nowLen = i.second.size();
		oldLen = nowLen ? nowLen > oldLen : oldLen;
	}
	out << "©°";
	for (auto col : len) {
		// for (int i = 0; i < col.second + 2; i++)
	}
	return out;
}