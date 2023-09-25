#ifndef ITERATOR_HPP
#define ITERATOR_HPP

template <class T>
class Iterator {
    private:
        T* m_ptr;
    public:
        Iterator(T* ptr) : m_ptr(ptr) { }
        T& operator*() {
            return *m_ptr;
        }
        Iterator operator++() {
            ++m_ptr;
            return *this;
        }
        Iterator operator++(int) {
            Iterator it = *this;
            ++*this;
            return it;
        }
        bool operator==(const Iterator& o) const {
            return m_ptr == o.m_ptr;
        }
        bool operator!=(const Iterator& o) const {
            return !(*this == o);
        }
};

#endif