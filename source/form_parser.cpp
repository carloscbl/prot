#include "form_parser.h"

form_parser::form_parser(const json & j):j(j){

    subsections = map<string,unique_ptr<form_subsection_ADT>>();
    subsections["form"] = make_unique<form_subsection_ADT>(j,"form");
    subsections["expressions"] = make_unique<form_subsection_ADT>(j,"expressions");
    subsections["variables"] = make_unique<form_subsection_ADT>(j,"variables");
    subsections["configurables"] = make_unique<form_subsection_ADT>(j,"configurables");
    subsections["questions"] = make_unique<form_subsection_ADT>(j,"questions");
}