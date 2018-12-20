#pragma once

#ifdef WIN32
#define HAVE_STRUCT_TIMESPEC
#endif 
#include <pthread.h>
#include <deque>
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>

#include "Global.h"

using namespace std;

//template<class TClass>
class Task
{
public:
  Task(void (*fn_ptr)(void*), void* arg); 
  ~Task();
  void operator()();
  void run();
private:
  void (*m_fn_ptr)(void*);
  void* m_arg;
};
