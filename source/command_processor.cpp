//
// Created by carlos on 2/03/19.
//

#include "command_processor.h"

using namespace std;

bool command_processor::params_sanity(vector<string> s, map<char, string> &correspondance)
{
    if (s.size() <= 0)
    {
        return false;
    }
    if (s[0][0] == '-')
    {
        int len = s[0].size();

        auto check_correspondance_seq = [&s, &correspondance]() {
            char last_mapped = ' ';
            for (auto e : s)
            {
                if (e[0] == '-')
                {
                    //e is param indicator
                    last_mapped = e[1];
                }
                else
                {
                    //e is param
                    correspondance[last_mapped] = e;
                }
            }
        };

        auto check_correspondance_bch = [&s, &correspondance]() -> bool {
            //safety check
            if (s[0].size() - 1 != s.size() - 1)
            {
                return true;
            }
            for (size_t i = 1; i < s.size(); ++i)
            {
                correspondance[s[0][i]] = s[i];
            }
            return false;
        };

        if (len - 2)
        { //Means 0 sequential,
            if (check_correspondance_bch())
            {
                cout << "Not matching args, be sure your args match provided indicators" << endl;
                return true;
            }
            return false;
        }
        else
        {
            //p("Sequential");
            check_correspondance_seq();
            return false;
        }
    }
    else
    {
        cout << "Please provide arguments in batches: {-eAh eee 'AAAAParam' hhhh} or sequentially {-e eee -A 'AAA' -h hhh}  " << endl;
        return true;
    }
}

//Refactor to handle the calls in the own iactionables
void command_processor::route_positional_param(vector<string> args)
{
    if (args.size() > 0)
    {
        //Means that at leas we got relative path and a param, lets check if it is valid
        auto it = positional_params.end();
        it = positional_params.find(args[0]);
        if (it != positional_params.end())
        {
            //Good! then enroute it!
            if (it->second.action != nullptr)
            {
                it->second.action();
                return;
            }
        }
        else if (actioners.find(args[0]) != actioners.end())
        {
            map<char, string> correspondance;
            if (args.size() < 2)
            {
                cout << "Provide complementary params" << endl;
                return;
            }
            if (!params_sanity(vector<string>(args.begin() + 2, args.end()), correspondance))
            {
                actioners[args[0]]->send_action(args[1], correspondance);
                return;
            }
        }
        else
        {
            cout << "Wrong Positional param, --help to se options" << endl;
        }
    }
    else
    {
        cout << "Please provide a correct positional param, --help to se options" << endl;
    }
}

void command_processor::perform_command(string s)
{
    auto result = boost::program_options::split_unix(s);
    route_positional_param(result);
}
void command_processor::perform_command(vector<string> s)
{
    route_positional_param(s);
}

void command_processor::register_actionable(string named_by, iactionable *ac)
{
    actioners[named_by] = ac;
}
