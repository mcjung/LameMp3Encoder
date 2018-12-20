#include "ThreadPool.h"

ThreadPool::ThreadPool() : m_pool_size(DEFAULT_POOL_SIZE)
{
  cout << "Constructed ThreadPool of size " << m_pool_size << endl;
}

ThreadPool::ThreadPool(int pool_size) : m_pool_size(pool_size)
{
  cout << "Constructed ThreadPool of size " << m_pool_size << endl;
}

ThreadPool::~ThreadPool()
{
  // Release resources
  if (m_pool_state != STOPPED) {
    destroy_threadpool();
  }
}

extern "C"
void* start_thread(void* arg)
{
  ThreadPool* tp = (ThreadPool*) arg;
  tp->execute_thread();
  return NULL;
}

int ThreadPool::initialize_threadpool()
{
  m_pool_state = STARTED;
  int ret = -1;
  for (int i = 0; i < m_pool_size; i++) {
    pthread_t tid;
    ret = pthread_create(&tid, NULL, start_thread, (void*) this);
    if (ret != 0) {
      cerr << "pthread_create() failed: " << ret << endl;
      return -1;
    }
    m_threads.push_back(tid);
  }
  cout << m_pool_size << " threads created by the thread pool" << endl;

  return 0;
}

int ThreadPool::destroy_threadpool()
{
  m_task_mutex.lock();
  m_pool_state = STOPPED;
  m_task_mutex.unlock();
  //cout << "Broadcasting STOP signal to all threads..." << endl;
  m_task_cond_var.broadcast(); 

  int ret = -1;
  for (int i = 0; i < m_pool_size; i++) {
    void* result;
    ret = pthread_join(m_threads[i], &result);
//    cout << "pthread_join() returned " << ret << ": " << strerror(errno) << endl;
    m_task_cond_var.broadcast(); 
  }
//  cout << m_pool_size << " threads exited from the thread pool" << endl;
  return 0;
}

void* ThreadPool::execute_thread()
{
  Task* task = NULL;
  while(true) {
    m_task_mutex.lock();
    
    while ((m_pool_state != STOPPED) && (m_tasks.empty())) {
      m_task_cond_var.wait(m_task_mutex.get_mutex_ptr());
    }

    if (m_pool_state == STOPPED && m_tasks.empty()) {
      m_task_mutex.unlock();
      pthread_exit(NULL);
    }

    task = m_tasks.front();
    m_tasks.pop_front();
    m_task_mutex.unlock();

    (*task)(); // could also do task->run(arg);
    delete task;
  }
  return NULL;
}

int ThreadPool::add_task(Task* task)
{
  m_task_mutex.lock();

  m_tasks.push_back(task);
  m_task_cond_var.signal();
  m_task_mutex.unlock();

  return 0;
}
