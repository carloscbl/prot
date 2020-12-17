#include "time_determinator.h"
#include "time_determinator.h"
#include "task_frequency.h"
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>
#include "expanded_taskstory_t.h"
#include "spdlog/spdlog.h"



time_determinator::time_determinator(task_t task_, scheduler &sche_, optional<size_t> designated_period_group) 
: task_(task_), sche_(sche_),m_designated_period_group(designated_period_group)
{
}

optional<bool> time_determinator::build(days local_start_offset, optional<days> projected_next_period_override_start_offset )
{
    //Formula:
    /*
    RP = restrictions (Period) # apply local restrictions to each day of period
    bool find_slot(RP, task)
    true set()
    false reallocate_and_set(is, RP, task)
    */
    //1º Apply restrictions to a period range
    //1.1 Get range
    // if(task_->get_tag() == "travel_to_activity"){
    //     cout << "aa" << endl;
    // }
    time_point end;
    if(projected_next_period_override_start_offset.has_value()){
        cout << "aa" <<endl;
    }

    if (is_specific_period().has_value()){
        days new_offset_offset = this->wildcard_data.value().offset_day;
        // made optional projection
        local_start_offset += projected_next_period_override_start_offset.value_or(new_offset_offset);
        end = floor<days>(system_clock::now() + local_start_offset) + days(1);
    }else{
        // made optional projection
        local_start_offset += projected_next_period_override_start_offset.value_or(days(0));
        end = this->task_->get_frequency().get_period() + projected_next_period_override_start_offset.value_or(days(0)) + system_clock::now();
    }

    time_point start = floor<days>(system_clock::now() + local_start_offset);
    using pipeline_t = function<bool(time_determinator *,const im_t &, time_point) >;
    pipeline_t pipeline = &time_determinator::forward_pipeline;

    auto when_ = this->task_->get_when();
    if(!when_.after.empty()){
        pipeline = &time_determinator::when_pipeline;
        // start += when_.minimum_delay.m_duration;
        // end += when_.maximum_delay.m_duration;
    }

    //when_
    //For each day Apply restrictions
    im_t interval_map = sche_.clone_interval_map();
    print_time(interval_map);
    
    cout << "\n"<< "trying " << this->task_->get_tag() << " into offset of " <<  local_start_offset.count()  <<"\n" << endl;
    //We need to traverse days within the first interval, but we need a policy to know
    // TODO: Policy when we pass the max frequiency range
    // TODO: How to move tasks that are not compatible with the current scheduler
    // if(task_->get_tag() == "washer_clean_up"){
    //     print_time(interval_map);
    // }
    days d = ceil<days>(end - start);

    if(d.count() <= 0){
        cout << __FILE__ << " EXITING EARLY NOT DAYS TO ALLOCATION" << endl;
        return nullopt;
    }
    build_elapsed_today_restriction(local_start_offset, start, interval_map);

    for (days::rep iteration_day = 0; iteration_day < d.count(); iteration_day++)
    {
        build_daily_restrictions(start, end, interval_map, iteration_day );
        if(pipeline(this, interval_map, get_current_day_begin(iteration_day, start) )) 
        {
            return true;
        }
    }
    //print_time(interval_map);
    return false;
}


const optional<size_t> time_determinator::is_specific_period() noexcept{
    const json & wild_task  = this->task_->get_json();
    optional<size_t> designated_period = nullopt;
    for(const auto & [k,mappings] : prot::designated_periods_to_ratio){
        if(wild_task.find(k) != wild_task.end()){
            designated_period = wild_task.at(k).get<size_t>() - prot::task_expansion::day_period_user_friendly_offset;

            auto today_index =  mappings.get_period_current_index();

            time_determinator::wildcard_time_determinator_data data_period{
                .period_ratio_name = k,
                .designated_period = designated_period.value(),
                .unit_ratio_in_seconds = mappings.get_ratio_seconds(1),
                .period_current_index = today_index,
                .offset_day = mappings.get_offset_day(days(today_index), designated_period.value()),
            };
            this->wildcard_data = data_period;
        }
    }
    return designated_period;
}

//We need to get the current user scheduler,
//Then we add the restrictions on top
//Restrictions apply json-land and then user-land configs
//Then we get the first free interval as start point

