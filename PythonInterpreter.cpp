#include "PythonInterpreter.h"

#include <Python.h>
#include <fmt/color.h>
#include <fmt/printf.h>

RootPythonInterpreter* PythonInterpreter::s_rootInterpreter = nullptr;


RootPythonInterpreter::RootPythonInterpreter()
{
   Py_Initialize();
   m_mainThreadState = Py_NewInterpreter();
   if (m_mainThreadState == nullptr)
   {
      fmt::print(fmt::fg(fmt::color::red), "Failed to create the main interpreter\n");
   }
   fmt::print("main thread state: {0}\n", (void*)m_mainThreadState);

   PyRun_SimpleString("print('hello python')");
}


RootPythonInterpreter::~RootPythonInterpreter()
{
   Py_FinalizeEx();
}


PythonInterpreter::PythonInterpreter()
{
   if (s_rootInterpreter == nullptr)
   {
      fmt::print(fmt::fg(fmt::color::red), "Python not initialized !\n");
      return;
   }
   
   static PyInterpreterConfig py_config = {
        .use_main_obmalloc = 0,
        .allow_fork = 0,
        .allow_exec = 0,
        .allow_threads = 1,
        .allow_daemon_threads = 0,
        .check_multi_interp_extensions = 1,
        .gil = PyInterpreterConfig_OWN_GIL,
   };
   const PyThreadState* state1 = nullptr; // PyThreadState_Get();
   PyStatus status = Py_NewInterpreterFromConfig(&m_threadState, &py_config);
   const PyThreadState* state2 = PyThreadState_Get();
   fmt::print("new thread state: {0}\n", (void*)m_threadState);
   fmt::print("new thread state1: {0} state2: {1}\n", (void*)state1, (void*)state2);
   
}


PythonInterpreter::~PythonInterpreter()
{
}


void PythonInterpreter::initialize()
{
   if (s_rootInterpreter != nullptr)
   {
      fmt::print(fmt::fg(fmt::color::green), "Python already initialized.\n");
      return;
   }

   s_rootInterpreter = new RootPythonInterpreter();
   
}


int PythonInterpreter::runSimpleString(const char* _pythonCode)
{
   PyThreadState* prevState = PyThreadState_Get();
   const bool differentState = (prevState != m_threadState);

   if (differentState)
   {
      PyThreadState_Swap(m_threadState);
   }
   
   int result = PyRun_SimpleString(_pythonCode);
   
   if (differentState)
   {
      PyThreadState_Swap(prevState);
   }
   return result;
}
