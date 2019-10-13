#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "Catch2/catch.hpp"
#include "user.h"
#include "form_runner.h"

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    
      
    // const auto & carlos = user::users["carlos"];
    // json qa_request1, qa_request2, qa_request3 ;
    // qa_request2["answer"] = "yes";
    // qa_request3["answer"] = "5";
    // // auto answer = qa_request1.find("answer");
    // // if (!qa_request1.is_null() && answer != qa_request.end())
    // // {
    // //     //cout << qa_request["answer"] << endl;
    // // }
    // auto aa = form::get_register().cbegin();

    // form_runner fr(carlos, *aa->second);

    // auto &response = fr.run(qa_request1);

    // //cout << response.dump(4) << endl;
    REQUIRE( 1 == 1 );
}