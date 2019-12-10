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

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::model;

DefaultApiImpl::DefaultApiImpl(std::shared_ptr<Pistache::Rest::Router> rtr)
    : DefaultApi(rtr)
    { }

void DefaultApiImpl::delete_userusername(const std::string &username, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_apps(Pistache::Http::ResponseWriter &response) {
    json forms = read_form_names();
    response.send(Pistache::Http::Code::Ok, forms.dump(4));
}
void DefaultApiImpl::get_appsapp_id(const std::string &appId, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userdeveloper_form(const std::string &developer, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userdeveloper_formform_name(const std::string &developer, const std::string &formName, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userusername_apps(const std::string &username, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userusername_appsinstall_app_id(const std::string &username, const std::string &installAppId, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userusername_questionaryapp_id(const std::string &username, const std::string &appId, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::get_userusername_task(const std::string &username, const Inline_object_1 &inlineObject1, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::post_userdeveloper_form(const std::string &developer, const Inline_object_2 &inlineObject2, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}
void DefaultApiImpl::post_userusername_questionaryapp_id(const std::string &username, const std::string &appId, const Inline_object_3 &inlineObject3, Pistache::Http::ResponseWriter &response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
}

void DefaultApiImpl::user_post(const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response) {
    auto usr = create_user(inlineObject.getUsername());
    
    response.send(Pistache::Http::Code::Ok, "Created");
}

void DefaultApiImpl::user_username_get(const std::string &username, Pistache::Http::ResponseWriter &response) {

    const auto usr = read_user(username);
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

