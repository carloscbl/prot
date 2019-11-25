#include "Catch2/catch.hpp"
#include "json.hpp"
#include "form_runner.h"
#include "user.h"
#include "form.h"
#include "task_restrictions.h"

template<typename T_duration>
task_t get_collider(seconds offset_from_today, T_duration duration, string name = "test_probe"){
    task_t test_probe = make_shared<task>();
    test_probe->set_tag(name);
    test_probe->id = name;
    time_t next_day_collider_start = system_clock::to_time_t( floor<days>( system_clock::now()) + offset_from_today);
    time_t next_day_collider_end = next_day_collider_start + duration_cast<seconds>(duration).count() ;
    test_probe->set_interval( next_day_collider_start, next_day_collider_end);
    return test_probe;
}


TEST_CASE( "test form_runner", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    //carlos->get_tasker().print_out();
    REQUIRE( carlos->get_tasker().empty() == true );

    json qa_request1, qa_request2, qa_request3 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "5";
    
    const auto & form = form::get_register().at("Washer easer");


    REQUIRE( form->name == "Washer easer" );

    form_runner fr(carlos, *form);

    auto &response = fr.run(qa_request1);
    //cout << response.dump() <<endl;
    REQUIRE(response["next_question"] == "Do you have washer?");

    auto &response1 = fr.run(qa_request2);
    //cout << response1.dump() <<endl;
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
    REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") != nullptr);
    

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
    carlos->clear();
}

TEST_CASE( "test form_runner industrial", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    REQUIRE( carlos->get_tasker().empty() == true );

    task_t test_probe = make_shared<task>();
    test_probe->set_tag("test_probe");
    test_probe->id = "test_probe";
    time_t next_day_collider_start = system_clock::to_time_t( floor<days>( system_clock::now()) +days(1) + hours(6));
    time_t next_day_collider_end = next_day_collider_start + duration_cast<seconds>(hours(1)).count() ;
    test_probe->set_interval( next_day_collider_start, next_day_collider_end);
    carlos->get_scheduler().add_single(move(test_probe));

    json qa_request1, qa_request2, qa_request3 ,qa_request4 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "50";
    qa_request4["answer"] = "yes";
    
    const auto & form = form::get_register().at("Washer easer");


    REQUIRE( form->name == "Washer easer" );

    form_runner fr(carlos, *form);

    auto &response = fr.run(qa_request1);
    //cout << response.dump() <<endl;
    REQUIRE(response["next_question"] == "Do you have washer?");

    auto &response1 = fr.run(qa_request2);
    //cout << response1.dump() <<endl;
    REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

    auto &response2 = fr.run(qa_request3);
    //cout << response2.dump() <<endl;
    REQUIRE(response2["next_question"] == "Your washer is a industrial one?");
    auto &response3 = fr.run(qa_request4);
    REQUIRE(response3["next_question"] == "END");

    REQUIRE(carlos->get_tasker().find_task("industrial_start") != nullptr);
    REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
    time_point now = system_clock::now();
    const time_point day_start = floor<days>(now);

    time_point expected_industrial_start = day_start + find_restriction("night").value().to  + seconds(1); 
    time_point expected_industrial_middles = day_start + find_restriction("day").value().to  + seconds(1); 

    auto industrial_start = carlos->get_tasker().find_task("industrial_start");
    auto industrial_middle = carlos->get_tasker().find_task("industrial_middle");
    time_point expected_industrial_middlee = day_start + find_restriction("day").value().to  + seconds(1) + industrial_middle->get_duration().m_duration; 

    time_t start_start = system_clock::to_time_t(expected_industrial_start);
    time_t middle_start = system_clock::to_time_t(expected_industrial_middles);
    time_t middle_end = system_clock::to_time_t(expected_industrial_middlee);

    REQUIRE(start_start == industrial_start->get_interval().start);
    REQUIRE(middle_start == industrial_middle->get_interval().start);
    REQUIRE(middle_end == industrial_middle->get_interval().end);


    REQUIRE( carlos->get_tasker().empty() != true );
    carlos->clear();
}

