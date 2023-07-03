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

#ifdef DEBUG
#define MAX_TASK_TESTS 500
#endif

void foo(){
    std::cout << "\n My Task, thread id: "<< std::this_thread::get_id() <<" \n";
}

int main() {

#ifdef DEBUG
    ThreadPool tpl(std::thread::hardware_concurrency());

    for(int i = 0; i <= MAX_TASK_TESTS; i++){
        tpl.AddTask(foo);
    }
#endif

    return 0;
}
