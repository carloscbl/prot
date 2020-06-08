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
#include "trace_bullet.hpp"
#include "form_runner.h"

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
    { }

void DefaultApiImpl::user_username_delete(const std::string &username, Pistache::Http::ResponseWriter &response) {
    if(delete_user(geturl_decode(username))){
        response.send(Pistache::Http::Code::Ok, "Done");
    }else{
        response.send(Pistache::Http::Code::Not_Found, "Username doest not exists");
    }
}

void DefaultApiImpl::apps_get(Pistache::Http::ResponseWriter &response) {
    auto binds_forms = read_form_names();
    vector<Prot_app_info> apps;
    for (auto &[ k,v] : binds_forms)
    {
        Prot_app_info app;
        app.setFormId(k);
        app.setFormName(v);
        apps.push_back(app);
    }
    json jresponse = apps;
    response.send(Pistache::Http::Code::Ok, jresponse.dump(4));
}

void DefaultApiImpl::apps_id_get(const int32_t &id, Pistache::Http::ResponseWriter &response){
    auto ff = read_form_by_id(id);
    if(!ff.has_value()){
        response.send(Pistache::Http::Code::Not_Found, "app id doesn't match any");
        return;
    }
    auto paired = ff.value();
    Prot_app_info app;
    app.setFormId(paired.first);
    app.setFormName(paired.second);
    json jresponse = app;
    response.send(Pistache::Http::Code::Ok, jresponse.dump(4));
}

void DefaultApiImpl::user_developer_form_get(const std::string &developer, Pistache::Http::ResponseWriter &response) {
    //Need to create a new call that joins forms, by a given developer
    auto mp = read_forms_by_developer(geturl_decode(developer));
    vector<string> form_names;

    for_each(mp.begin(), mp.end(), [&form_names](const pair<uint64_t,string>& c){
        form_names.push_back(c.second);
    });
    json jresponse = form_names;
    response.send( Pistache::Http::Code::Ok, jresponse.dump(4) );
}
void DefaultApiImpl::user_developer_form_form_id_get(const std::string &developer, const int32_t &formId, Pistache::Http::ResponseWriter &response) {
    auto form_ =read_form_by_id (formId);
    if(!form_.has_value()){
        response.send(Pistache::Http::Code::Not_Found, "not form for that id");
        return;
    }
    auto form_res =read_form(form_.value().second);
    
    response.send(Pistache::Http::Code::Ok, form_res->get_json().dump(4));
}

void DefaultApiImpl::user_username_apps_get(const std::string &username, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(username);
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    json jsresponse;
    for(const auto & [k,v] : read_instalations(decoded) ){
        json inner;
        inner["appID"] = k;
        inner["appName"] = v;
        jsresponse.push_back(inner);
    }

    response.send(Pistache::Http::Code::Ok, jsresponse.dump(4));
}

void DefaultApiImpl::user_username_apps_install_app_id_get(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(username);
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    auto res = read_instalations(decoded, installAppId);
    if(res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "Not found id");
        return;
    }
    json jsresponse;
    jsresponse["appID"]= res.begin()->first;
    jsresponse["appName"]= res.begin()->second;

    response.send(Pistache::Http::Code::Ok, jsresponse.dump(4));
}

void DefaultApiImpl::user_username_apps_install_app_id_delete(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response){
    std::string decoded = geturl_decode(username);
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    if(!read_form_by_id(installAppId)){
        response.send(Pistache::Http::Code::Not_Found, "form does not exists");
        return;
    }
    auto res = read_instalations(decoded, installAppId);
    if(res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "User doesn't have that instalation");
        return;
    }
    delete_instalation(decoded,installAppId);

    response.send(Pistache::Http::Code::Ok, "Done");
}

void DefaultApiImpl::user_username_apps_install_app_id_post(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response){
    std::string decoded = geturl_decode(username);
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    auto form_name = read_form_by_id(installAppId);
    if(!form_name){
        response.send(Pistache::Http::Code::Not_Found, "form does not exists");
        return;
    }
    auto res = read_instalations(decoded, installAppId);
    if(!res.empty()){
        response.send(Pistache::Http::Code::Not_Found, "User already have that instalation");
        return;
    }
    if(!create_instalation(decoded,form_name.value().second)){
        response.send(Pistache::Http::Code::Not_Found, "unable to do instalation");
        return;
    }

    response.send(Pistache::Http::Code::Ok, "Done");
}

