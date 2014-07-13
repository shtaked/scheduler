//
//  Common.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//
//  Here there are some base classes and templates
//  That can be used in different projects
//

#ifndef COMMON_H
#define COMMON_H

#include <sstream>
#include <queue>
#include <pthread.h>

namespace common {
    
    struct Job
    {
        typedef uint64_t JobId;
        
        JobId id;
        unsigned units_count;
        unsigned steps_count;
    };
    
    inline ::std::ostream& operator<<(::std::ostream& os, const Job& job)
    {
        os << "id (" << job.id << ") units: (" << job.units_count
            << ") steps: (" << job.steps_count << ")";
        return os;
    }

    typedef ::std::queue<Job> Jobs;
    
    
    struct Resource
    {
        typedef uint64_t NodeNumber;
        
        unsigned units_count;
        NodeNumber node_number;
    };
    
    inline ::std::ostream& operator<<(::std::ostream& os, const Resource& resource)
    {
        os << "node_number (" << resource.node_number << ") units: (" << resource.units_count << ")";
        return os;
    }
    
    typedef ::std::queue<Resource> Resources;
    
    //thread implementation
    class Thread
    {
    private:
        pthread_t thread;
        
        Thread(const Thread& copy);         // copy constructor denied
        static void *thread_func(void *d)   { ((Thread *)d)->run(); return NULL; }
        
    public:
        Thread()             {}
        virtual ~Thread()    {}
        
        virtual void run() = 0;
        
        int start()          { return pthread_create(&thread, NULL,
                                                     Thread::thread_func, (void*)this); }
        int wait ()          { return pthread_join  (thread, NULL); }
    };
    
    template<class T>
    class Singleton
    {
    public:
        static T &instance() {
            static T t;
            return t;
        }
        
    protected:
        Singleton() { }
        ~Singleton() { }
        
        Singleton(Singleton const&) = delete;
        Singleton& operator= (Singleton const&) = delete;
    };
    
    //logging text message (thread safe)
    //current implementation logs to console
    void log(const ::std::string &message);
    
    //line ids
    extern const ::std::string kJobsLineId;
    extern const ::std::string kResourceLineId;
    
}   //namespace common

#endif
