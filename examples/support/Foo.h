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

#ifndef THREADPOOLLIB_FOO_H
#define THREADPOOLLIB_FOO_H

namespace Examples {

    class Foo {
    public:
        Foo()= default;

        int MyTask(int value) const ;
        void MyCallback(int value) const;

        static void MyStaticTask(int value);
    };

    void foo();
    void fooParam(int a, int b);
    void fooCallback();
    int fooResult();
    int fooResultAndParam(int value);
    void fooResultCallback(int value);

} // Examples

void normalFunction();
void normalCallback();

#endif //THREADPOOLLIB_FOO_H
