#ifndef LINKED_SET_HPP
#define LINKED_SET_HPP 

template <class T>
class LinkedSet;

template <class T>
LinkedSet<T> __setUnion(const LinkedSet<T>& a, const LinkedSet<T>& b);

template <class T>
LinkedSet<T> __setDifference(const LinkedSet<T>& a, const LinkedSet<T>& b);

template <class T>
LinkedSet<T> __setIntersection(const LinkedSet<T>& a, const LinkedSet<T>& b);

template <class T>
class LinkedSet {
    private:
        struct node {
            T info;
            node* next;
            node(T i) {
                info = i;
                next = nullptr;
            }
        };
        typedef node* link;
        link head;
        int count;
    public:
        class Iterator {
            private:
                node* curr;
            public:
                Iterator(node* h) {
                    curr = h;
                }
                Iterator() {
                
                }
                T& operator*() {
                    return curr->info;
                }
                Iterator& operator++() {
                    if (curr)
                        curr = curr->next;
                    return *this;
                }
                Iterator operator++(int) {
                    Iterator tmp = *this;
                    ++*this;
                    return tmp;
                }
                bool operator==(const Iterator& o) const noexcept {
                    return curr == o.curr;
                }
                bool operator!=(const Iterator& o) const noexcept {
                    return !(*this == o);
                }
                Iterator& operator=(node* h) {
                    curr = h;
                    return *this;
                }
        };
    private:
        link insertUnique(link h, T key) {
            if (h == nullptr) {
                count++;
                return new node(key);
            }
            if (key == h->info)
                return h;
            h->next = insertUnique(h->next, key);
            return h;
        }
        link erase(link h, T key) {
            if (h == nullptr)
                return h;
            if (key == h->info) {
                node* t = h;
                h = h->next; 
                count--;
                delete t;
            } else {
                h->next = erase(h->next, key);
            }
            return h;
        }
        Iterator find(link h, T key) const {
            if (h == nullptr)
                return end();
            if (key == h->info)
                return Iterator(h);
            return find(h->next, key);
        }
    public:
        LinkedSet() {
            head = nullptr;
            count = 0;
        }
        LinkedSet(const LinkedSet<T>& oset) {
            for (auto it = oset.begin(); it != oset.end(); it++)
                add(*it);
        }
        ~LinkedSet() {
            while (head != nullptr) {
                node* t = head;
                head = head->next;
                delete t;
            }
        }
        bool empty() const {
            return head == nullptr;
        }
        int size() const {
            return count;
        }
        void add(T item) {
            head = insertUnique(head, item);
        }
        void remove(T item) {
            head = erase(head, item);
        }
        Iterator find(T key) const {
            return find(head, key);
        }
        LinkedSet<T> difference(const LinkedSet<T>& oset) {
            return __setDifference(*this, oset);
        }
        LinkedSet<T> setunion(const LinkedSet<T>& oset) {
            return __setUnion(*this, oset);
        }
        LinkedSet<T> intersection(const LinkedSet<T>& oset) {
            return __setIntersection(*this, oset);
        }
        Iterator begin() const {
            return Iterator(head);
        }
        Iterator end() const {
            return Iterator(nullptr);
        }
        LinkedSet operator=(const LinkedSet<T>& oset) {
            for (auto it = oset.begin(); it != oset.end(); it++)
                add(*it);
            return *this;
        }
};


template <class T>
LinkedSet<T> __setUnion(const LinkedSet<T>& aSet, const LinkedSet<T>& bSet) {
    LinkedSet<T> setU;
    for (auto it = aSet.begin(); it != aSet.end(); it++)
        setU.add(*it);
    for (auto it = bSet.begin(); it != bSet.end(); it++)
        setU.add(*it);
    return setU;
}

template <class T>
LinkedSet<T> __setDifference(const LinkedSet<T>& aSet, const LinkedSet<T>& bSet) {
    LinkedSet<T> setD;
    LinkedSet<T> *a, *b;
    for (auto it = aSet.begin(); it != aSet.end(); it++) {
        if (bSet.find(*it) == bSet.end()) {
            setD.add(*it);
        }
    }
    for (auto it = bSet.begin(); it != bSet.end(); it++) {
        if (aSet.find(*it) == aSet.end()) {
            setD.add(*it);
        }
    }
    return setD;
}

template <class T>
LinkedSet<T> __setIntersection(const LinkedSet<T>& aSet, const LinkedSet<T>& bSet) {
    LinkedSet<T> setI;
    for (auto it = aSet.begin(); it != aSet.end(); it++) {
        if (bSet.find(*it) != bSet.end()) {
            setI.add(*it);
        }
    }
    return setI;
}

#endif