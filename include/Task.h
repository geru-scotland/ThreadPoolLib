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

#include <functional>
#include <iostream>
#include "Macros.h"

#ifndef THREADPOOLLIB_TASK_H
#define THREADPOOLLIB_TASK_H

enum TaskStatus {
    STATUS_PENDING = 0,
    STATUS_RUNNING = 1,
    STATUS_DONE = 2
};

class Task {

public:
    Task() = default;

    /**
     * @brief () operator overload template.
     *
     * This function template enables the invocation of a given function `func` with parameters
     * provided in `argsTuple`, along with an associated callback `callback` to be executed
     * after `func` finishes its execution.
     *
     * Using std::invoke_result_t helps deduce the correct return type for the function `func`,
     * and std::is_void_v helps handle the different cases for the callback execution based on
     * the return type of the function.
     *
     * This approach provides flexibility in handling different function signatures, optimizing
     * performance by avoiding unnecessary copy or move operations, and offers a clean way to
     * execute a callback function post task execution.
     */
    template<typename Function, typename Callback, typename... Types>
    void operator()(Function&& func, Callback&& callback, const std::tuple<Types...>& argsTuple){

        typedef std::invoke_result_t<Function, Types...> ReturnType;

        task_ = [&func, &callback, argsTuple] ()
        {
            if constexpr(std::is_void_v<ReturnType>){
                std::apply(func, argsTuple);
                callback();
            } else
                callback(std::apply(func, argsTuple));

        };
    }

    /**
     * @brief () operator overload template.
     *
     * Similar to the previous function template but tailored to handle cases where no arguments
     * are passed in a tuple form. It takes a callable object and a callback function, both
     * perfectly forwarded.
     *
     * The callback function will be invoked upon completion of the task, regardless of
     * the return type of the task function, as evaluated by std::invoke_result_t.
     */
    template<typename Function, typename Callback>
    void operator()(Function&& func, Callback&& callback){

        typedef std::invoke_result_t<Function> ReturnType;

        task_ = [&func, &callback] ()
        {
            if constexpr(std::is_void_v<ReturnType>){
                func();
                callback();
            } else
                callback(func());

        };
    }

    /**
     * @brief () operator overload template.
     *
     * This function template focuses on situations where the task function and its
     * arguments are provided, but there's no callback function.
     *
     * The tuple of arguments is passed as a const reference, preventing modifications
     * while avoiding the cost of a full copy. The function and its arguments are then
     * enqueued as a task without a callback.
     */
    template<typename Function, typename... Types>
    void operator()(Function&& func, const std::tuple<Types...>& argsTuple){
        task_ = [&func, argsTuple](){
            std::apply(func, argsTuple);
        };
    }

    /**
     * @brief () operator overload template.
     *
     * This version of the function template is specially designed to cater for the cases
     * where only a task function is provided with no associated callback or tuple arguments.
     *
     * Here, the task function  will be directly added to the task queue.
     */
    template<typename Function>
    void operator()(Function&& func){
        task_ = [&func](){
            func();
        };
    }

    void Execute(){
        try{
            status_ = STATUS_RUNNING;
            task_();
        } catch(std::exception& e){
            TRACE_LOG("[EXCEPTION] %s", e.what());
        }
        status_ = STATUS_DONE;
    }

    void AssociateThread(int threadId) noexcept { threadId_ = threadId; }
    int GetThreadId() const noexcept { return threadId_; }

private:
    std::function<void()> task_;
    int threadId_{};
    TaskStatus status_{};
};

#endif //THREADPOOLLIB_TASK_H
