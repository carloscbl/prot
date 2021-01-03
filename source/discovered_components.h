#pragma once

#include <memory>
#include "json.hpp"
#include "app.h"
#include "user.h"
#include "time_utils.hpp"


using std::unique_ptr;

class discovered_components{
    using time_point = system_clock::time_point;
    unique_ptr<user> m_user;
    unique_ptr<app> m_app;
    bool m_built = false;

    public:
    const string user_id ;
    const uint64_t app_id = 0;
    const unique_ptr<json> m_qaHistory;
    const time_point m_lastDiscovered;
    const uint64_t user_app_id = 0;

    discovered_components(const string user_id ,
        const uint64_t app_id,
        unique_ptr<json> m_qaHistory,
        const time_point m_lastDiscovered,
        const uint64_t user_app_id
    ):
        user_id(user_id),
        app_id(app_id),
        m_qaHistory(std::move(m_qaHistory)),
        m_lastDiscovered(m_lastDiscovered),
        user_app_id(user_app_id)
    {};

    bool build();
};