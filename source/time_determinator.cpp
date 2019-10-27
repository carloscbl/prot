#include "time_determinator.h"
#include "time_determinator.h"
#include "task_frequency.h"
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_map.hpp>


time_determinator::time_determinator(task_t task_, scheduler &sche_) : task_(task_), sche_(sche_)
{
}

bool time_determinator::build()
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
    const time_point end = this->task_->get_frequency().get_period() + system_clock::now();
    const time_point start = floor<days>(system_clock::now());
    using pipeline_t = function<bool(time_determinator *,const im_t &, time_point) >;
    pipeline_t pipeline = &time_determinator::forward_pipeline;

    auto when_ = this->task_->get_when();
    if(!when_.after.empty()){
        pipeline = &time_determinator::when_pipeline;
    }

    //when_
    //For each day Apply restrictions
    im_t interval_map = sche_.clone_interval_map();
    //We need to traverse days within the first interval, but we need a policy to know
    // TODO: Policy when we pass the max frequiency range
    // TODO: How to move tasks that are not compatible with the current scheduler


    days d = ceil<days>(end - start); 
    for (days::rep iteration_day = 0; iteration_day < d.count(); iteration_day++)
    {
        build_daily_restrictions(start, end, interval_map, iteration_day );
        if(pipeline(this, interval_map, get_current_day_begin(iteration_day, start) )) 
        {
            return true;
        }
    }

    return false;
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
            dummy_task->id = "dummy_day_" + to_string(iteration_day) 
                + "_" 
                + _24_restriction_interval.restriction_name
                + "_" 
                + normalized_interval.tag;
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
    // TODO check when!!!
    // TODO pack and order group tasks
    auto slot = check_within_day_slot(interval_map ,current_day_begin);
    if( slot.has_value() ){
        apply_slot(slot.value());
        return true;
    }
    return false;
}

bool time_determinator::when_pipeline( const im_t & interval_map, time_point current_day_begin) {
    const auto & when_ = task_->get_when();
    auto range = sche_.get_range(system_clock::to_time_t(current_day_begin), system_clock::to_time_t(current_day_begin + days(1)));
    if (!range.has_value())
    {
        return false;
    }
    
    auto prev_task = std::find_if(range.value().begin(),range.value().end(),[&when_](task_t t)-> bool{ return when_.after == t->get_tag() ? true : false; }); 
    if( prev_task == std::end(range.value())){ return false;}
    task_t prev_task_ = (*prev_task);
    time_point should_start = system_clock::from_time_t(prev_task_->get_interval().end) + seconds(1);
    
    current_after_t after{
        .prev_task = prev_task_,
        .offset_min = task_->get_when().minimum_delay.m_duration,
        .offset_max = task_->get_when().maximum_delay.m_duration,
    };
    
    auto slot = check_within_day_slot( interval_map ,current_day_begin , &after );
    if( slot.has_value() ){
        apply_slot(slot.value());
        return true;
    }
    return false;
}

void time_determinator::apply_slot(time_point start) noexcept{
    //Just need to set it in the own task, the rest is handled outside
    seconds duration = task_->get_duration().m_duration;
    time_point end = start + duration;
    time_t start_ = system_clock::to_time_t(start)+1;
    time_t end_ = system_clock::to_time_t(end)+1;
    cout << "Alocating in: " << endl << ctime(&start_) << ctime(&end_) << endl;
    task_->set_interval( start_, end_);
    sche_.add_single(move(this->task_));
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

    if(this->task_->get_tag() == "industrial_middle")
    {
        print_time(interval_map);
    }
    //First check for upper bound of the beggin of the day... with this we find if exists place
    //Then we need to do lower_bound from result of valid upper_bound + duration of task
    seconds duration = task_->get_duration().m_duration;

    //This returns the first iterator after the time of search
    auto it_ = interval_map.lower_bound(interval_t::closed(computed_start , computed_start));
    time_t prev_time_upper = computed_start;
    for (auto it = it_; it != interval_map.end(); ++it )//Shouln't be interval_map end, should be get range iterator and only iterate over the same day
    {
        task_t match = it->second;
        auto it_interval = it->first;
        if(it_interval.lower() > end_of_day){
            return nullopt;
        }
        if(it_interval.upper() <= computed_start){
            //This means it was the last
            if(find_time_gap(prev_time_upper, computed_end, duration)){
                return system_clock::from_time_t(prev_time_upper);
            }
            return nullopt;
        }
        
        if(find_time_gap(prev_time_upper, it_interval.lower(), duration)){
            return system_clock::from_time_t(prev_time_upper);
        }
        prev_time_upper = it_interval.upper();
    }
    //Check until end of day not only current task
    if(find_time_gap(prev_time_upper, computed_end, duration)){
        return system_clock::from_time_t(prev_time_upper);
    }

    //From here we have to iterate to sum gap between iterations and get the size of the gap
    //Until find gap or fail if bigger that the day
    return nullopt;
}
