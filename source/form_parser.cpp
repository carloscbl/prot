#include "form_parser.h"

form_parser::form_parser(const json & j):j(j){

    subsections = map<string,unique_ptr<form_subsection_ADT>>();
    subsections["form"] = nullptr;
    subsections["expressions"] = nullptr;
    subsections["variables"] = nullptr;
    subsections["configurables"] = nullptr;
    subsections["questions"] = nullptr;
}