//IMPORTANT, We need to do an equal range for the selected day, and then create
// a local interval_map for that day restrictions

// The number need to be a stamp for the given start of the operated day
// If the interval is nightly then we need to check if the "to" less than "from"
// We need to add it for the next day

time_point time_determinator::get_current_day_begin(int64_t iteration_day, const time_point & from) const noexcept{
    return (iteration_day * days(1)) + from;
}

bool time_determinator::build_daily_restrictions( 
    const time_point from,
    const time_point to,
    im_t & interval_map,
    int64_t iteration_day)
    const noexcept
{
    const auto &rest = task_->get_restrictions();
    vector<json_interval> restrictions_interval = rest.get_all_from_to();
    
    time_point day_from = get_current_day_begin(iteration_day, from);

    for (auto &_24_restriction_interval : restrictions_interval)
    {
        auto normalized_intervals = _24_hour_interval_to_time_point(_24_restriction_interval, day_from);

        for_each(normalized_intervals.begin(), normalized_intervals.end(), [&]( time_point_interval & normalized_interval ){

            task_t dummy_task = std::make_shared<task>();
            dummy_task->set_description( "dummy_day_" + to_string(iteration_day) 
                + "_" 
                + _24_restriction_interval.restriction_name
                + "_" 
                + normalized_interval.tag);
            time_point start = normalized_interval.from ;
            time_t start_ = system_clock::to_time_t(start);

            time_point end   = normalized_interval.to ;
            time_t end_ = system_clock::to_time_t(end);
            dummy_task->set_interval(start_, end_);
            interval_map.set(make_pair(time_interval::closed(start_, end_), dummy_task ));
        } );
    }

    return true;
}

bool time_determinator::forward_pipeline(const im_t & interval_map, time_point current_day_begin) {
//Restrictions apply before the pipeline 
//now that restrictions are apply, time to check if there is slot
    auto slot = check_within_day_slot(interval_map ,current_day_begin);
    if( slot.has_value() ){
        return apply_slot(slot.value());
    }
    print_hour(current_day_begin);
    print_time(interval_map);
    SPDLOG_ERROR("Unable to find slot");
    return false;
}

bool time_determinator::when_pipeline( const im_t & interval_map, time_point current_day_begin) {
    const auto & when_ = task_->get_when();
    auto prev_task_ = sche_.get_task(when_.after, this->m_designated_period_group); // this is colliding
    if (!prev_task_)
    {
        SPDLOG_ERROR("Unable to find previous task to take reference: {}", when_.after);
        return false;
    }
    
    // time_point should_start = system_clock::from_time_t(prev_task_->get_interval().end) + seconds(1);
    
    current_after_t after{
        .prev_task = prev_task_,
        .offset_min = task_->get_when().minimum_delay.m_duration,
        .offset_max = task_->get_when().maximum_delay.m_duration,
    };
    
    auto slot = check_within_day_slot( interval_map ,current_day_begin , &after );
    if( slot.has_value() ){
        return apply_slot(slot.value());
    }
    // print here current map to check why it failed

    print_hour(current_day_begin);
    print_time(interval_map);
    SPDLOG_ERROR("Unable to find slot");
    return false;
}

bool time_determinator::apply_slot(time_point start) noexcept{
    //Just need to set it in the own task, the rest is handled outside
    seconds duration = task_->get_duration().m_duration;
    time_point end = start + duration;
    time_t start_ = system_clock::to_time_t(start)+1;
    time_t end_ = system_clock::to_time_t(end)+1;
    //cout << "Allocating in: " << endl << ctime(&start_) << ctime(&end_) << endl;
    task_->set_interval( start_, end_);
    if(!sche_.add_single(move(this->task_))){
        SPDLOG_ERROR("Unable to apply slot, probably colliding in scheduler, by policy: {}", sche_.policy);
        return false;
    }
    return true;
}

bool time_determinator::find_time_gap(time_t prev_upper, time_t current_lower, seconds duration_ ) const noexcept{
    time_t gap = current_lower - prev_upper;
    //return duration_.count() < gap;
    if(duration_.count() < gap){
        // cout << "Gap between :" << endl;
        // cout << ctime(&prev_upper ) << ctime(&current_lower ) << endl;
        return true;
    }else{

        return false;
    }
}

