/**
* prot
* No description provided (generated by Openapi Generator https://github.com/openapitools/openapi-generator)
*
* The version of the OpenAPI document: 1.0
* 
*
* NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
* https://openapi-generator.tech
* Do not edit the class manually.
*/

#include "DefaultApiImpl.h"
#include "db_ops.hpp"
#include "cloud_app_runner.h"
#include "spdlog/spdlog.h"
#include "time_utils.hpp"

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::model;



bool url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

std::string geturl_decode(const std::string& in){
    std::string out;
    if(url_decode(in,out)){
        return out;
    }
    throw std::runtime_error("Error: decoding url" + in);
}

DefaultApiImpl::DefaultApiImpl(std::shared_ptr<Pistache::Rest::Router> rtr)
    : DefaultApi(rtr)
    {
        // / rtr->addCustomHandler
    }

void DefaultApiImpl::user_user_id_delete(const std::string &userId, Pistache::Http::ResponseWriter &response) {
    if(delete_user(userId)){
        response.send(Pistache::Http::Code::Ok, "Done");
    }else{
        response.send(Pistache::Http::Code::Not_Found, "userId doest not exists");
    }
}

void DefaultApiImpl::apps_get(Pistache::Http::ResponseWriter &response) {
    auto binds_apps = read_app_meta();
    vector<Prot_app_info> apps;
    for (auto &[ k,v] : binds_apps)
    {
        Prot_app_info app;
        app.setAppId(k);
        app.setAppName(v["name"]);
        app.setAppUnstructuredInfo(v);
        apps.push_back(app);
    }
    json jresponse = apps;
    response.send(Pistache::Http::Code::Ok, jresponse.dump(4));
}

void DefaultApiImpl::apps_id_get(const int32_t &id, Pistache::Http::ResponseWriter &response){
    auto ff = read_app_by_id(id);
    if(!ff.has_value()){
        response.send(Pistache::Http::Code::Not_Found, "app id doesn't match any");
        return;
    }
    Prot_app_info app;
    app.setAppId(ff->second["id"]);
    app.setAppName(ff->second["name"]);
    app.setAppUnstructuredInfo(ff->second);
    json jresponse = app;
    response.send(Pistache::Http::Code::Ok, jresponse.dump(4));
}

void DefaultApiImpl::user_developer_app_get(const std::string &developer, Pistache::Http::ResponseWriter &response) {
    //Need to create a new call that joins apps, by a given developer
    auto mp = read_apps_by_developer(geturl_decode(developer));
    json jresponse = json::object();
    jresponse["items"] = json::array();

    for_each(mp.begin(), mp.end(), [&jresponse](const pair<uint64_t,string>& c){
        jresponse["items"].push_back({
            {"app_id",std::to_string(c.first)},
            {"app_name",c.second}
        });
    });
    response.send( Pistache::Http::Code::Ok, jresponse.dump(4) );
}
void DefaultApiImpl::user_developer_app_app_id_get(const std::string &developer, const int32_t &appId, Pistache::Http::ResponseWriter &response) {
    auto app_ =read_app_by_id (appId);
    if(!app_.has_value()){
        response.send(Pistache::Http::Code::Not_Found, "not app for that id");
        return;
    }
    auto app_res = move(app_->first);// read_app(app_.value().second["name"]);
    json res = json::object();
    res["app_obj"] = app_res->get_json().dump();
    response.send(Pistache::Http::Code::Ok, res);
}

