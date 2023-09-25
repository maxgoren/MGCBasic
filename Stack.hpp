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
            if (a != nullptr) delete [] a;
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