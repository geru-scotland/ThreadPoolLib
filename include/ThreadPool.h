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

class ThreadPool {

private:
    typedef std::vector<std::thread> ThreadPoolVector;
    typedef std::queue<std::function<void()>> TasksQueue;
    typedef std::lock_guard<std::mutex> LockGuard;
    typedef std::mutex Mutex;

    uint8_t threadNum_;
    ThreadPoolVector pool_;
    TasksQueue tasks_;
    Mutex mutex;

public:
    explicit ThreadPool(uint8_t num);
    ~ThreadPool();
    void StopPool();
    void ExecuteTask();
};


#endif //THREADPOOLLIB_THREADPOOL_H
