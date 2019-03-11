//
// Created by carlos on 2/03/19.
//

#ifndef FORM_TRAVERSER_H
#define FORM_TRAVERSER_H

#include <map>

using namespace std;
class question_node;
class form_traverser
{
private:
    const map<int id,const question_node *> qmap;
public:
    form_traverser());

    void run();
};


#endif //FORM_TRAVERSER_H
