#include "Catch2/catch.hpp"
#include "json.hpp"
#include "user.h"
#include "app.h"
#include "task_restrictions.h"
#include "cloud_app_runner.h"

#include <fstream>
#include <iostream>
#include "json.hpp"
// #include "time_utils.hpp"
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
using namespace fs;


template<typename T_duration>
task_t get_collider(seconds offset_from_today, T_duration duration, string name = "test_probe"){
    task_t test_probe = make_shared<task>();
    test_probe->set_tag(name);
    test_probe->set_id( 9999);
    time_t next_day_collider_start = system_clock::to_time_t( floor<days>( system_clock::now()) + offset_from_today);
    time_t next_day_collider_end = next_day_collider_start + duration_cast<seconds>(duration).count() ;
    test_probe->set_interval( next_day_collider_start, next_day_collider_end);
    return test_probe;
}


TEST_CASE( "test cloud_app_runner workout", "[runner]" ) {
    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::cout << "Current path is : " << full_path << std::endl;
    std::ifstream matrix_input_file("../test/iofiles_test/test_matrix_input_example.json", std::fstream::in);
    std::ifstream test_app_json_file("../test/iofiles_test/test_app_dailyworkout.json", std::fstream::in);
    json matrix_input;
    json test_app_json;
    matrix_input_file >> matrix_input;
    test_app_json_file >> test_app_json;
    user::users["carloscbl"] = make_shared<user>(user_minimal_data{
        "carloscbl"
    });
    const auto & carlos = user::users["carloscbl"];
    REQUIRE( carlos->get_name() == "carloscbl" );
    app _(test_app_json);
    const auto & app = app::get_apps_register().at("test Daily workout");
    cloud_app_runner fr(*carlos, *app, 8);
    auto question_test = test_app_json["questions"].at(2);
    next_question_data_and_taskstory_input nqdati;
    nqdati.next_question_text="bla bla";
    nqdati.taskstory_name="normal_flow";
    nqdati.raw_taskstory=question_test["taskstories"]["normal_flow"];
    nqdati.user_input=matrix_input;
    nqdati.current_question_obj=question_test;

    expand_taskstory_t et (nqdati);

    CHECK(et.expand_and_set()); // check expansion!
    fr.m_session_id = 8;
    fr.schedule_taskstory(nqdati);
    auto & tkr = carlos->get_tasker();
    auto & sche = carlos->get_scheduler();
    sche.print_out();
    CHECK(sche.size() == 7);
    // tkr.print_out();
    // CHECK(tkr.size() == 7); //wont work correctly with out db
    int a = 20;

    
}


// TEST_CASE( "test cloud_app_runner", "[runner]" ) {
  
//     user::users["carloscbl"] = make_shared<user>(user_minimal_data{
//         "carloscbl"
//     });
//     const auto & carlos_ = user::users["carloscbl"];
//     REQUIRE( carlos_->get_name() == "carloscbl" );

//     const auto & carlos = user::users["carloscbl"];
//     //carlos->get_tasker().print_out();
//     REQUIRE( carlos->get_tasker().empty() == true );

//     json qa_request1, qa_request2, qa_request3 ;
//     qa_request2["answer"] = "yes";
//     qa_request3["answer"] = "5";
    
//     const auto & app = app::get_apps_register().at("Washer easer");


//     REQUIRE( app->get_app_name() == "Washer easer" );

//     cloud_app_runner fr(*carlos, *app);

//     auto &response = fr.run(qa_request1);
//     //cout << response.dump() <<endl;
//     REQUIRE(response["next_question"] == "Do you have washer?");

//     auto &response1 = fr.run(qa_request2);
//     //cout << response1.dump() <<endl;
//     REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

//     auto &response2 = fr.run(qa_request3);
//     //cout << response2.dump() <<endl;
//     REQUIRE(response2["next_question"] == "END");

//     REQUIRE(carlos->get_tasker().find_task("base_task") != nullptr);

//     time_point now = system_clock::now();
//     const time_point day_start = floor<days>(now);

//     time_point expected_base_task_start = day_start + find_restriction("launch").value().to  + seconds(1); 
//     time_point expected_washer_start_start = day_start + find_restriction("night").value().to  + seconds(1); 
//     auto washer_start_task = carlos->get_tasker().find_task("washer_start");
//     auto washer_end_task = carlos->get_tasker().find_task("washer_end");
//     auto washer_cleanup_task = carlos->get_tasker().find_task("washer_clean_up");
//     REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") != nullptr);
    

//     time_t expected_washer_end_start_min = washer_start_task->get_interval().end + seconds(1).count() + washer_end_task->get_when().minimum_delay.m_duration.count(); 

//     time_t expected_washer_end_start_max = washer_start_task->get_interval().end + seconds(1).count()  + washer_end_task->get_when().maximum_delay.m_duration.count(); 

