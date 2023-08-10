#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <queue>
#include <pthread.h>
#include <iostream>

// 定义任务结构体
using callback = void(*)(void*);
using namespace std;
struct Task
{
    Task()
    {
        function = nullptr;
        arg = nullptr;
    }
    Task(callback f, void* arg)
    {
        function = f;
        this->arg = arg;
    }
    callback function;
    void* arg;
};

// 任务队列---------------------------------------------------------------------
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task& task);
    void addTask(callback func, void* arg);

    // 取出一个任务
    Task takeTask();

    // 获取当前队列中任务个数
    inline int taskNumber()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;    // 互斥锁
    queue<Task> m_queue;   // 任务队列
};

inline TaskQueue::TaskQueue()
{
    pthread_mutex_init(&m_mutex, NULL);
}

inline TaskQueue::~TaskQueue()
{
    pthread_mutex_destroy(&m_mutex);
}

inline void TaskQueue::addTask(Task& task)
{
    pthread_mutex_lock(&m_mutex);
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

inline void TaskQueue::addTask(callback func, void* arg)
{
    pthread_mutex_lock(&m_mutex);
    Task task;
    task.function = func;
    task.arg = arg;
    m_queue.push(task);
    pthread_mutex_unlock(&m_mutex);
}

inline Task TaskQueue::takeTask()
{
    Task t;
    pthread_mutex_lock(&m_mutex);
    if (m_queue.size() > 0)
    {
        t = m_queue.front();
        m_queue.pop();
    }
    pthread_mutex_unlock(&m_mutex);
    return t;
}

//线程池------------------------------------------------------------------
class ThreadPool
{
public:
    ThreadPool(int num);
    ~ThreadPool();

    // 添加任务
    void addTask(Task task);

private:
    // 工作的线程的任务函数
    static void* worker(void* arg);

private:
    pthread_mutex_t lock;     
    pthread_cond_t notEmpty;  //判断是否任务是否为空
    TaskQueue *taskQ; // 任务队列
    int Allnum; //线程池的线程个数
    int shutdown = 0;
};

inline ThreadPool::ThreadPool(int num)
{
    //实例化任务队列
    taskQ = new TaskQueue;
    do{
        //初始化线程池
        int worknum = 0;
        if(pthread_mutex_init(&lock,NULL) !=0 || pthread_cond_init(&notEmpty,NULL)!=0)
        {  
            break;
        }

        //创建线程
        for (int i = 0; i < num;i++)
        {
            pthread_t tid;
            pthread_create(&tid, NULL, worker, this);
            pthread_detach(tid);
        }

    } while (0);
}

inline ThreadPool::~ThreadPool()
{
    shutdown = 1;
 
    // 唤醒所有消费者线程
    for (int i = 0; i < Allnum; ++i)
    {
        pthread_cond_signal(&notEmpty);
    }

    if (taskQ) delete taskQ;;
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&notEmpty);
}

inline void ThreadPool::addTask(Task task)
{
    if(shutdown)
        return;
    taskQ->addTask(task);
    pthread_cond_signal(&notEmpty);
}

inline void * ThreadPool::worker(void * arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);

    while(1)
    {
        pthread_mutex_lock(&pool->lock);

        //判断任务队列是否为空
        while(!pool->taskQ->taskNumber()&&!pool->shutdown)
        {
            pthread_cond_wait(&pool->notEmpty, &pool->lock);
        }
        
        //判断线程池是否关闭
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        //取出一个线程
        Task task = pool->taskQ->takeTask();
        pthread_mutex_unlock(&pool->lock);
        task.function(task.arg);
    }
    return nullptr;
}

#endif