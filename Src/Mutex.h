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

class Mutex
{
public:
  Mutex();
  ~Mutex();
  void lock();
  void unlock();
  pthread_mutex_t* get_mutex_ptr();

private:
  pthread_mutex_t m_lock;
  volatile bool is_locked;
};
