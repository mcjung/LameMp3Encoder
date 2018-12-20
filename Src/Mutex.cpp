#include "Mutex.h"

Mutex::Mutex() {
  pthread_mutex_init(&m_lock, NULL);
  is_locked = false;
}

Mutex::~Mutex() {
  while(is_locked);
  unlock();
  pthread_mutex_destroy(&m_lock);
}

void Mutex::lock() {
  pthread_mutex_lock(&m_lock);
  is_locked = true;
}

void Mutex::unlock() {
  is_locked = false; 
  pthread_mutex_unlock(&m_lock);
}

pthread_mutex_t* Mutex::get_mutex_ptr(){
  return &m_lock;
}
