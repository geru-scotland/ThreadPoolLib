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

class ThreadPool {

private:
    typedef std::mutex Mutex;
    typedef std::atomic<bool> AtomicBool;

    typedef std::vector<std::thread> ThreadPoolVector;
    typedef std::queue<std::function<void()>> TasksQueue;
    typedef std::lock_guard<Mutex> LockGuard;

    uint8_t poolSize_;
    ThreadPoolVector pool_;
    TasksQueue tasks_;
    Mutex mutex_;
    AtomicBool poolActive_ = true;

public:
    explicit ThreadPool(uint8_t num);
    ~ThreadPool();

    // Ensures all running threads are properly terminated upon the pool's destruction.
    void StopPool();

    // Executes a task. To be run by threads in the pool.
    void ExecuteTask();

    /**
     * @brief AddTask function template.
     *
     * This function adds tasks to the queue by emplacing them directly
     * into the queue's memory, bypassing the need for copy construction.
     *
     * It employs perfect forwarding, meaning it preserves the original "value category"
     * (either lvalue or rvalue) of the passed arguments.
     *
     * Optimization note: An alternative approach would be to create a variable (say, a function wrapper),
     * then convert it to an rvalue using std::move, so it's moved instead of copied:
     *
     * 1. FunctionWrapper task = std::bind(...), and then
     * 2. tasks_.push(std::move(task))
     *
     * After the move, the content of the 'task' variable would be in an unspecified state,
     * as it's an rvalue, essentially a temporary or unnamed content in memory. Note that
     * both push and push_back methods copy the argument if it's an lvalue.
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

    template<typename Function, typename... Args>
    void AddTask(Function&& func, Args&&... args){
        EmplaceTaskImpl(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
    }

    template<typename Task>
    void EmplaceTaskImpl(Task&& task){
        LockGuard lock(mutex_);
        tasks_.emplace(std::forward<Task>(task));
    }
};


#endif //THREADPOOLLIB_THREADPOOL_H
