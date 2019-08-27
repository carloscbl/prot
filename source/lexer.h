#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <cassert>

#include "json.hpp"

using std::string;
using std::cout;
using std::endl;
using nlohmann::json;
using std::multimap;
using std::map;
using mm_t =std::multimap<string,string>;

class lexer
{
private:
    json j_lexer;

public:
    lexer(const string lexer_path = "../lexer.json"){
        std::ifstream i(lexer_path);
        i >> j_lexer;
        cout << j_lexer.dump(4) << endl;
        assert(!j_lexer.empty());
    };
    const json & get_lex() const{ return j_lexer; }
    mm_t get_section(const string & section) const {
        mm_t lex_m;
        for ( auto & [k,v] : j_lexer[section].items()){ //for task, desecription, name etc
            for ( auto & selector : v){//list of selectors
                lex_m.insert(make_pair(k,selector.get<string>()));
            }
        }
        assert(!lex_m.empty());
        return lex_m;
    }
};

const inline static lexer lex;

using mss_t = map<string,string>;

class generic_lex{
    const lexer & m_lex;
    const mm_t m_map;
    mss_t reverse_map;


    void build_reverse_map(){
        for (auto & [k,v] : m_map)
        {
            auto it =  m_map.equal_range(k);
            for (auto itr = it.first; itr != it.second; ++itr) { 
                reverse_map[itr->second] = itr->first;
            }
        } 
    }
public:
    generic_lex(const string & section):m_lex(lex),m_map(lex.get_section(section)){ build_reverse_map(); }

    string get_selector(const string & key){
        const auto & match = m_map.find(key);
        if(match != m_map.end()){
            return match->second;
        }else{
            return string();
        }
    }

    string get_field_by(const string & selector){
        const auto & match = reverse_map.find(selector);
        if(match != reverse_map.end()){
            return match->second;
        }else
        {
            return string();
        }
    }

    void print_out(){
        for (const auto & [k,v] : reverse_map)
        {
            cout << k << " : " << v << endl;
        }
        
    }
};

class task_lex : public generic_lex{

public:
    task_lex():generic_lex("task"){}

};

#endif //LEXER_H