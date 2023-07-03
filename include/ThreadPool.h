/*
 * This file is part of the ThreadPool project.
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

typedef std::vector<std::thread> ThreadPoolVector;
typedef std::queue<std::function<void()>> TasksQueue;

class ThreadPool {

private:
    uint8_t _threadNum;
    ThreadPoolVector _pool;
    TasksQueue _tasks;

public:
    ThreadPool(uint8_t num);
};


#endif //THREADPOOLLIB_THREADPOOL_H
