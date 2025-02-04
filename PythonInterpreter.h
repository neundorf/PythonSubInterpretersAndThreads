#pragma once

#include <Python.h>


class RootPythonInterpreter
{
   public:
      RootPythonInterpreter();
      ~RootPythonInterpreter();

   private:
      PyThreadState* m_mainThreadState = nullptr;
};


class PythonInterpreter
{
   public:
      PythonInterpreter();
      ~PythonInterpreter();

      int runSimpleString(const char* _pythonCode);
      
      static void initialize();

   private:
      PyThreadState *m_threadState = nullptr;
      static RootPythonInterpreter* s_rootInterpreter;
};

