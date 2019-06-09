#include "form_parser.h"


form_parser::form_parser(const json & j):j(j){

    subsections = map<string,unique_ptr<form_subsection_ADT>>();
    for(const auto& sect : subsection_names){
        subsections[sect] = make_unique<form_subsection_ADT>(j,sect);
    }

    for(const auto & [k,v] : j.items()){
        const auto & it = find(subsection_names.begin(),subsection_names.end(),k);
        if(it != subsection_names.end()){
            continue;
        }
        discover[k] = make_unique<form_subsection_ADT>(j,k);
    }
    form_traverse();
    //answer_branches<string> branches(j,"Lemonade");
}