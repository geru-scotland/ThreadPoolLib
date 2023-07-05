/*
 * This file is part of the ThreadPoolLib project.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * @author - Geru-Scotland (https://github.com/geru-scotland)
 */


#include <iostream>
#include "ThreadPool.h"


ThreadPool::ThreadPool(uint8_t num) : poolSize_(static_cast<uint8_t>(num)) {
    /*
     *   Initialize the thread pool
     *
     * - Use emplace_back instead of push_back to avoid unnecessary copies.
     *   This directly constructs the thread object in the vector's memory.
     *
     * - A thread is created with the thread function and the 'this' pointer as arguments.
     */
    for(int i = 0; i < poolSize_; i++) {
        pool_.emplace_back(&ThreadPool::ExecuteTask, this);
    }
}

ThreadPool::~ThreadPool() {
    while(poolActive_){
        LockGuard lock(mutex_);
        if(tasks_.empty())
            poolActive_ = false;
    }
    this->StopPool();
}

/**
 * Properly stops all the threads in the pool before freeing resources.
 *
 * Joining is mandatory for every thread to avoid zombie threads and resource leaks.
 * Note: It's essential to wait for each specific thread's execution to finish before freeing its resources.
 */
void ThreadPool::StopPool() {
    for(std::thread& thread : pool_){
        if(thread.joinable())
            thread.join();
    }
}

/**
 * Threads execute tasks from the queue while the pool is active.
 *
 * If tasks are available in the queue, a thread will pop a task,
 * execute it and go back to waiting for the next task.
 */
void ThreadPool::ExecuteTask() {
    std::function<void()> task;
    while(poolActive_){
        {
            LockGuard lock(mutex_);
            if(!tasks_.empty()){
                task = tasks_.front();
                tasks_.pop();
            }
        }
        if(task){
            task();
            task = std::function<void()>();
        }
    }
}