void DefaultApiImpl::user_user_id_apps_get(const std::string &userId, Pistache::Http::ResponseWriter &response) {
    if(!gen_exists<orm_prot::Users>(userId)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    json jsresponse;
    for(const auto & [k,v] : read_instalations(userId) ){
        json inner;
        inner["app_id"] = k;
        inner["app_name"] = v.app_name;
        inner["qa_history"] = v.qa_history;
        jsresponse.push_back(inner);
    }

    response.send(Pistache::Http::Code::Ok, jsresponse.dump(4));
}

void DefaultApiImpl::user_user_id_apps_install_app_id_get(const int32_t &installAppId, const std::string &userId, Pistache::Http::ResponseWriter &response) {
    if(!gen_exists<orm_prot::Users>(userId)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    auto res = read_instalations(userId, installAppId);
    if(res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "Not found id");
        return;
    }
    json jsresponse;
    jsresponse["app_id"]= res.begin()->first;
    jsresponse["app_name"]= res.begin()->second.app_name;
    jsresponse["qa_history"] = res.begin()->second.qa_history;

    response.send(Pistache::Http::Code::Ok, jsresponse.dump(4));
}

void DefaultApiImpl::user_user_id_apps_install_app_id_delete(const int32_t &installAppId, const std::string &userId, Pistache::Http::ResponseWriter &response){
    if(!gen_exists<orm_prot::Users>(userId)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    if(!read_app_by_id(installAppId)){
        response.send(Pistache::Http::Code::Not_Found, "app does not exists");
        return;
    }
    auto res = read_instalations(userId, installAppId);
    if(res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "User doesn't have that instalation");
        return;
    }
    delete_instalation(userId,installAppId);

    response.send(Pistache::Http::Code::Ok, "Done");
}

void DefaultApiImpl::user_user_id_apps_install_app_id_post(const int32_t &installAppId, const std::string &userId, Pistache::Http::ResponseWriter &response){
    if(!gen_exists<orm_prot::Users>(userId)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    auto app_name = read_app_by_id(installAppId);
    if(!app_name){
        response.send(Pistache::Http::Code::Not_Found, "app does not exists");
        return;
    }
    auto res = read_instalations(userId, installAppId);
    if(!res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "User already have that instalation");
        return;
    }
    if(!create_instalation(userId,app_name.value().second["name"])){
        response.send(Pistache::Http::Code::Not_Found, "unable to do instalation");
        return;
    }

    response.send(Pistache::Http::Code::Ok, "Done");
}

void DefaultApiImpl::user_user_id_questionary_app_id_get(const int32_t &appId, const std::string &userId, Pistache::Http::ResponseWriter &response) {
    //This is the request of questions
    auto usr = read_user(userId);
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }

    auto pair = read_app_by_id(appId);
    auto app_ = move(pair->first);//read_app(pair->second["name"]);
    if(!app_){
        response.send(Pistache::Http::Code::Not_Found, "app does not exists");
        return ;
    }
    cloud_app_runner fr(*usr, *app_);
    json qa_request1;
    auto & qa_res = fr.run(qa_request1);
    Inline_response_200_2 response_200_2;
    response_200_2.setCurrentQuestion(qa_res["next_question"].get<string>());
    response_200_2.setDataType(qa_res["data_type"].get<string>());
    response_200_2.setTypeDetails(qa_res["type_details"]);

    response.send(Pistache::Http::Code::Ok, json(response_200_2).dump(4));
}

void DefaultApiImpl::user_user_id_tasks_get(const std::string &userId, Pistache::Http::ResponseWriter &response) {
    //Get all tasks for the user
    cout << "Inside user_user_id_tasks_get" << endl;
    auto tsks = read_tasks(userId);
    if(tsks.empty()){
        response.send(Pistache::Http::Code::Not_Found, "not tasks or user");
    }

    json jresponse;
    for (auto &&[k,v] : tsks)
    {
        json new_json;
        nlohmann::to_json(new_json,*v);
        cout << new_json.dump(4) << endl;
        jresponse.push_back(new_json);
    }
    
    response.send(Pistache::Http::Code::Ok, jresponse.dump(4) );
}
void DefaultApiImpl::user_developer_app_post(const std::string &developer, const Inline_object_3 &inlineObject3, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(developer);
    if(!gen_exists<orm_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Unauthorized, "Developer does not exists as user");
    }
    auto res = create_app( inlineObject3.getAppObj() ,decoded);
    if(!res){
        response.send(Pistache::Http::Code::Not_Acceptable, "bad json");
    }
    response.send(Pistache::Http::Code::Created, "Done");
}

