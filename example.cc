#include <iostream>

#include <cstdio>
#include <ctime>

#include <ndg/string.hh>

using std::time_t;
using std::time;
using std::tm;
using std::localtime;
using std::strftime;
using std::cout;
using std::endl;

int main() {
  ndg::String log_file_fmt = "{}.{}.log";

  char date_time[17];
  time_t t = time(&t);
  struct tm *time_info = localtime(&t);
  strftime(
    date_time,
    sizeof(date_time),
    "%Y%m%d.%H%M%S",
    time_info
  );

  cout << (log_file_fmt % date_time % "FATAL") << endl;

  return 0;
}