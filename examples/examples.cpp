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
#include "Foo.h"

int main() {

#ifdef DEBUG
    ThreadPool tpl(std::thread::hardware_concurrency());

    Examples::Foo fooObj1;
    auto fooObj2 = std::make_shared<Examples::Foo>();

    /**
     * Tasks
     */

    // With lambdas
    tpl.AddTask([]() { std::cout << "\n Adding my task with a lambda, thread id: " << std::this_thread::get_id() << "\n"; });
    tpl.AddTask([]() { std::cout << "\n Adding my task with a lambda, thread id: " << std::this_thread::get_id() << "\n"; });

    // If we need the tasks to be methods, we can use a lambda to capture them.
    tpl.AddTask([&fooObj1, &fooObj2]() {
        printf("Lambda Task %i", fooObj1.MyTask(9));
        fooObj2->MyTask(1399);
        fooObj1.MyCallback(fooObj1.MyTask(4 * 1));
    });

    // Normal task "Fire-and-forget"
    tpl.AddTask(normalFunction);

    // Normal task "Fire-and-forget", foo function from Examples namespace.
    tpl.AddTask(Examples::foo);

    // Normal task,  with parameters.
    tpl.AddTask(Examples::fooParam, 4, 7);

    // Static method as a task,  with parameters.
    tpl.AddTask(Examples::Foo::MyStaticTask, 26);

    /**
     * Tasks + Callbacks
     */

    tpl.AddTaskWithCallback(normalFunction, normalCallback);

    // Adding a task with a Callback which will be called after the task is complete.
    tpl.AddTaskWithCallback(Examples::foo, Examples::fooCallback);

    // Callback which will receive the task's result.
    tpl.AddTaskWithCallback(Examples::fooResult, Examples::fooResultCallback);

    // The task will have some args (9 in the example), the result of it will be sent to the callback
    tpl.AddTaskWithCallback(Examples::fooResultAndParam, Examples::fooResultCallback, 9);

#endif

    return 0;
}