void DefaultApiImpl::user_username_questionary_app_id_get(const std::string &username, const int32_t &appId, Pistache::Http::ResponseWriter &response) {
    //This is the request of questions
    auto usr = read_user(geturl_decode(username));
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }

    auto pair = read_form_by_id(appId);
    auto form_ = read_form(pair->second);
    if(!form_){
        response.send(Pistache::Http::Code::Not_Found, "form does not exists");
        return ;
    }
    form_runner fr(*usr, *form_);
    json qa_request1;
    auto & qa_res = fr.run(qa_request1);
    Inline_response_200_1 response_200_1;
    response_200_1.setCurrentQuestion(qa_res["next_question"].get<string>());

    response.send(Pistache::Http::Code::Ok, json(response_200_1).dump(4));
}

void DefaultApiImpl::user_username_tasks_get(const std::string &username, Pistache::Http::ResponseWriter &response) {
    //Get all tasks for the user
    cout << "Inside user_username_tasks_get" << endl;
    std::string decoded = geturl_decode(username);
    auto tsks = read_tasks(decoded);
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
void DefaultApiImpl::user_developer_form_post(const std::string &developer, const Inline_object_2 &inlineObject2, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(developer);
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Unauthorized, "Developer does not exists as user");
    }
    json whole_request(inlineObject2);
    auto res = create_form( whole_request.at("form_obj") ,decoded);
    if(!res){
        response.send(Pistache::Http::Code::Not_Acceptable, "bad json");
    }
    response.send(Pistache::Http::Code::Created, "Done");
}
void DefaultApiImpl::user_username_questionary_app_id_post(const std::string &username, const int32_t &appId, const Inline_object_3 &inlineObject3, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(username);
    auto usr = read_user(decoded);
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    if(inlineObject3.restartIsSet()){
        response.send(Pistache::Http::Code::Ok, "Rested " + decoded + " " + std::to_string(appId) );
        auto session = read_session(usr->get_name() , appId);
        if (session){
            //We need to delete too, all associated tasks!
            delete_session(session->id);
        }
        return;
    }
    auto pair = read_form_by_id(appId);
    auto form_ = read_form(pair->second);
    form_runner fr(*usr, *form_);
    json qa_request;
    qa_request["answer"] = inlineObject3.getResponse();
    auto & qa_res = fr.run(qa_request);

    Inline_response_200_1 response_200_1;
    response_200_1.setCurrentQuestion(qa_res["next_question"].get<string>());
    response.send(Pistache::Http::Code::Ok, json(response_200_1).dump(4));
}

void DefaultApiImpl::user_username_task_task_id_delete(const std::string &username, const int32_t &taskId, Pistache::Http::ResponseWriter &response){
    std::string decoded = geturl_decode(username);
    if(delete_task(decoded, taskId )){
        response.send(Pistache::Http::Code::Ok, "Done" );
    }
    else{
        response.send(Pistache::Http::Code::Not_Found, "Not found!" );
    }
}

void DefaultApiImpl::user_username_task_task_id_get(const std::string &username, const int32_t &taskId, Pistache::Http::ResponseWriter &response){
    std::string decoded = geturl_decode(username);
    auto result = read_task(decoded, taskId);
    if(!result){
        response.send(Pistache::Http::Code::Not_Found, "Not found!" );
        return;
    }
    json task = *result;
    response.send(Pistache::Http::Code::Ok, task.dump(4));
}

void DefaultApiImpl::user_username_tasks_post(const std::string &username, const Inline_object_1 &inlineObject1, Pistache::Http::ResponseWriter &response){
    std::string decoded = geturl_decode(username);
    //Update... delete and new
    if(!gen_exists<test_prot::Users>(decoded)){
        response.send(Pistache::Http::Code::Not_Found, "user does not exists");
        return;
    }
    task tk;
    try
    {
        json jauto = json(inlineObject1)["task"];
        if(inlineObject1.getTypeOfTask() == "auto"){
            from_json_auto_task(jauto, tk);
        }else if (inlineObject1.getTypeOfTask() == "user"){
            from_json_user_task(jauto, tk);
        }else{
            response.send(Pistache::Http::Code::Not_Found, "bad type of task");
            return;
        }
        if(! create_task({{decoded,false}},tk)){
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

void DefaultApiImpl::user_post(const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response) {
    auto usr = create_user(inlineObject.getUsername());
    if(!usr){
        response.send(Pistache::Http::Code::Bad_Request, "Already exists");
        return;
    }
    Inline_response_200 response_200;
    response_200.setUserId(usr->get_id());
    response.send(Pistache::Http::Code::Ok, json(response_200).dump(4));
}

void DefaultApiImpl::user_username_get(const std::string &username, Pistache::Http::ResponseWriter &response) {
    std::string decoded = geturl_decode(username);
    const auto usr = read_user(decoded);
    if(!usr){
        response.send(Pistache::Http::Code::Not_Found, "username does not exists");
        return;
    }
    json js = *usr;
    response.send(Pistache::Http::Code::Ok, js.dump(4));
}

}
}
}
}

