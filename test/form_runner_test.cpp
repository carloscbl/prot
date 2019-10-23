#include "Catch2/catch.hpp"
#include "form_runner.h"
#include "json.hpp"
#include "user.h"
#include "form.h"
#include "task_restrictions.h"

TEST_CASE( "test form_runner", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos","123456"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    carlos->get_tasker().print_out();
    REQUIRE( carlos->get_tasker().empty() == true );

    cout << "----" << endl;
    json qa_request1, qa_request2, qa_request3 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "5";
    
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

    REQUIRE(carlos->get_tasker().find_task("base_task") != nullptr);
    
    time_point now = system_clock::now();
    const time_point day_start = floor<days>(now);

    time_point expected_base_task_start = day_start + find_restriction("launch").value().to  + seconds(1); 
    time_point expected_washer_start_start = day_start + find_restriction("night").value().to  + seconds(1); 
    auto washer_start_task = carlos->get_tasker().find_task("washer_start");
    auto washer_end_task = carlos->get_tasker().find_task("washer_end");
    auto washer_cleanup_task = carlos->get_tasker().find_task("washer_clean_up");

    time_t expected_washer_end_start_min = washer_start_task->get_interval().end + seconds(1).count() + washer_end_task->get_when().minimum_delay.m_duration.count(); 

    time_t expected_washer_end_start_max = washer_start_task->get_interval().end + seconds(1).count()  + washer_end_task->get_when().maximum_delay.m_duration.count(); 

    time_t expected_washer_cleanup_start_min = washer_end_task->get_interval().end + seconds(1).count() + washer_cleanup_task->get_when().minimum_delay.m_duration.count(); 
    time_t expected_washer_cleanup_start_max = washer_end_task->get_interval().end + seconds(1).count()  + washer_cleanup_task->get_when().maximum_delay.m_duration.count(); 

    time_t base_task_start = carlos->get_tasker().find_task("base_task")->get_interval().start;
    time_t washer_start_start = carlos->get_tasker().find_task("washer_start")->get_interval().start;
    time_t washer_end_start = carlos->get_tasker().find_task("washer_end")->get_interval().start;
    time_t washer_cleanup_start = carlos->get_tasker().find_task("washer_clean_up")->get_interval().start;

    time_t expected = system_clock::to_time_t(expected_base_task_start);
    time_t expected_2 = system_clock::to_time_t(expected_washer_start_start);
    //cout << "Test:" << endl <<ctime(&base_task_start) << ctime(&expected);

    REQUIRE( base_task_start == expected );

    REQUIRE( washer_start_start == expected_2 );

    REQUIRE( washer_end_start >= expected_washer_end_start_min );
    REQUIRE( washer_end_start <= expected_washer_end_start_max );

    REQUIRE( washer_cleanup_start >= expected_washer_cleanup_start_min );
    REQUIRE( washer_cleanup_start <= expected_washer_cleanup_start_max );

    REQUIRE( carlos->get_tasker().empty() != true );
    REQUIRE( 1 == 1 );
    carlos->clear();
}


TEST_CASE( "test form_runner industrial", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos","123456"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    REQUIRE( carlos->get_tasker().empty() == true );

    json qa_request1, qa_request2, qa_request3 ,qa_request4 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "50";
    qa_request4["answer"] = "yes";
    
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
    REQUIRE(response2["next_question"] == "Your washer is a industrial one?");
    auto &response3 = fr.run(qa_request4);
    REQUIRE(response3["next_question"] == "END");

    REQUIRE(carlos->get_tasker().find_task("industrial_start") != nullptr);
    
    time_point now = system_clock::now();
    const time_point day_start = floor<days>(now);

    time_point expected_industrial_start = day_start + find_restriction("night").value().to  + seconds(1); 
    time_point expected_base_task_start = day_start + find_restriction("launch").value().to  + seconds(1); 
    auto industrial_start = carlos->get_tasker().find_task("industrial_start");
    auto industrial_middle = carlos->get_tasker().find_task("industrial_middle");
    time_t start_start = system_clock::to_time_t(expected_industrial_start);

    REQUIRE(start_start == industrial_start->get_interval().start);


    REQUIRE( 1 == 1 );
    REQUIRE( carlos->get_tasker().empty() != true );
    carlos->clear();
}