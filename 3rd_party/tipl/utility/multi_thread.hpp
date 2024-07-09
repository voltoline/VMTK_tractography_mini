#ifndef MULTI_THREAD_HPP
#define MULTI_THREAD_HPP
#include <future>
namespace tipl{

class time
{
 public:
    time():  t1(std::chrono::high_resolution_clock::now()){};
    template<typename T> // T: std::chrono::milliseconds
    double elapsed(){return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now() - t1).count();}
    void restart(){t1 = std::chrono::high_resolution_clock::now();}
    void start(){t1 = std::chrono::high_resolution_clock::now();}
    void stop(){t2 = std::chrono::high_resolution_clock::now();}
    double total(){stop();return std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();}
    ~time(){}
 private:
    std::chrono::high_resolution_clock::time_point t1, t2;
};

template <class T,class Func>
void par_for(T size, Func f, int thread_count = std::thread::hardware_concurrency())
{
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = int(size);
    for(int id = 1; id < thread_count; id++)
    {
        futures.push_back(std::move(std::async(std::launch::async, [id,size,thread_count,&f]
        {
            for(T i = id; i < size; i += thread_count)
                f(i);
        })));
    }
    for(T i = 0; i < size; i += thread_count)
        f(i);
    for(auto &future : futures)
        future.wait();
}

template <class T,class Func>
void par_for_asyn(T size, Func f, int thread_count = std::thread::hardware_concurrency())
{
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = int(size);
    T now = 0;
    std::mutex read_now;
    for(int id = 1; id < thread_count; id++)
    {
        futures.push_back(std::move(std::async(std::launch::async, [id,size,thread_count,&f,&now,&read_now]
        {
            while(now < size)
            {
                T i;
                {
                    std::lock_guard<std::mutex> lock(read_now);
                    i = now;
                    ++now;
                }
                f(i);
            }
        })));
    }
    while(now < size)
    {
        T i;
        {
            std::lock_guard<std::mutex> lock(read_now);
            i = now;
            ++now;
        }
        f(i);
    }
    for(auto &future : futures)
        future.wait();
}


template <class T,class Func>
void par_for2(T size, Func f, unsigned int thread_count = std::thread::hardware_concurrency())
{
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = size;
    for(int id = 1; id < thread_count; id++)
    {
        futures.push_back(std::move(std::async(std::launch::async, [id,size,thread_count,&f]
        {
            for(T i = id; i < size; i += thread_count)
                f(i,id);
        })));
    }
    for(T i = 0; i < size; i += thread_count)
        f(i,0);
    for(auto &future : futures)
        future.wait();
}

template <class T,class Func>
void par_for_asyn2(T size, Func f, int thread_count = std::thread::hardware_concurrency())
{
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = int(size);
    T now = 0;
    std::mutex read_now;
    for(int id = 1; id < thread_count; id++)
    {
        futures.push_back(std::move(std::async(std::launch::async, [id,size,thread_count,&f,&now,&read_now]
        {
            while(now < size)
            {
                T i;
                {
                    std::lock_guard<std::mutex> lock(read_now);
                    i = now;
                    ++now;
                }
                f(i,id);
            }
        })));
    }
    while(now < size)
    {
        T i;
        {
            std::lock_guard<std::mutex> lock(read_now);
            i = now;
            ++now;
        }
        f(i,0);
    }
    for(auto &future : futures)
        future.wait();
}
template <class T,class Func>
void par_for_block(T size, Func f, int thread_count = std::thread::hardware_concurrency())
{
    if(!size)
        return;
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = size;

    size_t block_size = size/thread_count;
    size_t pos = 0;
    for(int id = 1; id < thread_count; id++)
    {
        size_t end = pos + block_size;
        futures.push_back(std::move(std::async(std::launch::async, [pos,end,&f]
        {
            for(size_t i = pos; i < end;++i)
                f(i);
        })));
        pos = end;
    }
    for(size_t i = pos; i < size;++i)
        f(i);
    for(auto &future : futures)
        future.wait();
}

template <class T,class Func>
void par_for_block2(T size, Func f, int thread_count = std::thread::hardware_concurrency())
{
    if(!size)
        return;
    std::vector<std::future<void> > futures;
    if(thread_count > size)
        thread_count = size;

    size_t block_size = size/thread_count;
    size_t pos = 0;
    for(int id = 1; id < thread_count; id++)
    {
        size_t end = pos + block_size;
        futures.push_back(std::move(std::async(std::launch::async, [id,pos,end,&f]
        {
            for(size_t i = pos; i < end;++i)
                f(i,id);
        })));
        pos = end;
    }
    for(size_t i = pos; i < size;++i)
        f(i,0);
    for(auto &future : futures)
        future.wait();
}


class thread{
private:
    std::shared_ptr<std::future<void> > th;
    bool started;
public:
    bool terminated;
public:
    thread(void):started(false),terminated(false){}
    ~thread(void){clear();}
    void clear(void)
    {
        if(th.get())
        {
            terminated = true;
            th->wait();
            th.reset();
        }
        terminated = false;
        started = false;
    }

    template<class lambda_type>
    void run(lambda_type&& fun)
    {
        if(started)
            clear();
        started = true;
        th.reset(new std::future<void>(std::async(std::launch::async,fun)));
    }
    void wait(void)
    {
        th->wait();
    }
    bool has_started(void)const{return started;}
};


}
#endif // MULTI_THREAD_HPP

