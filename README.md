# ThreadPoolLib

ThreadPoolLib is a minimalist C++ thread pool library, designed with simplicity and efficiency in mind. It provides an easy and straightforward way to manage and execute tasks concurrently in a multi-threaded environment, and is an essential tool for developers working on high-performance applications.

## Features

* **Ease of Use**: ThreadPoolLib allows developers to easily add tasks to be processed concurrently, through a simple and intuitive interface.

* **Efficiency**: ThreadPoolLib leverages modern C++ features and best practices to ensure that your tasks are processed as efficiently as possible. It uses rvalue references and move semantics to avoid unnecessary copies, and manages all thread-related resources for you to eliminate the risk of common threading pitfalls such as zombie threads.

* **Performance**: The library utilizes C++ `std::thread` to create and manage threads. It optimizes the number of threads created based on the number of cores available, ensuring optimal utilization of hardware resources.

### Callbacks and Result Handling

In addition to simply adding and running tasks concurrently, ThreadPoolLib also supports advanced use cases through task callbacks and result handling. Here's what you can do:

- **Task Callbacks**: Add a callback function that will be invoked once a task has completed its execution. This can be useful for handling results, logging, or chaining tasks.

- **Result Handling**: When a task finishes execution, it might return a result. With ThreadPoolLib, you can easily handle these results within the callback function, allowing you to use or store the outcome of the task.


## Future Enhancements

This library is under active development and future enhancements include:

* **Use of `std::unique_lock`:** To provide a finer-grained control over locking and synchronization, the library will be updated to use `std::unique_lock`. This allows more flexibility than the current `std::lock_guard`, as it permits unlocking and re-locking a mutex in the same scope, making it possible to avoid holding a lock during lengthy operations. This will significantly reduce contention and improve scalability and performance.

* **Thread Metrics and Visualization**: In the interest of providing valuable insights about the efficiency of the tasks and the overall performance of the applications, I am planning to introduce a feature that collects and exposes thread metrics. These metrics will enable users to visualize and analyze the behavior of their thread pools over time using tools such as Grafana and InfluxDB.

## Building

This project uses `cmake` as a build system.

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

Include `include/ThreadPool.h` and create a `ThreadPool` object by specifying the number of threads you want to use. Then you can add tasks using the `AddTask()` or `AddTaskWithCallback()`functions.


## Usage

The updated usage of ThreadPoolLib with the new features would be as follows:

### Adding Tasks

Include `ThreadPool.h` and create a `ThreadPool` object by specifying the number of threads you want to use. Then you can add tasks using the `AddTask` function.

```cpp
ThreadPool tpl(std::thread::hardware_concurrency());
```
- **With Lambdas**:

```cpp
tpl.AddTask([]() { std::cout << "\n My Task, thread id: " << std::this_thread::get_id() << " \n"; });
```

- **With Regular Functions**:

```cpp
void foo() {
    std::cout << "\n My Task, thread id: " << std::this_thread::get_id() << " \n";
}

tpl.AddTask(foo);
```

- **With Parameters**:

```cpp
void fooParam(int a, int b) {
    std::cout << "\n My Task with parameters, thread id: " << std::this_thread::get_id() << " \n";
}

tpl.AddTask(fooParam, 4, 7);
```

### Adding Tasks with Callbacks

- **With regular functions and callback**:

```cpp
void fooCallback() {
    std::cout << "\n My Callback, thread id: " << std::this_thread::get_id() << " \n";
}

tpl.AddTaskWithCallback(foo, fooCallback);
```

- **With Result Handling**:

```cpp
int fooResult() {
    int result = // some operation
    return result;
}

void fooResultCallback(int result) {
    std::cout << "\n Received result: "<< result << " from task, thread id: " << std::this_thread::get_id() <<" \n";
}

tpl.AddTaskWithCallback(fooResult, fooResultCallback);
```

- **With Result Handling and Parameters**:

```cpp
int fooResultAndParam(int x) {
    int result = // some operation involving x
return result;
}

void fooResultCallback(int result) {
    std::cout << "\n Received result: "<< result <<" from task, thread id: " << std::this_thread::get_id() << " \n";
}

tpl.AddTaskWithCallback(fooResultAndParam, fooResultCallback, 9);
```


* Note: The testing mode is enabled in the `CMakeLists.txt` file. To use the library without the testing mode, simply comment out the line: 
```bash
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
```


# License

This project is licensed under the GPL 3 License - see the LICENSE.md file for details.