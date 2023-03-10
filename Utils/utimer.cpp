#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

#define START(timename) auto timename = chrono::system_clock::now();
#define STOP(timename,elapsed)  auto elapsed = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - timename).count();


class utimer {
  chrono::system_clock::time_point start;
  chrono::system_clock::time_point stop;
  string message; 
  using usecs = chrono::microseconds;
  using msecs = chrono::milliseconds;

private:
  long * us_elapsed;
  short ThreadId = -1;   
public:

  utimer(const string m) : message(m),us_elapsed((long *)NULL) {
    start = chrono::system_clock::now();
  }
    
  utimer(const string m, long * us) : message(m),us_elapsed(us) {
    start = chrono::system_clock::now();
  }

  utimer(long * us, short threadId) : us_elapsed(us), ThreadId(threadId) {
    start = chrono::system_clock::now();
  }

  ~utimer() {
    stop =
      chrono::system_clock::now();
    chrono::duration<double> elapsed =
      stop - start;
    auto musec =
      chrono::duration_cast<chrono::microseconds>(elapsed).count();
    
    if (ThreadId == -1){
      cout << message + " computed in " + to_string(musec) + " usec\n";
    }else
      cout << "Thread " + to_string(ThreadId) + " computed in " + to_string(musec) + " usec\n";

    if(us_elapsed != NULL)
      (*us_elapsed) = musec;
  }
};