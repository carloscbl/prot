#include "app.h"
#include <iostream>
using namespace std;


app::~app()
{
    cout << "app destroyed" << endl;
}


app::app( const json & valid_app ): m_app(valid_app){
}