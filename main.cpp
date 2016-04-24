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

void print(vpu& v)
{
    string s;
    for (size_t i = 0; i < v.size(); ++i) {
        for(auto& j:v[i].first) {
            s+=j.first + " - ";
        }
        s.pop_back();
        s.pop_back();
        cout << s << "--> " << v[i].second << endl;
        s="";
    }
    cout << endl;
}


void candid_gen(vpu& v, vpu& ans)
{
    size_t count=v[0].first.size() + 1;
    u m,mm;
    for (size_t i = 0; i < v.size() - 1; ++i) {
        mm = v[i].first;

        for (size_t j = i + 1; j < v.size(); j++) {
            m.clear();
            m=mm;
            for(auto& h:v[j].first) {
                m[h.first]++;
            }

            if(m.size() == count){
                size_t l=0,p=0;
                for (size_t var = 0; var < ans.size(); ++var) {
                    for(auto& w:m) {
                        if(ans[var].first.count(w.first))
                            l++;
                    }
                    if(l==m.size())
                        p++;
                    l=0;
                }
                if(p==0)
                    ans.push_back(make_pair(m, 0));
//                print(ans);
                p=0;
            }
        }
    }
}


void sup_cal(vpu& v, vu& iv, size_t minsup, vpu& ans)
{
    u exist;
    string s="";
    size_t c, cc, ccc;
    for (size_t i = 0; i < v.size(); ++i) {
        c=0; cc=0; ccc = v[i].first.size();
        for (size_t j = 0; j < iv.size(); ++j) {
            s="";
            for(auto& k:v[i].first) {
                s+=";"+k.first;
                if(iv[j].count(k.first)){
                    c++;
                }
            }
            if(c==ccc) {
                cc++;
            }
            c=0;
        }
        if(cc >= minsup) {
            if(!exist.count(s))
                ans.push_back(make_pair(v[i].first, cc));
//            print(ans);
            exist[s]++;
            s="";
        }
    }
}



int main()
{
    string location; // = "/home/siavash/Desktop/a.txt";
    size_t minsup = 2;
    cout <<  "Data File Location : ";
    cin >> location;
    cout << "Minsup : ";
    cin >> minsup;

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

    vector<string> temp;
    for(auto& i:candidate) {
        if((size_t)i.second >= minsup)
            temp.push_back(i.first);
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
    sup_cal(candidate2, iv, minsup, t_candid);

    candidate2.clear();
    candidate2 = t_candid;

    while(true) {
        t_candid.clear();
        a_candid.clear();
        candid_gen(candidate2, t_candid);

        if(t_candid.size()==0) break;
        sup_cal(t_candid, iv, minsup, a_candid);

        if(a_candid.size() != 0)
            candidate2 = a_candid;
        else
            break;
    }

    cout << endl << "Answer : " << endl;
    print(candidate2);

    return 0;
}
