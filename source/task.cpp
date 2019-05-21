#include "task.h"

using namespace std;

task::task():CRUD_actionable(this->tasks_map,setters),dateUTC(time(nullptr)){
  static int acummulator = 0;
  id = "task" + to_string(acummulator++);
}

task::~task(){
  cout << "~destroying " << this->id <<endl;
}

void task::remove(map<char,string>params){
  auto it = params.end();

  it = params.find('i');
  if(it != params.end()){
    env::tasker.erase(it->second);
  }else{
    cout << "please provide 'i' argument";
    return;
  }
}

void task::update(map<char,string>params){
  auto it = params.end();

  it = params.find('i');
  if(it != params.end()){
    auto & t = env::tasker[it->second];

    for(auto e : params){
      auto  et = setters.end();
      et = setters.find(e.first);
      if(et != setters.end()){
        setters[e.first](*t,e.second);
      }
    }
  }else{
    cout << "please provide 'i' argument";
    return;
  }
}

void task::print_remain(){
  double remain_sec = difftime(this->dateUTC ,time(nullptr) );
  //cout << remain_sec << endl;

  double years = remain_sec / (60*60*24*365);
  double days  =  modf(years,&years) * 365;
  double hours = modf(days,&days) * 24;
  double minutes = modf(hours,&hours) * 60;
  modf(minutes,&minutes);// * 60;// task add -nMDY pipo 0 4 2020

  cout << id << ":" << "remain" << ": " << minutes<< " minutes :" << hours << " hours :"<< (uint32_t)days<< " days :" <<(uint32_t)years<< " years" << endl;
}

void task::remain(map<char,string>params){
  auto it = params.end();
  it = params.find('i');
  if(it != params.end()){
    env::tasker[it->second]->print_remain();
  }else{
    for(auto & t : env::tasker){
      t.second->print_remain();
    }
  }
}

void task::send_action(std::string action, std::map<char,std::string> params){
  if(tasks_map.find(action) != tasks_map.end()){
    tasks_map[action](params);
  }else{
    cout << action << " :Does not match. Not provided correct arguments" <<endl;
  }
}

void task::set_day(string day){
  tm * tm_localtime = localtime(&dateUTC);
  tm_localtime->tm_mday = stoi(day);
  dateUTC = mktime(tm_localtime);
}

void task::set_month(string mon){
  tm * tm_localtime = localtime(&dateUTC);
  tm_localtime->tm_mon = stoi(mon)-1;
  dateUTC = mktime(tm_localtime);
}

void task::set_year(string year){
  tm * tm_localtime = localtime(&dateUTC);
  tm_localtime->tm_year = stoi(year)-1900;
  dateUTC = mktime(tm_localtime);
}

void task::set_hour(string hour){
  tm * tm_localtime = localtime(&dateUTC);
  tm_localtime->tm_hour = stoi(hour);
  dateUTC = mktime(tm_localtime);
}

void task::set_minute(string minute){
  tm * tm_localtime = localtime(&dateUTC);
  tm_localtime->tm_min = stoi(minute);
  dateUTC = mktime(tm_localtime);
}