#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> spt(const string& s, const string& seperator) {
	vector<string> ss;
	int p = 0;
	int cur = 0;
	while (p != s.npos) {
		p = s.find(seperator, p + 1);
		ss.push_back(s.substr(cur, p - cur));
		cur = p + seperator.size();
	}
	return ss;
}

int getInt() {
	string s;
	getline(cin, s);
	return atoi(s.c_str());
}