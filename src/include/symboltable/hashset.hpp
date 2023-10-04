#ifndef iterable_hashset_hpp
#define iterable_hashset_hpp
#include "linkedset.hpp"
#include "hashfn.hpp"

template <class T, class hasher = hashfn<T>>
class IterableSet {
    private:
        linkedset<T>* table;
        int n;
        int maxn;
        void init(int max) {
            maxn = max;
            n = 0;
            table = new linkedset<T>[maxn];
        }
        class Iterator {
            private:
                linkedset<T>* __setPtr;
                typename linkedset<T>::Iterator __setIter;
                T junk;
            public:
                Iterator(linkedset<T>* __Ptr, T info) {
                    __setPtr = __Ptr;
                    __setIter = __Ptr->begin();
                    while (__setIter != __Ptr->end() && *__setIter != info) __setIter++;
                }
                Iterator(linkedset<T>* __Ptr) {
                    __setPtr = __Ptr;
                    while (__setPtr && __setPtr->empty()) {
                        __setPtr++;
                    }
                    if (__setPtr)
                    __setIter = __setPtr->begin();
                }
                Iterator() {

                }
                T& operator*() {
                    if (__setIter != __setPtr->end())
                        return *__setIter;
                    return junk;
                }
                Iterator operator++() {
                    if (__setIter != __setPtr->end()) {
                        ++__setIter;
                        if (__setIter != __setPtr->end()) {
                            return *this;
                        }
                    }
                    do {
                        __setPtr++;
                    } while (__setPtr->isEmpty());
                    __setIter = __setPtr->begin();
                    return *this;
                }
                Iterator operator++(int) {
                    Iterator it = *this;
                    ++*this;
                    return it;
                }
                bool operator==(const Iterator& o) const noexcept {
                    return __setPtr == o.__setPtr && __setIter == o.__setIter;
                }
                bool operator!=(const Iterator& o) const noexcept {
                    return !(*this==o);
                }
        };
    public:
        IterableSet(int max = 113) {
            init(max);
        }
        IterableSet(const IterableSet& o) {
            init(o.maxn);   
            for (auto e : o)
                put(e);
        }
        ~IterableSet() {
            delete [] table;
        }
        int size() const {
            return n;
        }
        int max_size() const {
            return maxn;
        }
        bool empty() const {
            return n == 0;
        }
        void put(T info) {
            unsigned int idx = hasher()(info) % maxn;
            table[idx].add(info);
            n++;
        }
        void remove(T info) {
            unsigned int idx = hasher()(info) % maxn;
            table[idx].remove(info);
            n++;
        }
        Iterator find(T info) {
            unsigned int idx = hasher()(info) % maxn;
            return table[idx].find(info) == table[idx].end() ? end():Iterator(table+idx, info);
        }
        Iterator begin() {
            return Iterator(table);
        }
        Iterator end() {
            return Iterator(table+maxn);
        }
        const Iterator begin() const {
            return Iterator(table);
        }
        const Iterator end() const {
            return Iterator(table+maxn);
        }
        IterableSet operator=(const IterableSet& o) {
            init(o.maxn);   
            for (auto e : o)
                put(e);
            return *this;
        }
};

#endif