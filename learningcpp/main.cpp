#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>

using namespace std;

class Date {
public:
    Date(int year_, int month_, int day_) {
        year  = year_;
        if (month_ < 1 or month_ > 12)
        {
            throw out_of_range("Month value is invalid: " + to_string(month_));
        } else month = month_;
        if (day_ < 1 or day_ > 31)
        {
            throw out_of_range("Day value is invalid: " + to_string(day_));
        } else day = day_;
    }
    int GetYear()  const { return year;  }
    int GetMonth() const { return month; }
    int GetDay()   const { return day;   }
private:
    int year  = 0;
    int month = 0;
    int day   = 0;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear()){
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else {
        return lhs.GetDay() < rhs.GetDay();
    }
}

ostream& operator << (ostream& stream, const Date date) {
    stream << setfill('0') << setw(4) << date.GetYear();
    stream << '-';
    stream << setfill('0') << setw(2) << date.GetMonth();
    stream << '-';
    stream << setfill('0') << setw(2) << date.GetDay();
    return stream;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event)
    {
        if (events.count(date) > 0 and events[date].count(event) > 0)
        {
            return;
        }
        events[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event)
    {
        if (events[date].count(event))
        {
            events[date].erase(event);
            return true;
        }
        return false;
    }
    int DeleteDate(const Date& date)
    {
        int deleted = 0;
        if (events.count(date))
        {
            deleted = events.at(date).size();
            events.erase(date);
        }
        return deleted;
    }
    
    void Find(const Date& date) const
    {
        if (events.count(date))
        {
            for (const auto& item : events.at(date))
            {
                cout << item << endl;
            }
        }
    }
    
    void Print() const
    {
        for (const auto& [key, val] : events)
        {
            for (const auto& e : val)
            {
                cout << key << ' ' << e << endl;
            }
        }
    }
private:
    map<Date, set<string>> events;
};

Date ParseDate(const string& string_date)
{
    istringstream stream(string_date);
    bool state = true;
    
    int year;
    state = state and (stream >> year);
    state = state and (stream.peek() == '-');
    stream.ignore(1);
    
    int month;
    state = state and (stream >> month);
    state = state and (stream.peek() == '-');
    stream.ignore(1);
    
    int day;
    state = state and (stream >> day);
    state = state and (stream.eof());
    
    if (!state) throw domain_error("Wrong date format: " + string_date);
    
    return Date(year, month, day);
}

int main()
{
    try
    {
        Database db;
        
        string command;
        while (getline(cin, command))
        {
            stringstream input(command);
            string operation;
            input >> operation;
            
            if (operation == "Add")
            {
                string string_date, event;
                input >> string_date >> event;
                Date date = ParseDate(string_date);
                db.AddEvent(date, event);
            }
            else if (operation == "Del")
            {
                string string_date, event;
                input >> string_date >> event;
                Date date = ParseDate(string_date);
                if (!event.empty()) {
                    string res = db.DeleteEvent(date, event) ? "Deleted successfully" : "Event not found";
                    cout << res << endl;
                }
                else
                {
                    cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
                }
            }
            else if (operation == "Find")
            {
                string string_date;
                input >> string_date;
                Date date = ParseDate(string_date);
                db.Find(date);
            }
            else if (operation == "Print")
            {
                db.Print();
            }
            else if (operation != "" and operation != " ")
            {
                cout << "Unknown command: " << operation << endl;
            }
        }
    } catch (exception& ex)
    {
        cout << ex.what() << endl;
    }
    
    return 0;
}
