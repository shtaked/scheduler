//
//  Scheduler.h
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#ifndef SHEDULER_H
#define SHEDULER_H

#include <list>
#include <map>

#include "JobsLine.h"
#include "ResourcesLine.h"

namespace scheduler {
    
    //Imlements Scheduler logic
    //Listenes for both Jobs and Resources line
    class Scheduler :
        public communication::JobsLineListener,
        public communication::ResourcesLineListener
    {
    public:
        Scheduler();
        virtual ~Scheduler();
        
        void start();
        void stop();
        
    private:
        //this to callbacks are called from Lines threads
        virtual void onJobReceived(const common::Job &job) override;
        virtual void onResourceReceived(const common::Resource &resource) override;
        
        //method tries to find free resources for this job
        //returns true if success, false otherwise
        bool findResource(const common::Job &job);

    private:
        //lines
        communication::JobsLine jobs_line_;
        communication::ResourcesLine resources_line_;

        //queue for jobs
        common::Jobs jobs_queue_;
        //mutex that help you access the queue from different threads
        pthread_mutex_t jobs_mutex_;
        
        //resource queue is implemented as queue of free node numbers
        //and map where key is node number - and value is resource info
        typedef ::std::list<::common::Resource::NodeNumber> NodeNumbersQueue;
        typedef ::std::map<::common::Resource::NodeNumber, ::common::Resource> ResourceMap;
        
        NodeNumbersQueue node_numbers_queue_;
        ResourceMap resource_map_;
        pthread_mutex_t resources_mutex_;
        
        //condition variables - signals about new messages
        pthread_cond_t new_resources_;
        pthread_cond_t new_job_;
    };
    
}   //namespace scheduler

#endif //SHEDULER_H
