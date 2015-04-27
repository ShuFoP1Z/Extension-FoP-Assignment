//Routines that get the system date and time and produce them as a formatted strings

#include "TimeUtils.h"

void GetSystemTime( int& hrs, int& mins, int& secs)
{ //get time from system
	//set to system time
	time_t now = time(0);
	struct tm& t = *localtime(&now);
	hrs  = t.tm_hour ;
	mins = t.tm_min ;
	secs = t.tm_sec ;
}

string TimeToString( int h, int m, int s)
{ //convert the time to a string in 24-h digital clock format (00:00:00)
    ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << h << ":"
       << setw(2) << m << ":"
       << setw(2) << s;
    os.fill(prev);
    return ( os.str());
}

string GetTime() 
{ //return the current time in a string format
	int hrs, mins, secs;		//hold the current time
	GetSystemTime( hrs, mins, secs);
	return ( TimeToString( hrs, mins, secs));
}

void GetSystemDate( int& day, int& month, int& year)
{ //get date from system
	time_t ltime;
	struct tm *now;
    // Set time zone from TZ environment variable. If TZ is not set, the operating
    // system is queried to obtain the default value for the variable. 
    _tzset();
    // Get UNIX-style time. 
    time( &ltime );
    // Convert to time structure. 
	now =  localtime( &ltime );
    // Set Time objects members. 
	day = now->tm_mday;
	month = now->tm_mon + 1;
	year = now->tm_year + 1900;
}

string DateToString( int day, int month, int year) 
{ //convert the date to a string in format (dd/mm/yyyy)
	ostringstream os;
    const char prev(os.fill ('0'));
    os << setw(2) << day << "/"
       << setw(2) << month << "/"
       << setw(4) << year;
    os.fill(prev);
    return ( os.str());
}

string GetDate() 
{ //return the current date in a string format
	int day, month, year;		//hold the current date
	GetSystemDate( day, month, year);
	return ( DateToString( day, month, year));
}
