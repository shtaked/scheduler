//
//  Scheduler.cpp
//  Scheduler
//
//  Created by Eugene Koiro on 13.07.14.
//  Copyright (c) 2014 Eugene Koiro. All rights reserved.
//

#include "Scheduler.h"

#include <sstream>

namespace scheduler {
    
    Scheduler::Scheduler()
    {
        jobs_line_.addListener(this);
        resources_line_.addListener(this);
        
        pthread_mutex_init(&jobs_mutex_, nullptr);
        pthread_mutex_init(&resources_mutex_, nullptr);
        
        pthread_cond_init(&new_resources_, nullptr);
        pthread_cond_init(&new_job_, nullptr);
    }
    
    Scheduler::~Scheduler()
    {
        jobs_line_.removeListener(this);
        resources_line_.removeListener(this);
        
        pthread_mutex_destroy(&jobs_mutex_);
        pthread_mutex_destroy(&resources_mutex_);
        
        pthread_cond_destroy(&new_resources_);
        pthread_cond_destroy(&new_job_);
    }
    
    void Scheduler::start()
    {
        jobs_line_.open(common::kJobsLineId);
        resources_line_.open(common::kResourceLineId);
        
        //infinite loop for receiving jobs
        while (true) {
            pthread_mutex_lock(&jobs_mutex_);
            
            static pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
            
            //if jobs queue is empty
            //than we need to wait for new job messages
            if (jobs_queue_.empty()) {
                pthread_mutex_unlock(&jobs_mutex_);
                pthread_cond_wait(&new_job_, &cond_mutex);
                pthread_mutex_unlock(&cond_mutex);
            }
            
            unsigned long jobs_rest = 0;
            
            //jetting first job from queue
            common::Job job = jobs_queue_.front();
            jobs_queue_.pop();
            jobs_rest = jobs_queue_.size();
            
            pthread_mutex_unlock(&jobs_mutex_);
            
            //
            ::std::ostringstream out_stream;
            out_stream << "Job decueud for scheduling: " << job << ::std::endl;
            out_stream << "Jobs count in queue: " << jobs_rest << ::std::endl;
            common::log(out_stream.str());
            
            //trying to schedule it
            while (!findResource(job)) {
                //if there is no available resources
                //than wait for new
                pthread_cond_wait(&new_resources_, &cond_mutex);
                pthread_mutex_unlock(&cond_mutex);
            }
        }
    }
    
    void Scheduler::stop()
    {
        jobs_line_.close();
        jobs_line_.close();
    }
    
    void Scheduler::onJobReceived(const common::Job &job)
    {
        ::std::ostringstream out_stream;
        out_stream << "New job received: " << job << ::std::endl;
        common::log(out_stream.str());
        
        pthread_mutex_lock(&jobs_mutex_);
        
        //enqueue new job
        jobs_queue_.push(job);
        
        pthread_mutex_unlock(&jobs_mutex_);
        
        pthread_cond_signal(&new_job_);
    }
    
    void Scheduler::onResourceReceived(const common::Resource &resource)
    {
        ::std::ostringstream out_stream;
        out_stream << "New resource received: " << resource << ::std::endl;
        common::log(out_stream.str());
        
        pthread_mutex_lock(&resources_mutex_);
        
        //enqueue new rosources
        node_numbers_queue_.push_back(resource.node_number);
        resource_map_[resource.node_number].node_number = resource.node_number;
        resource_map_[resource.node_number].units_count += resource.units_count;
        
        pthread_mutex_unlock(&resources_mutex_);
        
        pthread_cond_signal(&new_resources_);
    }
    
    bool Scheduler::findResource(const common::Job &job)
    {
        bool is_found = false;
        
        pthread_mutex_lock(&resources_mutex_);
        
        //
        ::std::ostringstream out_stream;
        out_stream << ::std::endl << "#########" << ::std::endl;
        out_stream << "Available resources:" << ::std::endl;
        for (auto &key_value : resource_map_) {
            out_stream << key_value.second << ::std::endl;
        }
        //

        auto it = node_numbers_queue_.begin();
        while (it != node_numbers_queue_.end()) {
            common::Resource &resource = resource_map_[*it];
            
            //trying to find available resources
            if (resource.units_count >= job.units_count) {
                out_stream << "Resource was found for job: " << job << ::std::endl;
                out_stream << "Resource: " << resource << ::std::endl;
                out_stream << "Scheduled" << ::std::endl;
                
                //if found then we need to decrease units_count in map
                resource.units_count -= job.units_count;
                if (!resource.units_count) {
                    //if count of units in current node is 0
                    //then remove it form map
                    resource_map_.erase(*it);
                }
                node_numbers_queue_.erase(it);
                is_found = true;
                break;
            } else {
                ++it;
            }
        }
        
        pthread_mutex_unlock(&resources_mutex_);
        
        if (!is_found)
            out_stream << "Not enough resources for job: " << job << ::std::endl;
            out_stream << "Pending" << ::std::endl;
        
        out_stream << "#########" << ::std::endl << ::std::endl;
        common::log(out_stream.str());
        
        return is_found;
    }
    
}   //namespace scheduler