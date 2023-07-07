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

#ifndef THREADPOOLLIB_TASK_H
#define THREADPOOLLIB_TASK_H


class Task {
public:
    Task() = default;

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

    void ExecuteTask(){
        task_();
    }

private:
    std::function<void()> task_;
};

#endif //THREADPOOLLIB_TASK_H
