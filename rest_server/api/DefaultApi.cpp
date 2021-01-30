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
#include "spdlog/spdlog.h"

namespace org {
namespace openapitools {
namespace server {
namespace api {

using namespace org::openapitools::server::helpers;
using namespace org::openapitools::server::model;

class posthandler_raii{
    const Pistache::Rest::Request &request;
    const Pistache::Http::ResponseWriter& response;
    public:
    posthandler_raii(const Pistache::Rest::Request &request, const Pistache::Http::ResponseWriter& response):request(request),response(response){}
    ~posthandler_raii(){
        SPDLOG_DEBUG("{} {} -> {} {} ", methodString(request.method()), request.resource(), response.getResponseCode(), codeString(response.getResponseCode()) );
    }
};

DefaultApi::DefaultApi(std::shared_ptr<Pistache::Rest::Router> rtr) { 
    router = rtr;
}

void DefaultApi::init() {
    setupRoutes();
}

bool prehandler( Pistache::Http::Request & request, Pistache::Http::ResponseWriter& response) {
    SPDLOG_DEBUG("{} {}", methodString(request.method()) , request.resource());
    return true;
}


void DefaultApi::setupRoutes() {
    using namespace Pistache::Rest;
    router->addMiddleware(Routes::middleware(&prehandler));
    Routes::Get(*router, base + "/apps", Routes::bind(&DefaultApi::apps_get_handler, this));
    Routes::Get(*router, base + "/apps/:id", Routes::bind(&DefaultApi::apps_id_get_handler, this));
    Routes::Get(*router, base + "/user/:developer/app/:appId", Routes::bind(&DefaultApi::user_developer_app_app_id_get_handler, this));
    Routes::Get(*router, base + "/user/:developer/app", Routes::bind(&DefaultApi::user_developer_app_get_handler, this));
    Routes::Post(*router, base + "/user/:developer/app", Routes::bind(&DefaultApi::user_developer_app_post_handler, this));
    Routes::Post(*router, base + "/user", Routes::bind(&DefaultApi::user_post_handler, this));
    Routes::Get(*router, base + "/user/:userId/apps", Routes::bind(&DefaultApi::user_user_id_apps_get_handler, this));
    Routes::Delete(*router, base + "/user/:userId/apps/:installAppId", Routes::bind(&DefaultApi::user_user_id_apps_install_app_id_delete_handler, this));
    Routes::Get(*router, base + "/user/:userId/apps/:installAppId", Routes::bind(&DefaultApi::user_user_id_apps_install_app_id_get_handler, this));
    Routes::Post(*router, base + "/user/:userId/apps/:installAppId", Routes::bind(&DefaultApi::user_user_id_apps_install_app_id_post_handler, this));
    Routes::Delete(*router, base + "/user/:userId", Routes::bind(&DefaultApi::user_user_id_delete_handler, this));
    Routes::Get(*router, base + "/user/:userId", Routes::bind(&DefaultApi::user_user_id_get_handler, this));
    Routes::Get(*router, base + "/user/:userId/questionary/:appId", Routes::bind(&DefaultApi::user_user_id_questionary_app_id_get_handler, this));
    Routes::Post(*router, base + "/user/:userId/questionary/:appId", Routes::bind(&DefaultApi::user_user_id_questionary_app_id_post_handler, this));
    Routes::Delete(*router, base + "/user/:userId/task/:taskId", Routes::bind(&DefaultApi::user_user_id_task_task_id_delete_handler, this));
    Routes::Get(*router, base + "/user/:userId/task/:taskId", Routes::bind(&DefaultApi::user_user_id_task_task_id_get_handler, this));
    Routes::Get(*router, base + "/user/:userId/tasks", Routes::bind(&DefaultApi::user_user_id_tasks_get_handler, this));
    Routes::Post(*router, base + "/user/:userId/tasks", Routes::bind(&DefaultApi::user_user_id_tasks_post_handler, this));
    // Default handler, called when a route is not found
    router->addCustomHandler(Routes::bind(&DefaultApi::default_api_default_handler, this));
}

void DefaultApi::apps_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    posthandler_raii ph(request, response);
    try {
      this->apps_get(response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}

void DefaultApi::apps_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto id = request.param(":id").as<int32_t>();
    
    try {
      this->apps_id_get(id, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_developer_app_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto developer = request.param(":developer").as<std::string>();
    auto appId = request.param(":appId").as<int32_t>();
    
    try {
      this->user_developer_app_app_id_get(developer, appId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_developer_app_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto developer = request.param(":developer").as<std::string>();
    
    try {
      this->user_developer_app_get(developer, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_developer_app_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto developer = request.param(":developer").as<std::string>();
    
    // Getting the body param
    
    Inline_object_2 inlineObject2;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject2);
      this->user_developer_app_post(developer, inlineObject2, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {

    // Getting the body param
    posthandler_raii ph(request, response);
    
    Inline_object inlineObject;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject);
      this->user_post(inlineObject, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_apps_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_apps_get(userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_apps_install_app_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto installAppId = request.param(":installAppId").as<int32_t>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_apps_install_app_id_delete(installAppId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_apps_install_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto installAppId = request.param(":installAppId").as<int32_t>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_apps_install_app_id_get(installAppId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_apps_install_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto installAppId = request.param(":installAppId").as<int32_t>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_apps_install_app_id_post(installAppId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_delete(userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_get(userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_questionary_app_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto appId = request.param(":appId").as<int32_t>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_questionary_app_id_get(appId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_questionary_app_id_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto appId = request.param(":appId").as<int32_t>();
    auto userId = request.param(":userId").as<std::string>();
    
    // Getting the body param
    
    Inline_object_3 inlineObject3;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject3);
      this->user_user_id_questionary_app_id_post(appId, userId, inlineObject3, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_task_task_id_delete_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto taskId = request.param(":taskId").as<std::string>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_task_task_id_delete(taskId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_task_task_id_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto taskId = request.param(":taskId").as<std::string>();
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_task_task_id_get(taskId, userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_tasks_get_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto userId = request.param(":userId").as<std::string>();
    
    try {
      this->user_user_id_tasks_get(userId, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}
void DefaultApi::user_user_id_tasks_post_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    // Getting the path params
    posthandler_raii ph(request, response);
    auto userId = request.param(":userId").as<std::string>();
    
    // Getting the body param
    
    Inline_object_1 inlineObject1;
    
    try {
      nlohmann::json::parse(request.body()).get_to(inlineObject1);
      this->user_user_id_tasks_post(userId, inlineObject1, response);
    } catch (nlohmann::detail::exception &e) {
        //send a 400 error
        response.send(Pistache::Http::Code::Bad_Request, e.what());
        return;
    } catch (Pistache::Http::HttpError &e) {
        response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
        return;
    } catch (std::exception &e) {
        //send a 500 error
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
        return;
    }

}

void DefaultApi::default_api_default_handler(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    posthandler_raii ph(request, response);
    response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}
}
}
}

