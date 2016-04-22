#include <map>
#include <regex>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
using namespace std;
using namespace boost;

typedef unordered_map<string, int> u;
typedef vector<unordered_map<string, int>> vu;
typedef vector<pair<unordered_map<string, int>, size_t>> vpu;

void candid_gen(vpu& v, vpu& ans)
{
    size_t count=v[0].first.size() + 1;
    u m,mm;
    for (size_t i = 0; i < v.size() - 1; ++i) {
        for(auto& k:v[i].first)
            mm[k.first]++;
        for (size_t j = i + 1; j < v.size(); j++) {
            m.clear();
            m=mm;
            for(auto& h:v[j].first)
                m[h.first]++;
            if(m.size() == count)
                ans.push_back(make_pair(m, 0));
        }
    }
}


void sup_cal(vpu& v, vu& iv, size_t minsup, vpu& ans)
{
    size_t c, cc, ccc;
    for (size_t i = 0; i < v.size(); ++i) {
        c=0; cc=0; ccc = v[i].first.size();
        for (size_t j = 0; j < iv.size(); ++j) {
            for(auto& k:v[i].first)
                if(iv[j].count(k.first))
                    c++;
            if(c==ccc)
                cc++;
            c=0;
        }
        if(cc >= minsup)
            ans.push_back(make_pair(v[i].first, cc));
    }
}


int main()
{
    string location = "/home/siavash/Desktop/a.txt";
    size_t minsup = 2;
//    cin >> location;
//    cin >> minsup;
//    cin >> minconf;

    //read from file
    ifstream f(location.c_str());
    string d((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());

    // iv -> final vector
    vu iv;

    // fm -> all items  and supports
    u candidate;

    // dv -> contain lines of data: TID & Itemset
    vector<string> dv;
    split(dv,d,is_any_of("\n"));

    for(size_t  i=1; i<dv.size(); i++)
    {
        dv[i] = regex_replace(dv[i] , regex("\\s+"), "");
        u m;
        vector<string> v, vv;

        split(v,dv[i], is_any_of(","));
        split(vv, v[1], is_any_of(";"));
        for(size_t j=0; j<vv.size(); j++) {
            m[vv[j]]++;
            candidate[vv[j]]++;
        }
        iv.push_back(m);
    }

    for (size_t i = 0; i < iv.size(); i++) {
        for (auto& k:iv[i]) {
            cout << k.first << " ";
        }
        cout << endl;
    }


    vector<string> temp;
    for(auto& i:candidate) {
        cout<< i.first << "--" << i.second << endl;
        if((size_t)i.second >= minsup)
            temp.push_back(i.first);
    }

    for(auto& i:temp) {
        cout << i << endl;
    }

    vpu candidate2;
    u m;
    for(size_t i=0; i<temp.size()-1; i++) {
        for(size_t j=i+1; j<temp.size(); j++) {
            m[temp[i]]++;
            m[temp[j]]++;
            candidate2.push_back(make_pair(m, 0));
            m.clear();
        }    
    }


    vpu t_candid, a_candid;
//    for (size_t i = 0; i < candidate2.size(); i++) {
//        for (auto& k:candidate2[i].first) {
//            cout << k.first << " ";
//        }
//        cout << endl;
//    }
    sup_cal(candidate2, iv, minsup, t_candid);

//    for (size_t i = 0; i < t_candid.size(); i++) {
//        for (auto& k:t_candid[i].first) {
//            cout << k.first << " ";
//        }
//        cout << endl;
//    }



    while(true) {
        t_candid.clear();
        a_candid.clear();
        candid_gen(candidate2, t_candid);
        if(t_candid.size()==0) break;
        sup_cal(t_candid, iv, minsup, a_candid);
        candidate2 = a_candid;
    }

    for (size_t i = 0; i < candidate2.size(); ++i) {
        for(auto& j:candidate2[i].first) {
            cout << j.first << " - ";
        }
        cout << "-->" << candidate2[i].second << endl;
    }

    return 0;
}
