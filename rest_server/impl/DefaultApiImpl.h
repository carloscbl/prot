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

/*
* DefaultApiImpl.h
*
* 
*/

#ifndef DEFAULT_API_IMPL_H_
#define DEFAULT_API_IMPL_H_


#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <memory>

#include <DefaultApi.h>

#include <pistache/optional.h>

#include "Inline_object.h"
#include "Inline_object_1.h"
#include "Inline_object_2.h"
#include "Inline_object_3.h"
#include "Inline_response_200.h"
#include "Inline_response_200_1.h"
#include "Object.h"
#include "Prot_app_info.h"
#include "User.h"
#include <string>

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::model;

class DefaultApiImpl : public org::openapitools::server::api::DefaultApi {
public:
    DefaultApiImpl(std::shared_ptr<Pistache::Rest::Router>);
    ~DefaultApiImpl() {}

    void apps_get(Pistache::Http::ResponseWriter &response);
    void apps_id_get(const int32_t &id, Pistache::Http::ResponseWriter &response);
    void user_developer_form_form_id_get(const std::string &developer, const int32_t &formId, Pistache::Http::ResponseWriter &response);
    void user_developer_form_get(const std::string &developer, Pistache::Http::ResponseWriter &response);
    void user_developer_form_post(const std::string &developer, const Inline_object_2 &inlineObject2, Pistache::Http::ResponseWriter &response);
    void user_post(const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response);
    void user_username_apps_get(const std::string &username, Pistache::Http::ResponseWriter &response);
    void user_username_apps_install_app_id_delete(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response);
    void user_username_apps_install_app_id_get(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response);
    void user_username_apps_install_app_id_post(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response);
    void user_username_delete(const std::string &username, Pistache::Http::ResponseWriter &response);
    void user_username_get(const std::string &username, Pistache::Http::ResponseWriter &response);
    void user_username_questionary_app_id_get(const std::string &username, const int32_t &appId, Pistache::Http::ResponseWriter &response);
    void user_username_questionary_app_id_post(const std::string &username, const int32_t &appId, const Inline_object_3 &inlineObject3, Pistache::Http::ResponseWriter &response);
    void user_username_task_task_id_delete(const std::string &username, const int32_t &taskId, Pistache::Http::ResponseWriter &response);
    void user_username_task_task_id_get(const std::string &username, const int32_t &taskId, Pistache::Http::ResponseWriter &response);
    void user_username_tasks_get(const std::string &username, Pistache::Http::ResponseWriter &response);
    void user_username_tasks_post(const std::string &username, const Inline_object_1 &inlineObject1, Pistache::Http::ResponseWriter &response);

};

}
}
}
}



#endif