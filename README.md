# ThreadPoolLib

ThreadPoolLib is a minimalist C++ thread pool library, designed with simplicity and efficiency in mind. It provides an easy and straightforward way to manage and execute tasks concurrently in a multi-threaded environment, and is an essential tool for developers working on high-performance applications.

## Features

* **Ease of Use**: ThreadPoolLib allows developers to easily add tasks with callbacks to be processed concurrently, through a simple and intuitive interface.

* **Efficiency**: ThreadPoolLib leverages modern C++ features and best practices to ensure that your tasks are processed as efficiently as possible. It uses rvalue references and move semantics to avoid unnecessary copies, and manages all thread-related resources for you to eliminate the risk of common threading pitfalls such as zombie threads.

* **Performance**: The library utilizes C++ `std::thread` to create and manage threads. It optimizes the number of threads created based on the number of cores available, ensuring optimal utilization of hardware resources.

## Updates

### Improved performance and flexibility with `std::unique_lock`
* **Use of `std::unique_lock`:** To provide a finer-grained control over locking and synchronization, the library has been updated to use `std::unique_lock`. This provides greater flexibility compared to the previous `std::lock_guard`, allowing for unlocking and re-locking a mutex within the same scope. This flexibility is especially useful in facilitating lengthy operations without holding the lock, thus reducing contention and enhancing performance. The implementation now leverages `std::condition_variable` and its `wait()` method, which puts threads to sleep when there are no tasks to process, rather than constantly polling for new tasks. This helps to reduce CPU usage. Additionally, the introduction of a predicate function with `wait()`, manages the wake-up conditions for the sleeping threads, ensuring that they only awake when there are tasks to be consumed. This results in a more efficient and responsive system that optimizes task handling and thread utilization.

### Callbacks and Result Handling

Instead of simply adding and running tasks concurrently, ThreadPoolLib supports advanced use cases through task callbacks and result handling. Here's what you can do:

- **Task Callbacks**: Add a callback function that will be invoked once a task has completed its execution. This can be useful for handling results, logging, or chaining tasks.

- **Result Handling**: When a task finishes execution, it might return a result. With ThreadPoolLib, you can easily handle these results within the callback function, allowing you to use or store the outcome of the task.

## Future Enhancements

This library is under active development and future enhancements include:

* **Thread Metrics and Visualization**: In the interest of providing valuable insights about the efficiency of the tasks and the overall performance of the applications, I am planning to introduce a feature that collects and exposes thread metrics. These metrics will enable users to visualize and analyze the behavior of their thread pools over time using tools such as Grafana and InfluxDB.
* **Task dependency**: (suggested by [@tugrul512bit](https://github.com/tugrul512bit) [here](https://github.com/geru-scotland/ThreadPoolLib/issues/4) It will consist in creating dependencies between tasks, so one task can not be consumed until all of its dependencies are resolved, generating this way graphs of tasks, such as:

```
task 1 <--- (task 2 + task 3)
 |
 |
 V 
task 4 + task 5 ----> task 6
```

## Building

This project uses `cmake` as a build system.

```bash
mkdir build && cd build
cmake ..
make
```

## Basic usage
Include `include/ThreadPool.h` and create a `ThreadPool` object by specifying the number of threads you want to use. Then you can add tasks using the `AddTask()` or `AddTaskWithCallback()` functions. The `Examples` mentioned here refers to a namespace containing some sample functions and methods.

```cpp
ThreadPool tpl(std::thread::hardware_concurrency());
```

- **Example 1**:

```cpp
Task task1;
task1(normalFunction, normalCallback);
pool.AddTask(std::move(task1));
```

- **Example 2**:
```cpp
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

```

- **Example 3**:
```cpp
std::shared_ptr<Task> task3 = std::make_shared<Task>();
(*task3)(normalFunction, normalCallback);
pool.AddTask((*task3));

```

- **Example 4**:
```cpp
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

```

* Note: The testing mode is enabled in the `CMakeLists.txt` file. To use the library without the testing mode, simply comment out the line: 
```bash
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
```


# License

This project is licensed under the GPL 3 License - see the LICENSE.md file for details.