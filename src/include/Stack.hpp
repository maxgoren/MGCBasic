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

/*
#include <stack>
using std::stack;

template <class T>
struct Stack :public stack<T> {
    T pop() {
        T ret = stack<T>::top();
        stack<T>::pop();
        return ret;
    }
};
*/

template <class T>
class Stack {
    private:
       struct snode {
            T info;
            snode* next;
            snode(T in, snode* nx) {
                info = in;
                next = nx;
            }
       };
       int m_count;
       snode* m_head;
    public:
        Stack() {
            m_head = nullptr;
            m_count = 0;
        }
        Stack(const Stack& o) {
            m_head = nullptr;
            m_count = 0;
            if (!o.empty()) {
                snode dummy; snode* c = &dummy;
                for (snode* it = o.m_head; it != nullptr; it = it->next) {
                    c->next = new snode(it->info, nullptr);
                    c = c->next;
                    m_count++;
                }
            }
        }
        ~Stack() {
            while (!empty()) pop();
        }
        bool empty() const {
            return m_head == nullptr;
        }
        int size() const {
            return m_count;
        }
        void push(T info) {
            m_head = new snode(info, m_head);
            m_count++;
        }
        T pop() {
            T ret = m_head->info;
            snode* tmp = m_head;
            m_head = m_head->next;
            m_count--;
            delete tmp;
            return ret;
        }
        T& top() {
            return m_head->info;
        }
        void clear() {
            while (!empty()) pop();
        }
        Stack operator=(const Stack& o) {
            m_count = 0;
            m_head = nullptr;
            if (!o.empty()) {
                snode dummy; snode* c = &dummy;
                for (snode* it = o.m_head; it != nullptr; it = it->next) {
                    c->next = new snode(it->info, nullptr);
                    c = c->next;
                    m_count++;
                }
            }
            return *this;
        }
};

#endif