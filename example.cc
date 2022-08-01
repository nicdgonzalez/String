/**
 * NOTE:
 * This is an executable version of the example found in the README file.
 * Refer to the *README.rst* file.
 */

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

  // Example Output: 20220702.122421.FATAL.log
  cout << (log_file_fmt % date_time % "FATAL") << endl;

  return 0;
}
