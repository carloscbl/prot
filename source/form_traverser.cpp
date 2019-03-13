#include "form_traverser.h"



form_traverser::form_traverser(map<question_node_id, unique_ptr<question_node>> && questions):qmap(move(questions))
{

}

void form_traverser::run(){

}