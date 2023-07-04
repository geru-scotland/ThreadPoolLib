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

void foo(){
    std::cout << "\n [Task]: foo(), thread id: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
}

void fooParam(int a, int b){
    std::cout << "\n [Task]: fooParam(), and my parameters are: " << a << " and "<< b <<" - thread id: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
}

void fooCallback(){
    std::cout << "\n [Callback]: fooCallback(), thread id: " << std::this_thread::get_id() << "\n";
}

int fooResult(){
    int result = 9;
    std::cout << "\n [Task]: fooResult(), My result will be: " << result << " - thread id: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
    return result;
}

int fooResultAndParam(int a){
    int result = a * 2;
    std::cout << "\n [Task]: fooResultAndParam(), My result will be: "<< a << "*2 = "  << result << " - thread id: " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
    return result;
}

void fooResultCallback(int a){
    std::cout << "\n [Callback]: fooResultCallback(), Data result: " << a << "  - thread id: " << std::this_thread::get_id() << "\n";
}

int main() {

#ifdef DEBUG
    ThreadPool tpl(std::thread::hardware_concurrency());

    /**
     * Tasks
     */

    // With a lambda.
    tpl.AddTask([]() { std::cout << "\n Adding my task with a lambda, thread id: " << std::this_thread::get_id() << "\n"; });

    // Normal task "Fire-and-forget"
    tpl.AddTask(foo);

    // Normal task,  with parameters.
    tpl.AddTask(fooParam, 4, 7);

    /**
     * Tasks + Callbacks
     */

    // Adding a task with a Callback which will be called after the task is complete.
    tpl.AddTaskWithCallback(foo, fooCallback);

    // Callback which will receive the task's result.
    tpl.AddTaskWithCallback(fooResult, fooResultCallback);

    // The task will have some args (9 in the example), the result of it will be sent to the callback
    tpl.AddTaskWithCallback(fooResultAndParam, fooResultCallback, 9);

#endif

    return 0;
}
