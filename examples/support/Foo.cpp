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
#include <thread>
#include "Foo.h"

namespace Examples {

    int Foo::MyTask(int value) const {
        printf("\n MyTests: %i\n", value);
        return value;
    }

    void Foo::MyCallback(int value) const {
        printf("\n MyCallback: %i\n", value);
    }

    void Foo::MyStaticTask(int value) {
        printf("\n Static Task: %i\n", value);
    }

    void foo(){
        std::cout << "\n [Task]: Foo(), thread id: " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
    }

    void fooParam(int a, int b){
        std::cout << "\n [Task]: fooParam(), and my parameters are: " << a << " and "<< b <<" - thread id: " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
    }

    void fooCallback(){
        std::cout << "\n [Callback]: fooCallback(), thread id: " << std::this_thread::get_id() << "\n";
    }

    int fooResult(){
        int result = 9;
        std::cout << "\n [Task]: fooResult(), My result will be: " << result << " - thread id: " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate a 2 seconds process/delay.
        return result;
    }

    int fooResultAndParam(int value){
        int result = value * 2;
        std::cout << "\n [Task]: fooResultAndParam(), My result will be: " << value << "*2 = " << result << " - thread id: " << std::this_thread::get_id() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate value 2 seconds process/delay.
        return result;
    }

    void fooResultCallback(int value){
        std::cout << "\n [Callback]: fooResultCallback(), Data result: " << value << "  - thread id: " << std::this_thread::get_id() << "\n";
    }

} // End Examples

void normalFunction(){
    printf("This is a normal function");
}

void normalCallback(){
    printf("This is a normal callback");
}