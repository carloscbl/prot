#pragma once
#include "discovered_components.h"
#include "spdlog/spdlog.h"
#include "cloud_app_runner.h"


// class app_projector
// {
//     discovered_components & discovery;
// public:
//     app_projector(discovered_components & discovery);

//     void project(){}
// };

class app_projector
{
    discovered_components & discovery;
public:
    app_projector(discovered_components & discovery):discovery(discovery){};

    void project(){
        // This will perform a run of cloud_app_runner
        if ( !discovery.is_built()){
            if(!discovery.build()){
                return;
            }
        }

        // cloud_app_runner car (*discovery.m_user, *discovery.m_app );
        // car.run();
        SPDLOG_DEBUG(" patata" );
    }
};