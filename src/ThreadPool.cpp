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

ThreadPool::ThreadPool(uint8_t num) :  threadNum_(static_cast<uint8_t>(num)) {
    printf("Thread Pool starting with %i", threadNum_);

    /*
     *   Create the thread pool
     *
     * - emplace_back instead of push_back in order to avoid unnecessary copies.
     *   This will build the object directly in the vector's allocated memory.
     *
     * - The usual std::thread(&ThreadPool::ExecuteTask, this) would be redundant
     *   as the type of the vector is std::thread, emplace_back will create a thread
     *   with these arguments.
     */
    for(int i = 0; i < threadNum_; i++) {
        pool_.emplace_back(&ThreadPool::ExecuteTask, this);
    }
}

ThreadPool::~ThreadPool() {
    this->StopPool();
}

/**
 * In order to avoid zombie threads, we need to perform a join for
 * each of them, it will free its resources.
 * Note: we need a reference to the real thread object, we need
 * that specific thread to join.
 * (Nonetheless, the compiler will not allow us to copy a thread object)
 */
void ThreadPool::StopPool() {
    for(std::thread& thread : pool_){
        if(thread.joinable())
            thread.join();
    }
}

void ThreadPool::ExecuteTask() {


}
