#include "Catch2/catch.hpp"
#include "form_runner.h"
#include "json.hpp"
#include "user.h"
#include "form.h"

TEST_CASE( "test form_runner", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos","123456"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    json qa_request1, qa_request2, qa_request3 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "5";
    // // auto answer = qa_request1.find("answer");
    // // if (!qa_request1.is_null() && answer != qa_request.end())
    // // {
    // //     //cout << qa_request["answer"] << endl;
    // // }
    auto aa = form::get_register().cbegin();

    REQUIRE( aa->first == "form1" );

    form_runner fr(carlos, *aa->second);

    auto &response = fr.run(qa_request1);

    //cout << response.dump(4) << endl;
    REQUIRE( 1 == 1 );
}