optional<time_point> time_determinator::check_within_day_slot(const im_t & interval_map, time_point day_to_search_in, const current_after_t * after) const noexcept{

    time_t end_of_day = system_clock::to_time_t( day_to_search_in + days(1) );
    time_t computed_start = system_clock::to_time_t( day_to_search_in );
    time_t computed_end = end_of_day;
    if (after)
    {
        computed_start = after->prev_task->get_interval().end + after->offset_min.count();
        if( after->offset_max.count() != 0 ){
            computed_end = after->prev_task->get_interval().end + after->offset_max.count();
        }
    }

    // if(task_->get_tag() == "perform_activity"){
    //     print_time(interval_map);
    // }
    //First check for upper bound of the beggin of the day... with this we find if exists place
    //Then we need to do lower_bound from result of valid upper_bound + duration of task
    seconds duration = task_->get_duration().m_duration;

    //This returns the first iterator after the time of search
    auto it_ = interval_map.lower_bound(interval_t::closed(computed_start , computed_start));
    time_t prev_time_upper = computed_start;
    // 00:00|-------------|23:59
    // TODO equal range instead of lower_bound unitl end_of_day
    for (auto it = it_; it != interval_map.end(); ++it )//Shouln't be interval_map end, should be get range iterator and only iterate over the same day
    {
        task_t match = it->second;
        auto current_it_interval = it->first;

        //Order matters!!
        if(current_it_interval.lower() > end_of_day){// Free space today? No? Skip to next day
            //return nullopt;
            break;
        }
        
        if(current_it_interval.upper() <= computed_start){//Means today should be free, just 1 check or die
            //This means it was the last
            if(find_time_gap(prev_time_upper, computed_end, duration)){
                return system_clock::from_time_t(prev_time_upper);
            }
            return nullopt;
        }
        
        if(find_time_gap(prev_time_upper, current_it_interval.lower(), duration)){ // iterate by every gap between tasks
            return system_clock::from_time_t(prev_time_upper);
        }

        prev_time_upper = current_it_interval.upper();//The current upper is the next lower
    }
    //Check until end of day not only current task
    //So here we are limiting to start and end within the same day, but we can start in and end in the next
    //prev_time_upper is garanteed to be within the same day

    if(!after || this->wildcard_data.has_value()){//If we got a when structure we skip other restrictions
        if(find_time_gap_edge(prev_time_upper, interval_map, duration, day_to_search_in)){
            //cout << "edge" << endl;
            return system_clock::from_time_t(prev_time_upper);
        }
    }
    //From here we have to iterate to sum gap between iterations and get the size of the gap
    //Until find gap or fail if bigger that the day
    return nullopt;
}

//policy allow edge or not
bool time_determinator::find_time_gap_edge(time_t prev_upper, const im_t & interval_map, seconds duration_, time_point today ) const{
    //1º check starts within same day
    if (!within_same_day(time_point(seconds(prev_upper)),today)){
        return false;
    }
    //2º get next task
    auto it_ = interval_map.upper_bound(interval_t::closed(prev_upper , prev_upper));
    if(it_ == interval_map.end()){
        return true;
    }
    task_t match = it_->second;
    auto current_it_interval = it_->first;
    //Is getting the last lowest but we dont know if is the last... we need to check last o end
    if(find_time_gap(prev_upper, current_it_interval.lower(),duration_)){
        return true;
    }
    return false;
}

bool time_determinator::build_elapsed_today_restriction(days local_start_offset, const time_point & day_from , im_t & interval_map){
    if(local_start_offset.count() != 0){
        return false;
    }

    task_t dummy_task = std::make_shared<task>();
    dummy_task->set_description( "elapsed_first_day_0_within");
    time_point start =  day_from;
    time_t start_ = system_clock::to_time_t(start);

    time_point end   = system_clock::now() + minutes(15) ;
    time_t end_ = system_clock::to_time_t(end);
    dummy_task->set_interval(start_, end_);
    interval_map.set(make_pair(time_interval::closed(start_, end_), dummy_task ));
    return true;
}


