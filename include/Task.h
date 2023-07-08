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
     * This function template allows the addition of tasks along with associated callbacks
     * to the task queue. It does employ perfect forwarding to preserve the original
     * "value category" of passed arguments, and emplaces them directly
     * into the task queue
     *
     * In addition to executing the task, this function also manages the execution of a
     * callback function upon completion of the task.
     *
     * By using std::invoke_result_t, we ensure that the ReturnType correctly adapts to the
     * return type of the task function. This approach maintains optimal performance by avoiding
     * unnecessary copy or move operations.
     */
    template<typename Function, typename Callback, typename... Args>
    void operator()(Function&& func, Callback&& callback, Args&& ...args){

        typedef std::invoke_result_t<Function, Args...> ReturnType;

        task_ = [function = std::forward<Function>(func),
                callback = std::forward<Callback>(callback),
                ...params = std::forward<Args>(args)] ()
        {
            if constexpr(std::is_void_v<ReturnType>){
                function(params...);
                callback();
            }
            else
                callback(function(params...));
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
