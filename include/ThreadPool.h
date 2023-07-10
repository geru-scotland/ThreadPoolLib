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
#include "Task.h"

class ThreadPool {

private:
    /**
     * Thread-safe.
     */
    typedef std::mutex Mutex;
    typedef std::atomic<bool> AtomicBool;
    typedef std::condition_variable ConditionVariable;
    typedef std::unique_lock<Mutex> UniqueLock;

    /**
     * Data structures
     */
    typedef std::vector<std::thread> ThreadPoolVector;
    typedef std::queue<std::shared_ptr<Task>> TasksQueue;
    typedef std::unordered_map<std::thread::id, int> ThreadIdMap;

    uint8_t poolSize_;
    ThreadPoolVector pool_;
    TasksQueue tasks_;
    Mutex mutex_;
    AtomicBool poolActive_ = true;
    ConditionVariable cv_;
    ThreadIdMap threadIdMap_;

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
    void AddTask(Task&& task){
        UniqueLock mtxLock(mutex_);
        tasks_.emplace(std::forward<Task>(task));
        cv_.notify_one();
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

public:
    explicit ThreadPool(uint8_t num);

    // Ensures all running threads are properly terminated upon the pool's destruction.
    ~ThreadPool();

    /**
     * @brief Disable copy and move semantics.
     *
     * These declarations prevent generation of default copy and move constructors
     * and assignment operators. This is to avoid shallow copy issues or potential
     * resource leaks for objects of the ThreadPool class.
     */
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    // Executes a task. To be run by threads in the pool.
    void ExecuteTask();

    /**
     * @brief Creates a new Task object with the specified function, callback and argument tuple.
     *
     * This function template constructs a Task object, which is designed to store a callable and its associated callback.
     * The function, callback, and arguments are stored in the Task, which is then added to the task queue.
     *
     * This allows for flexibility in the callable's signature and the usage of arguments.
     *
     * @return A shared pointer to the created Task.
     */
    template<typename Function, typename Callback, typename... Types>
    std::shared_ptr<Task> CreateTask(const Function& func, const Callback& callback, const std::tuple<Types...>& argsTuple){
        std::shared_ptr<Task> task = std::make_shared<Task>();
        (*task)(func, callback, argsTuple);
        AddTask(task);
        return task;
    }

    /**
     * @brief Constructs a new Task object with a given function and callback.
     *
     * This function template creates a Task, which encapsulates a callable and a callback to be executed post the callable's execution.
     * The created Task is added to the task queue.
     *
     * This version is intended to be used when the callable doesn't require any arguments.
     *
     * @return A shared pointer to the created Task.
     */
    template<typename Function, typename Callback>
    std::shared_ptr<Task> CreateTask(const Function& func, const Callback& callback){
        std::shared_ptr<Task> task = std::make_shared<Task>();
        (*task)(func, callback);
        AddTask(task);
        return task;
    }

    /**
     * @brief Generates a Task object with a specified function and its corresponding argument tuple.
     *
     * This function template generates a Task object that encapsulates a callable and its arguments.
     * The Task is then added to the task queue.
     *
     * This variant is meant for scenarios where the callable doesn't have an associated callback but requires arguments.
     *
     * @return A shared pointer to the created Task.
     */
    template<typename Function, typename... Types>
    std::shared_ptr<Task> CreateTask(const Function& func, const std::tuple<Types...>& argsTuple){
        std::shared_ptr<Task> task = std::make_shared<Task>();
        (*task)(func, argsTuple);
        AddTask(task);
        return task;
    }

    /**
     * @brief Creates a Task object with the provided function.
     *
     * This function template fabricates a Task object that wraps a callable.
     * The produced Task is then appended to the task queue.
     *
     * This version is utilized when the callable neither has a callback nor requires any arguments.
     *
     * @return A shared pointer to the created Task.
     */
    template<typename Function>
    std::shared_ptr<Task> CreateTask(const Function& func){
        std::shared_ptr<Task> task = std::make_shared<Task>();
        (*task)(func);
        AddTask(task);
        return task;
    }
};

#endif //THREADPOOLLIB_THREADPOOL_H
