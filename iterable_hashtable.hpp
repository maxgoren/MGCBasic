/*
MIT License

Copyright (c) 2023 Max Goren - root@uniqueptr.com, www.maxgcoding.com

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

#ifndef iterable_hashtable_hpp
#define iterable_hashtable_hpp
#include <iostream>
#include "hashfn.hpp"

template <typename K, typename V, class hasher = hashfn<K>>
class IterableMap {
    private:
        constexpr static double MAX_LOADFACTOR = 0.5;
        constexpr static double MAX_SHRINKFACTOR = 0.78;
        struct MapEntry {
            K key;
            V value;
            bool empty;
            bool tombstone;
            MapEntry(K _key, V _value) {
                key = _key; value = _value;
                empty = false;
                tombstone = false;
            }
            MapEntry() { empty = true; tombstone = false; }
            MapEntry(const MapEntry& t) {
                key = t.key;
                value = t.value;
                empty = t.empty;
                tombstone = t.tombstone;
            }
            bool isEmpty() const { return empty; }
            bool tombstoned() const { return tombstone; }
        };
        MapEntry *table;
        int maxn;
        int n;
        int tombstones;
        void growAndRehash() {
            int oldmax = maxn;
            maxn = 2*oldmax;
            MapEntry* temp = table;
            table = new MapEntry[maxn];
            n = 0;
            for (int i = 0; i < oldmax; i++) {
                if (!temp[i].isEmpty() && !temp[i].tombstoned())
                    put(temp[i].key, temp[i].value);
            }
            delete [] temp;
        }
        void shrinkAndRehash() {
            int tmxn = maxn/1.21;
            int oldmax = maxn;
            do { tmxn+=5; } while (((double)n/tmxn) > MAX_LOADFACTOR);
            maxn = tmxn;
            MapEntry* temp = table;
            table = new MapEntry[maxn];
            n = 0;
            for (int i = 0; i < oldmax; i++) {
                if (!temp[i].isEmpty() && !temp[i].tombstoned())
                    put(temp[i].key, temp[i].value);
            }
            delete [] temp;
        }
        double loadfactor() {
            return (double)n/maxn;
        }
    public:
        class Iterator {
            private:
                MapEntry* __MapEntryPtr;
            public:
                Iterator(MapEntry* c) : __MapEntryPtr(c) {
                    
                }
                pair<K,V> operator*() {
                    if (__MapEntryPtr->isEmpty() || __MapEntryPtr->tombstoned()) {
                        while (__MapEntryPtr->isEmpty() || __MapEntryPtr->tombstoned()) {
                            __MapEntryPtr++;
                        }
                    }
                    return make_pair(__MapEntryPtr->key, __MapEntryPtr->value);
                }
                Iterator operator++() {
                    do {
                            __MapEntryPtr++;
                    } while (__MapEntryPtr->isEmpty() || __MapEntryPtr->tombstoned());
                    return *this;
                }
                Iterator operator++(int) {
                    Iterator tmp = *this;
                    ++*this;
                    return tmp;
                }
                bool operator==(const Iterator& o) const {
                    return __MapEntryPtr == o.__MapEntryPtr;
                }
                bool operator!=(const Iterator& o) const {
                    return !(*this==o);
                }
        };
    public:
        IterableMap(int max = 1013) {
            maxn = max;
            n = 0;
            table = new MapEntry[maxn];
            tombstones = 0;
        }
        IterableMap(const IterableMap& o) {
            for (auto it = o.begin(); it != o.end(); it++) {
                put((*it).first, (*it).second);
            }
        }
        ~IterableMap() {
            delete [] table;
        }
        void put(K key, V value) {
            if (loadfactor() > MAX_LOADFACTOR) {
                growAndRehash();
            }
            int idx = hasher()(key) % maxn;
            int m = 1;
            while (!table[idx].isEmpty()) {
                if (key == table[idx].key && !table[idx].tombstoned()) {
                    table[idx].value = value;
                    return;
                }
                idx = (idx + (m*m)) % maxn;
                m++;
            }
            table[idx].key = key;
            table[idx].value = value;
            table[idx].empty = false;
            n++;
        }
        int size() const {
            return n;
        }
        int maxsize() const {
            return maxn;
        }
        Iterator find(K key) {
            int idx = hashfn<K>()(key) % maxn;
            int m = 1;
            while (!table[idx].isEmpty()) {
                if (key == table[idx].key)
                    return  table[idx].tombstoned() ? end():Iterator(table+idx);
                idx = (idx + (m*m)) % maxn;
                m++;
            }
            return end();
        }
        void remove(K key) {
            int idx = hashfn<K>()(key) % maxn;
            int m = 1;
            bool found = false;
            while (!table[idx].isEmpty()) {
                if (key == table[idx].key && !table[idx].tombstoned()) {
                    found = true;
                    break;
                }
                idx = (idx + (m*m)) % maxn;
                m++;
            }
            if (found) {
                table[idx].tombstone = true;
                n--;
                tombstones++;
                if (((double)(n+tombstones)/maxn) > MAX_SHRINKFACTOR) {
                    shrinkAndRehash();
                }
            }
        }
        Iterator begin() {
            return Iterator(table);
        }
        Iterator end() {
            return Iterator(table+maxn);
        }
        const Iterator cbegin() const {
            return Iterator(table);
        }
        const Iterator cend() const {
            return Iterator(table+maxn);
        }
        V& operator[](K key) {
            if (find(key) == end()) {
                put(key, (V)0);
            }
            int idx = hasher()(key) % maxn;
            int m = 1;
            while (!table[idx].isEmpty()) {
                if (key == table[idx].key)
                    break;
                idx = (idx + (m*m)) % maxn;
                m++;
            }
            return table[idx].value;
        }
};

#endif