void DefaultApiImpl::user_user_id_questionary_app_id_post(const int32_t &appId, const std::string &userId, const Inline_object_5 &inlineObject5, Pistache::Http::ResponseWriter &response) {
    measure_execution_raii(__FUNCTION__);
    auto usr = read_user(userId);
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }

    if(inlineObject5.restartIsSet()){
        response.send(Pistache::Http::Code::Ok, "Reseted " + std::to_string(appId) );
        auto session = read_session(usr->get_id() , appId);
        if (session){
            //We need to delete too, all associated tasks!
            delete_session(session->id);
        }
        return;
    }

    auto pair = read_app_by_id(appId);
    auto app_ = move(pair->first);//read_app(pair->second["name"]);
    cloud_app_runner car(*usr, *app_);
    json qa_request;
    qa_request["answer"] = inlineObject5.getResponse();
    auto & qa_res = car.run(qa_request);

    json response_json ={
        {"next_question", qa_res["next_question"]},
    };
    if(car.scheduled_tasks.has_value()){
        for(auto && v: car.scheduled_tasks.value()){
            response_json["taskstory"].push_back(v->get_json());
        }
    }
    response.send(Pistache::Http::Code::Ok, response_json.dump(4));
}

void DefaultApiImpl::user_user_id_task_task_id_delete(const std::string &taskId, const std::string &userId, Pistache::Http::ResponseWriter &response){
    if(delete_task(userId, taskId )){
        response.send(Pistache::Http::Code::Ok, "Done" );
    }
    else{
        response.send(Pistache::Http::Code::Not_Found, "Not found!" );
    }
}

void DefaultApiImpl::user_user_id_task_task_id_get(const std::string &taskId, const std::string &userId, Pistache::Http::ResponseWriter &response){
    auto result = read_task_secure(userId, taskId);
    if(!result){
        response.send(Pistache::Http::Code::Not_Found, "Not found!" );
        return;
    }
    json task = *result;
    response.send(Pistache::Http::Code::Ok, task.dump(4));
}

void DefaultApiImpl::user_user_id_tasks_post(const std::string &userId, const Inline_object_2 &inlineObject2, Pistache::Http::ResponseWriter &response){
    //Update... delete and new
    if(!gen_exists<orm_prot::Users>(userId)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    task tk;
    try
    {
        json jauto = json(inlineObject2)["task"];
        if(inlineObject2.getTypeOfTask() == "auto"){
            from_json_auto_task(jauto, tk);
        }else if (inlineObject2.getTypeOfTask() == "user"){
            if (jauto.find("external_id") == jauto.end()){
                response.send(Pistache::Http::Code::Not_Found, "if task is from a user calendar it needs an external_id");
                return;
            }
            from_json_user_task(jauto, tk);
        }else{
            response.send(Pistache::Http::Code::Not_Found, "bad type of task");
            return;
        }
        if(! create_task({{userId,false}},tk)){
            response.send(Pistache::Http::Code::Not_Found, "unable to create task");
            return;
        }
        response.send(Pistache::Http::Code::Ok, "Done");
        return;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        response.send(Pistache::Http::Code::Not_Found, "bad json conversion" + string(e.what()) );
        return;
    }
    
    
    //create_task()
}

void DefaultApiImpl::user_post(const Inline_object_1 &inlineObject1, Pistache::Http::ResponseWriter &response) {
    auto usr = create_user(inlineObject1.getUserId(),inlineObject1.getUsername());
    if(!usr){
        response.send(Pistache::Http::Code::Bad_Request, "Already exists");
        return;
    }
    Inline_response_200 response_200;
    response_200.setUserId(usr->get_id());
    response.send(Pistache::Http::Code::Ok, json(response_200).dump(4));
}

void DefaultApiImpl::user_user_id_get(const std::string &userId, Pistache::Http::ResponseWriter &response) {
    const auto usr = read_user(userId);
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "userId does not exists");
        return;
    }
    json js = *usr;
    response.send(Pistache::Http::Code::Ok, js.dump(4));
}

void DefaultApiImpl::user_user_id_patch(const std::string &userId, const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response){

}

void DefaultApiImpl::user_developer_app_app_id_delete(const std::string &developer, const int32_t &appId, Pistache::Http::ResponseWriter &response){

    if(db_op::delete_app(appId,developer)){
        response.send(Pistache::Http::Code::Ok);
        return;
    }
    response.send(Pistache::Http::Code::Not_Found);
}

void DefaultApiImpl::user_developer_app_app_id_put(const std::string &developer, const int32_t &appId, const Inline_object_4 &inlineObject4, Pistache::Http::ResponseWriter &response){
    auto res = db_op::update_app(inlineObject4.getAppObj(), developer, appId);
    if(res){
        response.send(Pistache::Http::Code::Ok);
        return;
    }
    response.send(Pistache::Http::Code::Not_Found);

}


}
}
}
}

