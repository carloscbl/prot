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
        try
        {
            if ( !discovery.is_built()){
                if(!discovery.build()){
                    return;
                }
            }
            if(!this->discovery.m_qaHistory){
                SPDLOG_ERROR(" user '{}' app '{}' doesn't contain qaHistory", discovery.user_id, discovery.app_id);
                return;
            }
            auto history = discovery.m_qaHistory->find("history");
            if(history == discovery.m_qaHistory->end()){
                SPDLOG_ERROR(" user '{}' app '{}' doesn't contain qaHistory -> history field json", discovery.user_id, discovery.app_id);
                return;
            }

            shared_ptr<taskstory_commit_batched_raii> batch = make_shared<taskstory_commit_batched_raii>(discovery.m_user->get_tasker());
            cloud_app_runner car (*discovery.m_user, *discovery.m_app ); // needs a non sessined mode
            // size_t fw_projections = discovery.m_user->conf.forward_projection;
            size_t fw_projections = 2;
            // size_t bw_projection = discovery.m_user->conf.backwards_projection;
            car.projected_run(*history, fw_projections, batch);
        }
        catch(const std::exception& e)
        {
            SPDLOG_ERROR("exception thrown {} ",e.what());
        }
    }
};