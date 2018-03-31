

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>

using std::vector;
using std::string;
using std::pair;

struct FirstLast {
	char first;
	char last;
};

typedef pair<int, FirstLast> Record;

struct Wheeler {
	string str_;
	int len_;
	explicit Wheeler(string &s) : str_(s), len_(s.length()) {}

	int shift(int start, int shift) {
		return ((start + shift) < len_) ? (start + shift) : ((start + shift) % len_);
	}

	string get_str_from(int pos) {
		string result = str_.substr(pos) + str_.substr(0, pos);
		return result;
	}

	bool compare(int pos1, int pos2) {
		bool result = false;
		for (int i = 0; i < len_; ++i) {
			int real_pos1 = shift(pos1, i);
			int real_pos2 = shift(pos2, i);
			if (str_[real_pos1] != str_[real_pos2]) {
				result = str_[real_pos1] < str_[real_pos2];
				break;
			}
		}
		return result;
	}

	FirstLast get_fl(int pos) {
		int last_pos = shift(pos, len_ - 1);
		return { str_[pos], str_[last_pos] };
	}

};


struct comparator {
	bool operator()(FirstLast &fl1, FirstLast &fl2) { return fl1.first < fl2.first; }
};

string to_bwt(vector<Record> &v, string &text) {
	
	Wheeler wheeler(text);
	for (int i = 0; i < text.length(); ++i) {
		v.push_back({ i, wheeler.get_fl(i) });
	}
	auto comp = [&wheeler](Record const &rec1, Record const &rec2) { return wheeler.compare(rec1.first, rec2.first); };

	std::sort(v.begin(), v.end(), comp);
	
	/*comparator comp;
	auto comp2 = [](FirstLast &fl1, FirstLast &fl2) {return fl1.first < fl2.first; };
	std::sort(v.begin(), v.end(), comp);*/
    
	string result (" ", text.length());

	for (int i = 0; i < v.size(); ++i) {
		result[i] = v[i].second.last;
	}

	return result;
}

int main(int argc, char **argv) {
	
	vector<Record> v;
	string s("hello$");
	/*Wheeler wheeler(s);
	for (int i = 0; i < s.length(); ++i) {
		v.push_back({ i, wheeler.get_fl(i) });
	}
	auto comp = [&wheeler](Record const &rec1, Record const &rec2) { return wheeler.compare(rec1.first, rec2.first); };
	
	std::sort(v.begin(), v.end(), comp);
*/
	std::cout << to_bwt(v, s);
	    
	return 0;
}