#pragma once

#ifdef WIN32
#define HAVE_STRUCT_TIMESPEC
#endif 
#include <pthread.h>
#include <deque>
#include <iostream>
#include <vector>

#include "Global.h"

using namespace std;

class CondVar {
public:

  CondVar();
  ~CondVar();
  void wait(pthread_mutex_t* mutex);
  void signal();
  void broadcast();

private:
  pthread_cond_t m_cond_var;
};
