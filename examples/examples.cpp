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
#include <tuple>
#include "ThreadPool.h"
#include "Foo.h"
#include "Task.h"

int main() {

    /**
     * Some basic usage examples.
     *
     * These are merely illustrative, it can be used as one see fit.
     */
    std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());
    std::shared_ptr<Task> task1, task2, task3, task4;

    /**
     * Example 1:
     */
    task1 = pool->CreateTask(normalFunction, normalCallback);

    /**
     * Example 2
     */
    task2 = pool->CreateTask(
            [](){
                printf("\n Lambda main function \n");
            },
            []() {
                printf("\n Lambda callback \n");
            }
    );


    /**
     * Example 3
     */
    std::tuple<int, int, int> args = std::make_tuple(2, 555, 999);
    task3 = pool->CreateTask(normalFunctionParams, normalCallbackParams, args);


    /**
     * Example 4
     */
    Examples::Foo fooObj1;
    auto fooObj2 = std::make_shared<Examples::Foo>();

    task4 = pool->CreateTask(
            [&fooObj1, &fooObj2]() {
                printf("Lambda Task %i", fooObj1.MyTask(9));
                fooObj2->MyTask(1399);
            },
            [&fooObj1](){
                fooObj1.MyCallback(fooObj1.MyTask(4 * 2));
            }
    );

    printf("\n Thread id for task1: %i\n", task1->GetThreadId());
    printf("\n Thread id for task2: %i\n", task2->GetThreadId());
    printf("\n Thread id for task3: %i\n", task3->GetThreadId());
    printf("\n Thread id for task4: %i\n", task4->GetThreadId());

    return 0;
}
