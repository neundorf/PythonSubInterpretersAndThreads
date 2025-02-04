#include "PythonInterpreter.h"

#include <fmt/color.h>
#include <fmt/printf.h>

#include <atomic>
#include <thread>

std::atomic<bool> g_stop = false;


void loop(int incr)
{
   std::string code = fmt::format("i=i+{}\nprint(f\"i={{i}}\")\n", incr);
   PythonInterpreter py;
   py.runSimpleString("i=0");
   while (!g_stop)
   {
      py.runSimpleString(code.c_str());
   }
}



int main()
{
   PythonInterpreter::initialize();
   
   PythonInterpreter py1;
   
   const char* c1 = R"(i=12
print(f"-------- i= {i}")
)";
   
   py1.runSimpleString(c1);

   PythonInterpreter py2;
   
   const char* c2 = R"(i=1
print(f"2-------- i= {i}")
)";

   const char* c3 = R"(i=i+1
print(f"3-------- i= {i}")
)";
   
   py2.runSimpleString(c2);
   
   py1.runSimpleString(c3);
   py2.runSimpleString(c3);

   std::thread t1(loop, 1);
   std::thread t2(loop, -2);
   
   const time_t endTime = time(0) + 5;
   while (time(0) < endTime)
   {
      sleep(1);
   }
   g_stop = true;
   
   t1.join();
   t2.join();
   
   return 0;
}
