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
 * DefaultApi.h
 *
 * 
 */

#ifndef DefaultApi_H_
#define DefaultApi_H_


#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>


#include "Inline_object.h"
#include "Inline_object_1.h"
#include "Inline_object_2.h"
#include "Inline_object_3.h"
#include "Inline_response_200.h"
#include "Object.h"
#include "Prot_app_info.h"
#include "User.h"
#include <string>

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::model;

class  DefaultApi {
public:
    DefaultApi(std::shared_ptr<Pistache::Rest::Router>);
    virtual ~DefaultApi() {}
    void init();

    const std::string base = "";

private:
    void setupRoutes();

    void apps_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void apps_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_developer_form_form_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_developer_form_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_developer_form_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_apps_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_apps_install_app_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_apps_install_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_apps_install_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_questionary_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_questionary_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_task_task_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_task_task_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_tasks_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void user_username_tasks_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
    void default_api_default_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

    std::shared_ptr<Pistache::Rest::Router> router;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    virtual void apps_get(Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="id"></param>
    virtual void apps_id_get(const int32_t &id, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="developer"></param>
    /// <param name="formId"></param>
    virtual void user_developer_form_form_id_get(const std::string &developer, const int32_t &formId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="developer"></param>
    virtual void user_developer_form_get(const std::string &developer, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="developer"></param>
    /// <param name="inlineObject2"> (optional)</param>
    virtual void user_developer_form_post(const std::string &developer, const Inline_object_2 &inlineObject2, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="inlineObject"> (optional)</param>
    virtual void user_post(const Inline_object &inlineObject, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    virtual void user_username_apps_get(const std::string &username, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="installAppId"></param>
    virtual void user_username_apps_install_app_id_delete(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="installAppId"></param>
    virtual void user_username_apps_install_app_id_get(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="installAppId"></param>
    virtual void user_username_apps_install_app_id_post(const std::string &username, const int32_t &installAppId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    virtual void user_username_delete(const std::string &username, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    virtual void user_username_get(const std::string &username, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// Returns the current question pending to be responded
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="appId"></param>
    virtual void user_username_questionary_app_id_get(const std::string &username, const int32_t &appId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="appId"></param>
    /// <param name="inlineObject3"> (optional)</param>
    virtual void user_username_questionary_app_id_post(const std::string &username, const int32_t &appId, const Inline_object_3 &inlineObject3, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="taskId"></param>
    virtual void user_username_task_task_id_delete(const std::string &username, const std::string &taskId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="taskId"></param>
    virtual void user_username_task_task_id_get(const std::string &username, const std::string &taskId, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// Your GET endpoint
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    virtual void user_username_tasks_get(const std::string &username, Pistache::Http::ResponseWriter &response) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="username"></param>
    /// <param name="inlineObject1"> (optional)</param>
    virtual void user_username_tasks_post(const std::string &username, const Inline_object_1 &inlineObject1, Pistache::Http::ResponseWriter &response) = 0;

};

}
}
}
}

#endif /* DefaultApi_H_ */