//     time_t expected_washer_cleanup_start_min = washer_end_task->get_interval().end + seconds(1).count() + washer_cleanup_task->get_when().minimum_delay.m_duration.count(); 
//     time_t expected_washer_cleanup_start_max = washer_end_task->get_interval().end + seconds(1).count()  + washer_cleanup_task->get_when().maximum_delay.m_duration.count(); 

//     time_t base_task_start = carlos->get_tasker().find_task("base_task")->get_interval().start;
//     time_t washer_start_start = carlos->get_tasker().find_task("washer_start")->get_interval().start;
//     time_t washer_end_start = carlos->get_tasker().find_task("washer_end")->get_interval().start;
//     time_t washer_cleanup_start = carlos->get_tasker().find_task("washer_clean_up")->get_interval().start;
    

//     time_t expected = system_clock::to_time_t(expected_base_task_start);
//     time_t expected_2 = system_clock::to_time_t(expected_washer_start_start);
//     //cout << "Test:" << endl <<ctime(&base_task_start) << ctime(&expected);

//     REQUIRE( base_task_start == expected );

//     REQUIRE( washer_start_start == expected_2 );

//     REQUIRE( washer_end_start >= expected_washer_end_start_min );
//     REQUIRE( washer_end_start <= expected_washer_end_start_max );

//     REQUIRE( washer_cleanup_start >= expected_washer_cleanup_start_min );
//     REQUIRE( washer_cleanup_start <= expected_washer_cleanup_start_max );

//     REQUIRE( carlos->get_tasker().empty() != true );
//     carlos->clear();
// }

// TEST_CASE( "test cloud_app_runner industrial", "[runner]" ) {
  
//     user::users["carloscbl"] = make_shared<user>(user_minimal_data{
//         "carloscbl"
//     });
//     const auto & carlos_ = user::users["carloscbl"];
//     REQUIRE( carlos_->get_name() == "carloscbl" );

//     const auto & carlos = user::users["carloscbl"];
//     REQUIRE( carlos->get_tasker().empty() == true );

//     task_t test_probe = make_shared<task>();
//     test_probe->set_tag("test_probe");
//     test_probe->set_id(9999);
//     time_t next_day_collider_start = system_clock::to_time_t( floor<days>( system_clock::now()) +days(1) + hours(6));
//     time_t next_day_collider_end = next_day_collider_start + duration_cast<seconds>(hours(1)).count() ;
//     test_probe->set_interval( next_day_collider_start, next_day_collider_end);
//     carlos->get_scheduler().add_single(move(test_probe));

//     json qa_request1, qa_request2, qa_request3 ,qa_request4 ;
//     qa_request2["answer"] = "yes";
//     qa_request3["answer"] = "50";
//     qa_request4["answer"] = "yes";
    
//     const auto & app = app::get_apps_register().at("Washer easer");


//     REQUIRE( app->get_app_name() == "Washer easer" );

//     cloud_app_runner fr(*carlos, *app);

//     auto &response = fr.run(qa_request1);
//     //cout << response.dump() <<endl;
//     REQUIRE(response["next_question"] == "Do you have washer?");

//     auto &response1 = fr.run(qa_request2);
//     //cout << response1.dump() <<endl;
//     REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

//     auto &response2 = fr.run(qa_request3);
//     //cout << response2.dump() <<endl;
//     REQUIRE(response2["next_question"] == "Your washer is a industrial one?");
//     auto &response3 = fr.run(qa_request4);
//     REQUIRE(response3["next_question"] == "END");

//     REQUIRE(carlos->get_tasker().find_task("industrial_start") != nullptr);
//     REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
//     time_point now = system_clock::now();
//     const time_point day_start = floor<days>(now);

//     time_point expected_industrial_start = day_start + find_restriction("night").value().to  + seconds(1); 
//     time_point expected_industrial_middles = day_start + find_restriction("day").value().to  + seconds(1); 

//     auto industrial_start = carlos->get_tasker().find_task("industrial_start");
//     auto industrial_middle = carlos->get_tasker().find_task("industrial_middle");
//     time_point expected_industrial_middlee = day_start + find_restriction("day").value().to  + seconds(1) + industrial_middle->get_duration().m_duration; 

//     time_t start_start = system_clock::to_time_t(expected_industrial_start);
//     time_t middle_start = system_clock::to_time_t(expected_industrial_middles);
//     time_t middle_end = system_clock::to_time_t(expected_industrial_middlee);

//     REQUIRE(start_start == industrial_start->get_interval().start);
//     REQUIRE(middle_start == industrial_middle->get_interval().start);
//     REQUIRE(middle_end == industrial_middle->get_interval().end);


//     REQUIRE( carlos->get_tasker().empty() != true );
//     carlos->clear();
// }

// TEST_CASE( "NEGATIVE test cloud_app_runner industrial", "[runner]" ) {
  
//     user::users["carloscbl"] = make_shared<user>(user_minimal_data{
//         "carloscbl"
//     });
//     const auto & carlos_ = user::users["carloscbl"];
//     REQUIRE( carlos_->get_name() == "carloscbl" );

