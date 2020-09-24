#include "app.h"
#include <iostream>
using namespace std;


app::~app()
{
    cout << "app destroyed" << endl;
}


app::app( const json & valid_app ): m_app(valid_app){
    apps[get_app_name()] = make_unique<app>(*this);
}