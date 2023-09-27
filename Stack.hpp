/*
MIT License

Copyright (c) 2023 Max Goren, http://www.maxgcoding.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef STACK_HPP
#define STACK_HPP

template <class T>
class Stack {
    private:
        T* a;
        int n;
        int maxn;
        void resize(int newsize) {
            T* tmp = new T[newsize];
            for (int i = 0; i < n; i++)
                tmp[i] = a[i];
            delete [] a;
            a = tmp;
            maxn = newsize;
        }
    public:
        Stack(int initsize = 100) {
            a = new T[initsize];
            maxn = initsize;
            n = 0;
        }
        Stack(const Stack& o) {
            n = o.n;
            maxn = o.maxn;
            a = new T[o.maxn];
            for (int i = 0; i < o.n; i++)
                a[i] = o.a[i];
        }
        ~Stack() {
            delete [] a;
        }
        void push(T info) {
            if (n + 1 == maxn)
                resize(2*maxn);
            a[n++] = info;
        }
        T pop() {
            return a[--n];
        }
        int size() const {
            return n;
        }
        bool empty() const {
            return n == 0;
        }
        Stack operator=(const Stack& o) {
            n = o.n;
            maxn = o.maxn;
            a = new T[o.maxn];
            for (int i = 0; i < o.n; i++)
                a[i] = o.a[i];
            return *this;
        }
};

#endif