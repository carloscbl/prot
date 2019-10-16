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
    const auto & form = form::get_register().at("Washer easer");


    REQUIRE( form->name == "Washer easer" );

    form_runner fr(carlos, *form);

    auto &response = fr.run(qa_request1);
    // cout << response.dump() <<endl;
    REQUIRE(response["next_question"] == "Do you have washer?");

    auto &response1 = fr.run(qa_request2);
    // cout << response1.dump() <<endl;
    REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

    auto &response2 = fr.run(qa_request3);
    //cout << response2.dump() <<endl;
    REQUIRE(response2["next_question"] == "END");

    REQUIRE( 1 == 1 );
}