//     const auto & carlos = user::users["carloscbl"];
//     REQUIRE( carlos->get_tasker().empty() == true );

//     carlos->get_scheduler().add_single(get_collider(days(1) + hours(2), hours(1)));
//     //carlos->get_scheduler().print_out();
//     json qa_request1, qa_request2, qa_request3 ,qa_request4 ;
//     qa_request2["answer"] = "yes";
//     qa_request3["answer"] = "50";
//     qa_request4["answer"] = "yes";
    
//     const auto & app = app::get_apps_register().at("Washer easer");


//     REQUIRE( app->get_app_name() == "Washer easer" );

//     cloud_app_runner fr(*carlos, *app);
//     fr.clear_sessions();

//     auto &response = fr.run(qa_request1);
//     //cout << response.dump() <<endl;
//     REQUIRE(response["next_question"] == "Do you have washer?");

//     auto &response1 = fr.run(qa_request2);
//     //cout << response1.dump() <<endl;
//     REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

//     auto &response2 = fr.run(qa_request3);
//     //cout << response2.dump() <<endl;
//     REQUIRE(response2["next_question"] == "Your washer is a industrial one?");
//     auto &response3 = fr.run(qa_request4);
//     REQUIRE(response3["next_question"] == "END");

//     REQUIRE(carlos->get_tasker().find_task("industrial_start") != nullptr);
//     REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
//     time_point now = system_clock::now();
//     const time_point day_start = floor<days>(now);

//     time_point expected_industrial_start = day_start + find_restriction("night").value().to  + seconds(1); 
//     time_point expected_industrial_middles = day_start + find_restriction("day").value().to  + seconds(1); 

//     auto industrial_start = carlos->get_tasker().find_task("industrial_start");
//     auto industrial_middle = carlos->get_tasker().find_task("industrial_middle");
//     time_point expected_industrial_middlee = day_start + find_restriction("day").value().to  + seconds(1) + industrial_middle->get_duration().m_duration; 

//     time_t start_start = system_clock::to_time_t(expected_industrial_start);
//     time_t middle_start = system_clock::to_time_t(expected_industrial_middles);
//     time_t middle_end = system_clock::to_time_t(expected_industrial_middlee);
//     REQUIRE(start_start == industrial_start->get_interval().start);
//     REQUIRE(middle_start != industrial_middle->get_interval().start);
//     REQUIRE(middle_end != industrial_middle->get_interval().end);


//     REQUIRE( carlos->get_tasker().empty() != true );
//     carlos->clear();
// }

// TEST_CASE( "test cloud_app_runner task failure invalidation", "[runner]" ) {
  
//     user::users["carloscbl"] = make_shared<user>(user_minimal_data{
//         "carloscbl"
//     });
//     const auto & carlos_ = user::users["carloscbl"];
//     REQUIRE( carlos_->get_name() == "carloscbl" );

//     const auto & carlos = user::users["carloscbl"];
//     //carlos->get_tasker().print_out();
//     REQUIRE( carlos->get_tasker().empty() == true );

//     //cout << "----" << endl;
//     json qa_request1, qa_request2, qa_request3 ;
//     qa_request2["answer"] = "yes";
//     qa_request3["answer"] = "5";
    
//     const auto & app = app::get_apps_register().at("Washer easer");

//     carlos->get_scheduler().add_single(get_collider(days(1) + hours(1), hours(22)));
//     //carlos->get_scheduler().print_out();

//     REQUIRE( app->get_app_name() == "Washer easer" );

//     cloud_app_runner fr(*carlos, *app);

//     auto &response = fr.run(qa_request1);
//     // cout << response.dump() <<endl;
//     REQUIRE(response["next_question"] == "Do you have washer?");

//     auto &response1 = fr.run(qa_request2);
//     // cout << response1.dump() <<endl;
//     REQUIRE(response1["next_question"] == "How many Kilograms have your washer capacity?");

//     auto &response2 = fr.run(qa_request3);
//     //cout << response2.dump() <<endl;
//     REQUIRE(response2["next_question"] == "END");
//     //cout << "---==__==__==__==__==---" << endl;
//     //carlos->get_tasker().print_out();
//     //carlos->get_scheduler().print_out();
//     REQUIRE(carlos->get_tasker().find_task("base_task") == nullptr);
//     REQUIRE( carlos->get_scheduler().get_task("washer_clean_up") == nullptr);
    
//     auto washer_cleanup_task = carlos->get_tasker().find_task("washer_clean_up");

//     REQUIRE(washer_cleanup_task == nullptr);
 
//     auto size = carlos->get_scheduler().size();
//     REQUIRE( size == 1);
//     REQUIRE( carlos->get_tasker().size() == 0 );
//     //cout << "FINAL SCHEDULER" << endl;
//     //carlos->get_scheduler().print_out();

//     carlos->clear();
// }