TEST_CASE( "NEGATIVE test form_runner industrial", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    REQUIRE( carlos->get_tasker().empty() == true );

    carlos->get_scheduler().add_single(get_collider(days(1) + hours(2), hours(1)));
    //carlos->get_scheduler().print_out();
    json qa_request1, qa_request2, qa_request3 ,qa_request4 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "50";
    qa_request4["answer"] = "yes";
    
    const auto & form = form::get_register().at("Washer easer");


    REQUIRE( form->name == "Washer easer" );

    form_runner fr(carlos, *form);
    fr.clear_sessions();

    auto &response = fr.run(qa_request1);
    //cout << response.dump() <<endl;
    REQUIRE(response["next_question"] == "Do you have washer?");

    auto &response1 = fr.run(qa_request2);
    //cout << response1.dump() <<endl;
    REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

    auto &response2 = fr.run(qa_request3);
    //cout << response2.dump() <<endl;
    REQUIRE(response2["next_question"] == "Your washer is a industrial one?");
    auto &response3 = fr.run(qa_request4);
    REQUIRE(response3["next_question"] == "END");

    REQUIRE(carlos->get_tasker().find_task("industrial_start") != nullptr);
    REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
    time_point now = system_clock::now();
    const time_point day_start = floor<days>(now);

    time_point expected_industrial_start = day_start + find_restriction("night").value().to  + seconds(1); 
    time_point expected_industrial_middles = day_start + find_restriction("day").value().to  + seconds(1); 

    auto industrial_start = carlos->get_tasker().find_task("industrial_start");
    auto industrial_middle = carlos->get_tasker().find_task("industrial_middle");
    time_point expected_industrial_middlee = day_start + find_restriction("day").value().to  + seconds(1) + industrial_middle->get_duration().m_duration; 

    time_t start_start = system_clock::to_time_t(expected_industrial_start);
    time_t middle_start = system_clock::to_time_t(expected_industrial_middles);
    time_t middle_end = system_clock::to_time_t(expected_industrial_middlee);
    REQUIRE(start_start == industrial_start->get_interval().start);
    REQUIRE(middle_start != industrial_middle->get_interval().start);
    REQUIRE(middle_end != industrial_middle->get_interval().end);


    REQUIRE( carlos->get_tasker().empty() != true );
    carlos->clear();
}

TEST_CASE( "test form_runner task failure invalidation", "[runner]" ) {
  
    user::users["carlos"] = make_shared<user>(user_minimal_data{
        "carlos"
    });
    const auto & carlos_ = user::users["carlos"];
    REQUIRE( carlos_->get_name() == "carlos" );

    const auto & carlos = user::users["carlos"];
    //carlos->get_tasker().print_out();
    REQUIRE( carlos->get_tasker().empty() == true );

    //cout << "----" << endl;
    json qa_request1, qa_request2, qa_request3 ;
    qa_request2["answer"] = "yes";
    qa_request3["answer"] = "5";
    
    const auto & form = form::get_register().at("Washer easer");

    carlos->get_scheduler().add_single(get_collider(days(1) + hours(1), hours(22)));
    //carlos->get_scheduler().print_out();

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
    //cout << "---==__==__==__==__==---" << endl;
    //carlos->get_tasker().print_out();
    //carlos->get_scheduler().print_out();
    REQUIRE(carlos->get_tasker().find_task("base_task") == nullptr);
    REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
    auto washer_cleanup_task = carlos->get_tasker().find_task("washer_clean_up");

    REQUIRE(washer_cleanup_task == nullptr);
 
    auto size = carlos->get_scheduler().size();
    REQUIRE( size == 1);
    REQUIRE( carlos->get_tasker().size() == 0 );
    //cout << "FINAL SCHEDULER" << endl;
    //carlos->get_scheduler().print_out();

    carlos->clear();
}