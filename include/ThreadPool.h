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

#ifndef THREADPOOLLIB_THREADPOOL_H
#define THREADPOOLLIB_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <atomic>
#include <exception>
#include <condition_variable>

#include "Macros.h"

class ThreadPool {

private:
    typedef std::mutex Mutex;
    typedef std::atomic<bool> AtomicBool;
    typedef std::condition_variable ConditionVariable;

    typedef std::vector<std::thread> ThreadPoolVector;
    typedef std::queue<std::function<void()>> TasksQueue;
    typedef std::unique_lock<Mutex> UniqueLock;

    uint8_t poolSize_;
    ThreadPoolVector pool_;
    TasksQueue tasks_;
    Mutex mutex_;
    AtomicBool poolActive_ = true;
    ConditionVariable cv;

public:
    explicit ThreadPool(uint8_t num);

    // Ensures all running threads are properly terminated upon the pool's destruction.
    ~ThreadPool();

    // Executes a task. To be run by threads in the pool.
    void ExecuteTask();

    /**
     * @brief AddTaskWithCallback function template.
     *
     * This function template allows the addition of tasks along with associated callbacks
     * to the task queue. Similar to the AddTask function, it employs perfect forwarding to
     * preserve the original "value category" of passed arguments, and emplaces them directly
     * into the task queue.
     *
     * In addition to executing the task, this function also manages the execution of a
     * callback function upon completion of the task.
     *
     * By using std::invoke_result_t, we ensure that the ReturnType correctly adapts to the
     * return type of the task function. This approach maintains optimal performance by avoiding
     * unnecessary copy or move operations.
     */
    template<typename Function, typename Callback, typename... Args>
    void AddTaskWithCallback(Function&& func, Callback&& callback, Args&&... args){

        typedef std::invoke_result_t<Function, Args...> ReturnType;

        std::function<void()> wrapper = [function = std::forward<Function>(func),
                callback = std::forward<Callback>(callback),
                ...args = std::forward<Args>(args)] ()
        {
            if constexpr(std::is_void_v<ReturnType>){
                function(args...);
                callback();
            }
            else
                callback(function(args...));
        };
        EmplaceTaskImpl(std::move(wrapper));
    }

    /**
     * @brief AddTask function template.
     *
     * This function template facilitates the addition of tasks to the task queue. It employs
     * perfect forwarding, a technique that helps to preserve the original "value category"
     * of passed arguments.
     *
     * By directly emplacing the task into the queue using std::bind and std::forward, it
     * optimizes performance by bypassing unnecessary copy or move operations.
     *
     * The use of std::bind also enhances flexibility, as it allows the function to accept
     * a variety of callable entities (like function pointers, functors, lambdas) and bind them
     * with their respective arguments.
     */
    template<typename Function, typename... Args>
    void AddTask(Function&& func, Args&&... args){
        EmplaceTaskImpl(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
    }

    /**
     * @brief EmplaceTaskImpl function template.
     *
     * This function template manages the addition of tasks (encapsulated in std::function objects)
     * into the task queue. It makes use of perfect forwarding to preserve the "value category"
     * of passed tasks and directly emplaces them into the queue, avoiding unnecessary copy or
     * move operations.
     *
     * The usage of a mutex ensures thread-safety during the task emplacement into the queue. It
     * guarantees that only one thread can modify the task queue at any given moment, effectively
     * preventing race conditions.
     */
    template<typename Task>
    void EmplaceTaskImpl(Task&& task){
        UniqueLock mtxLock(mutex_);
        tasks_.emplace(std::forward<Task>(task));
        cv.notify_one();
    }

    /**
     * @brief Creates a predicate for checking if a container is empty or if a pool is active.
     *
     * Returns a lambda function as wrapper of the predicate.
     */
    template<typename T, typename V>
    std::function<bool()> MutexLockPredicate(const T& cont, const V& poolActive){
        auto wrapper = std::function<bool()>();
        try {
             wrapper = [&cont, &poolActive](){
                return !cont.empty() || !poolActive;
            };
        } catch(const std::exception& e){
            printf("[Exception] %s", e.what());
        }
        return std::move(wrapper);
    }
};


#endif //THREADPOOLLIB_THREADPOOL_H
