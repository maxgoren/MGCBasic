#ifndef LINKED_SET_HPP
#define LINKED_SET_HPP 

template <class T>
class linkedset;

template <class T>
linkedset<T> __setUnion(const linkedset<T>& a, const linkedset<T>& b);

template <class T>
linkedset<T> __setDifference(const linkedset<T>& a, const linkedset<T>& b);

template <class T>
linkedset<T> __setIntersection(const linkedset<T>& a, const linkedset<T>& b);

template <class T>
class linkedset {
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
        linkedset() {
            head = nullptr;
            count = 0;
        }
        linkedset(const linkedset<T>& oset) {
            for (auto it = oset.begin(); it != oset.end(); it++)
                add(*it);
        }
        ~linkedset() {
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
        linkedset<T> difference(const linkedset<T>& oset) {
            return __setDifference(*this, oset);
        }
        linkedset<T> setunion(const linkedset<T>& oset) {
            return __setUnion(*this, oset);
        }
        linkedset<T> intersection(const linkedset<T>& oset) {
            return __setIntersection(*this, oset);
        }
        Iterator begin() const {
            return Iterator(head);
        }
        Iterator end() const {
            return Iterator(nullptr);
        }
        linkedset operator=(const linkedset<T>& oset) {
            for (auto it = oset.begin(); it != oset.end(); it++)
                add(*it);
            return *this;
        }
};


template <class T>
linkedset<T> __setUnion(const linkedset<T>& aSet, const linkedset<T>& bSet) {
    linkedset<T> setU;
    for (auto it = aSet.begin(); it != aSet.end(); it++)
        setU.add(*it);
    for (auto it = bSet.begin(); it != bSet.end(); it++)
        setU.add(*it);
    return setU;
}

template <class T>
linkedset<T> __setDifference(const linkedset<T>& aSet, const linkedset<T>& bSet) {
    linkedset<T> setD;
    linkedset<T> *a, *b;
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
linkedset<T> __setIntersection(const linkedset<T>& aSet, const linkedset<T>& bSet) {
    linkedset<T> setI;
    for (auto it = aSet.begin(); it != aSet.end(); it++) {
        if (bSet.find(*it) != bSet.end()) {
            setI.add(*it);
        }
    }
    return setI;
}

#endif