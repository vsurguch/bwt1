//
//  main.cpp
//  BWT
//
//  Created by Vladimir Surguch on 31/03/2018.
//  Copyright © 2018 Vladimir Surguch. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include <cstdlib>

using std::vector;
using std::string;
using std::pair;
using std::map;

struct FirstLast {
    char first;
    char last;
};

struct FirstLast2{
    char first;
    char last;
    int rel_i_last;
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
    
    string result;
    result.resize(text.length(), ' ');
    
    for (int i = 0; i < v.size(); ++i) {
        result[i] = v[i].second.last;
    }
    
    return result;
}

int get_preceeding(vector<FirstLast2> &v, map<char, int> &starting_positions, int pos){
    int result = -1;
    
    char ch_in_bwt = v[pos].last;
    int rel_i_last = v[pos].rel_i_last;
    
    if (starting_positions.find(ch_in_bwt) != starting_positions.end()){
        result = starting_positions.find(ch_in_bwt)->second + rel_i_last;
    }
    
    return result;
}

string from_bwt(string &text) {
    
    // building main array
    vector<FirstLast2> v (text.length(), {'0', '0', 0});
    
    string text_sorted = text;
    
    std::sort(text_sorted.begin(), text_sorted.end(), [](char const ch1, char const ch2){return ch1 < ch2; });
    
    for (int i = 0; i < text.length(); ++i){
        v[i] = {text_sorted[i], text[i], 0};
    }
    
    // counting starting positions in sorted bwt
    map<char, int> starting_positions;
    starting_positions.insert({v[0].first, 0});
    
    for (int i = 1; i < v.size(); ++i){
        if (v[i].first != v[i-1].first){
            starting_positions.insert({v[i].first, i});
        }
    }
    
    //counting posiotions in bwt
    map<char, int> last_column_counter;

    for (int i = 0; i < v.size(); ++i){
        char ch = v[i].last;
        if (last_column_counter.find(ch) == last_column_counter.end()){
            last_column_counter.insert({ch, 0});
            v[i].rel_i_last = 0;
        }
        else {
            auto it = last_column_counter.find(ch);
            int counter = it->second;
            counter++;
            it->second = counter;
            v[i].rel_i_last = counter;
        }
    }
//    // output main aryray
//    for (int i = 0; i < v.size(); ++i){
//        std::cout << v[i].first << ": " << v[i].last << v[i].rel_i_last << std::endl;
//    }
//    // output starting positions
//    for (auto it = starting_positions.begin(); it != starting_positions.end(); ++it){
//        std::cout << it->first << " " << it->second << std::endl;
//    }
    
    //building result
    string result;
    result.resize(text.length(), ' ');
    int pos = 0;
    
    for (int i = result.length() -1; i >= 0; --i){
        result[i] = v[pos].first;
        pos = get_preceeding(v, starting_positions, pos);
    }
    
    return result;
}


void stress_testing(){
    vector<Record> v;
    vector<char> nucleotides;
    nucleotides.push_back('A');
    nucleotides.push_back('T');
    nucleotides.push_back('G');
    nucleotides.push_back('V');
    
    for (int n = 0; n < 10; ++n){
        int l = rand() % 50000;
        std::cout << l << std::endl;
        string str;
        str.resize(l+1, '$');
        v.clear();
        for (int i = 0; i < l; ++i){
            str[i] = nucleotides[rand() % 4];
        }
        string bwt = to_bwt(v, str);
        string reconstructed = from_bwt(bwt);
        
        bool comp = str.compare(reconstructed);
        
        if (comp) std::cout << "mismatch" << std::endl;
        else std::cout << "match" << std::endl;
//        std::cout << str << std::endl << reconstructed << std::endl << comp << std::endl;
    }
    
}

int main(int argc, const char * argv[]) {
//    vector<Record> v1;
//    vector<Record> v2;
//    string text("theraininspainstaysmainlyintheplain$");
//    string bwt = to_bwt(v1, text);
//    std::cout << bwt << std::endl;
//    
//    string text_reconstructed = from_bwt(bwt);
//    
//    std::cout << text_reconstructed << std::endl;
    
    stress_testing();
    std::cout << " Program ended." << std::endl;
    
    return 0;
}
