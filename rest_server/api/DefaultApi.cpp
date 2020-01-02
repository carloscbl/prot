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

#include "DefaultApi.h"
#include "Helpers.h"

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::helpers;
using namespace org::openapitools::server::model;

DefaultApi::DefaultApi(std::shared_ptr<Pistache::Rest::Router> rtr) { 
    router = rtr;
}

void DefaultApi::init() {
    setupRoutes();
}

void DefaultApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Get(*router, base + "/apps", Routes::bind(&DefaultApi::apps_get_handler, this));
    Routes::Get(*router, base + "/apps/:id", Routes::bind(&DefaultApi::apps_id_get_handler, this));
    Routes::Delete(*router, base + "/user/:username", Routes::bind(&DefaultApi::delete_userusername_handler, this));
    Routes::Post(*router, base + "/user/:developer/form", Routes::bind(&DefaultApi::post_userdeveloper_form_handler, this));
    Routes::Get(*router, base + "/user/:developer/form/:formId", Routes::bind(&DefaultApi::user_developer_form_form_id_get_handler, this));
    Routes::Get(*router, base + "/user/:developer/form", Routes::bind(&DefaultApi::user_developer_form_get_handler, this));
    Routes::Post(*router, base + "/user", Routes::bind(&DefaultApi::user_post_handler, this));
    Routes::Get(*router, base + "/user/:username/apps", Routes::bind(&DefaultApi::user_username_apps_get_handler, this));
    Routes::Delete(*router, base + "/user/:username/apps/:installAppId", Routes::bind(&DefaultApi::user_username_apps_install_app_id_delete_handler, this));
    Routes::Get(*router, base + "/user/:username/apps/:installAppId", Routes::bind(&DefaultApi::user_username_apps_install_app_id_get_handler, this));
    Routes::Post(*router, base + "/user/:username/apps/:installAppId", Routes::bind(&DefaultApi::user_username_apps_install_app_id_post_handler, this));
    Routes::Get(*router, base + "/user/:username", Routes::bind(&DefaultApi::user_username_get_handler, this));
    Routes::Get(*router, base + "/user/:username/questionary/:appId", Routes::bind(&DefaultApi::user_username_questionary_app_id_get_handler, this));
    Routes::Post(*router, base + "/user/:username/questionary/:appId", Routes::bind(&DefaultApi::user_username_questionary_app_id_post_handler, this));
    Routes::Get(*router, base + "/user/:username/tasks", Routes::bind(&DefaultApi::user_username_tasks_get_handler, this));

    // Default handler, called when a route is not found
    router->addCustomHandler(Routes::bind(&DefaultApi::default_api_default_handler, this));
}

void DefaultApi::apps_get_handler(const Pistache::Rest::Request &, Pistache::Http::ResponseWriter response) {

    try {
      this->apps_get(response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::apps_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto id = request.param(":id").as<int32_t>();
    
    try {
      this->apps_id_get(id, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::delete_userusername_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    
    try {
      this->delete_userusername(username, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::post_userdeveloper_form_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto developer = request.param(":developer").as<std::string>();
    
    // Getting the body param
    
    Inline_object_1 inlineObject1;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject1);
      this->post_userdeveloper_form(developer, inlineObject1, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_developer_form_form_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto developer = request.param(":developer").as<std::string>();
    auto formId = request.param(":formId").as<int32_t>();
    
    try {
      this->user_developer_form_form_id_get(developer, formId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_developer_form_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto developer = request.param(":developer").as<std::string>();
    
    try {
      this->user_developer_form_get(developer, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {

    // Getting the body param
    
    Inline_object inlineObject;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject);
      this->user_post(inlineObject, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_apps_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    
    try {
      this->user_username_apps_get(username, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_apps_install_app_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    auto installAppId = request.param(":installAppId").as<int32_t>();
    
    try {
      this->user_username_apps_install_app_id_delete(username, installAppId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_apps_install_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    auto installAppId = request.param(":installAppId").as<int32_t>();
    
    try {
      this->user_username_apps_install_app_id_get(username, installAppId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_apps_install_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    auto installAppId = request.param(":installAppId").as<int32_t>();
    
    try {
      this->user_username_apps_install_app_id_post(username, installAppId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    
    try {
      this->user_username_get(username, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_questionary_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    auto appId = request.param(":appId").as<int32_t>();
    
    try {
      this->user_username_questionary_app_id_get(username, appId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_questionary_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    auto appId = request.param(":appId").as<int32_t>();
    
    // Getting the body param
    
    Inline_object_2 inlineObject2;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject2);
      this->user_username_questionary_app_id_post(username, appId, inlineObject2, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_username_tasks_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    auto username = request.param(":username").as<std::string>();
    
    try {
      this->user_username_tasks_get(username, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}

void DefaultApi::default_api_default_handler(const Pistache::Rest::Request &, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}
}
}
}

