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
#include "Task.h"

int main() {

    ThreadPool pool(std::thread::hardware_concurrency());

    /**
     * Some basic usage examples.
     *
     * These are merely illustrative, it can be used as one see fit.
     */


    /**
     * Example 1:
     * Note: by using std::move(task1) objet value renders undefined
     * Therefore, the variable task1 should not be used.
     */
    Task task1;
    task1(normalFunction, normalCallback);
    pool.AddTask(std::move(task1));

    /**
     * Example 2
     */
    Task task2;
    task2(
            [](){
                printf("\n Lambda main function \n");
            },
            []() {
                printf("\n Lambda callback \n");
            }
    );

    pool.AddTask(std::move(task2));


    /**
     * Example 3
     */
    std::shared_ptr<Task> task3 = std::make_shared<Task>();
    (*task3)(normalFunction, normalCallback);
    pool.AddTask((*task3));


    /**
     * Example 4
     */
    Examples::Foo fooObj1;
    auto fooObj2 = std::make_shared<Examples::Foo>();

    Task task4;
    task4(
            [&fooObj1, &fooObj2]() {
                printf("Lambda Task %i", fooObj1.MyTask(9));
                fooObj2->MyTask(1399);
            },
            [&fooObj1](){
                fooObj1.MyCallback(fooObj1.MyTask(4 * 2));
            }
    );

    pool.AddTask(std::move(task4));

    return 0;
}
