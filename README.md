# ThreadPoolLib

ThreadPoolLib is a minimalist C++ thread pool library, designed with simplicity and efficiency in mind. It provides an easy and straightforward way to manage and execute tasks concurrently in a multi-threaded environment, and is an essential tool for developers working on high-performance applications.

## Features

* **Ease of Use**: ThreadPoolLib allows developers to easily add tasks to be processed concurrently, through a simple and intuitive interface.

* **Efficiency**: ThreadPoolLib leverages modern C++ features and best practices to ensure that your tasks are processed as efficiently as possible. It uses rvalue references and move semantics to avoid unnecessary copies, and manages all thread-related resources for you to eliminate the risk of common threading pitfalls such as zombie threads.

* **Performance**: The library utilizes C++ `std::thread` to create and manage threads. It optimizes the number of threads created based on the number of cores available, ensuring optimal utilization of hardware resources.

## Future Enhancements

This library is under active development and future enhancements include:

* **Use of `std::unique_lock`:** To provide a finer-grained control over locking and synchronization, the library will be updated to use `std::unique_lock`. This allows more flexibility than the current `std::lock_guard`, as it permits unlocking and re-locking a mutex in the same scope, making it possible to avoid holding a lock during lengthy operations. This will significantly reduce contention and improve scalability and performance.

* **Task Return Values:** Future iterations of ThreadPoolLib will allow tasks to return a value. This will add flexibility and enable the library to handle a wider variety of use-cases.

## Building

This project uses `cmake` as a build system.

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

Include `ThreadPool.h` and create a `ThreadPool` object by specifying the number of threads you want to use. Then you can add tasks using the `AddTask` function.

* **With Lambda**:

```cpp
ThreadPool tpl(std::thread::hardware_concurrency());

tpl.AddTask([]() { std::cout << "\n My Task, thread id: "<< std::this_thread::get_id() <<" \n"; });
```
* ****

Aside from using lambda functions, `AddTask` also supports ordinary functions and functors. This can be handy when you have predefined tasks.

First, define your function:

```cpp
void foo() {
    std::cout << "\n My Task, thread id: "<< std::this_thread::get_id() <<" \n";
}
```

Then pass the pointer to the function to AddTask:

```cpp
ThreadPool tpl(std::thread::hardware_concurrency());

tpl.AddTask(foo);
```

* Note: The testing mode is enabled in the `CMakeLists.txt` file. To use the library without the testing mode, simply comment out the line: 
```bash
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
```


# License

This project is licensed under the GPL 3 License - see the LICENSE.md file for details.