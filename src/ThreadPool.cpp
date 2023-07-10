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
        std::thread::id threadId = pool_.back().get_id();
        threadIdMap_[threadId] = i;
#ifdef DEBUG
        std::cout << "Thread id: " << threadId << " associated to: "<< i << std::endl;
#endif
    }
}

/**
 * When destroying the object, properly stops all the threads in the pool before
 * freeing resources.
 *
 * Joining is mandatory for every thread to avoid zombie threads and resource leaks.
 * Note: It's essential to wait for each specific thread's execution to finish before
 * freeing its resources.
 */
ThreadPool::~ThreadPool() {
    // Don't need to lock mutex here, worst case scenario, we will have an extra iteration
    while(poolActive_){
        {
            if(tasks_.empty()){
                /**
                 * Since std::thread::joinable doesn't check this, we need to have in mind a few things:
                 *
                 * Before the joining and resource cleaning, we need to wake up all threads.
                 * std::thread::join waits for the thread's current execution to finish. It will be
                 * woken up when it gets notified AND the predicate is true. If we try to
                 * perform the joining on a thread which execution is in constant loop,
                 * that joining will keep waiting forever, and the object will not be destroyed.
                 * which will end in the system being frozen.
                 *
                 * Note: poolActive needs to be set to false before sending the
                 * notifications, as it forms part of the condition for the predicate;
                 * if it is set to true, it shall not wake up, therefore the thread will hang.
                 */
                poolActive_ = false;
                cv_.notify_all();
            }
        }
    }

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
    std::shared_ptr<Task> task;
    while(poolActive_){
#ifdef DEBUG
        std::thread::id threadId = std::this_thread::get_id();
        int id = threadIdMap_[threadId];
        std::cout << "Thread checking queue: " << threadId << " associated to: "<< id << std::endl;
#endif
        {
            UniqueLock lock(mutex_);
            cv_.wait(lock, MutexLockPredicate(tasks_, poolActive_));
            if(!tasks_.empty()){
                task = std::move(tasks_.front());
                tasks_.pop();
                task->AssociateThread(threadIdMap_[std::this_thread::get_id()]);
                lock.unlock();
                task->Execute();
            }
        }
    }
}
