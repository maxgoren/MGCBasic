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

#ifndef AVLMAP_HPP
#define AVLMAP_HPP
#include <iostream>
using namespace std;

template <class T, class T2>
class avlmap {
    private:
        typedef pair<T,T2> entry;
        struct node {
            entry data;
            int height;
            node* left;
            node* right;
            node* parent;
            node(entry p) {
                data = p; height = 0; left = right = parent = nullptr;
            }
        };
    public:
        class avlIterator {
            private:
                node* curr;
            public:
                avlIterator(node* c) {
                    curr = c;
                }
                entry& operator*() {
                    return curr->data;
                }
                avlIterator& operator++() noexcept {
                   node* p;
                    if (curr->right != nullptr) {
                        curr = curr->right;
                        while (curr->left != nullptr) curr = curr->left;
                    } else {
                        p = curr->parent;
                        while (p != nullptr && curr == p->right) {
                            curr = p;
                            p = p->parent;
                        }
                        curr = p;
                    }
                   return *this;
                }
                avlIterator operator++(int) noexcept {
                    avlIterator it = *this;
                    ++*this;
                    return it;
                }
                bool operator==(const avlIterator& o) const {
                    return this->curr == o.curr;
                }
                bool operator!=(const avlIterator& o) const {
                    return !(*this==o);

                }
        };
    private:
        node* root;
        int N;
        int height(node* h) {
            return (h == nullptr) ? -1:h->height;
        }
        int max(int a, int b) { return (a < b) ? b:a; }
        void left_rotate(node *x) {
            node *y = x->right;
            if (y) {
                x->right = y->left;
                if (y->left) y->left->parent = x;
                y->parent = x->parent;
            }
            if (!x->parent) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;
            if (y) y->left = x;
            x->parent = y;
            x->height = 1 + max(height(x->left), height(x->right));
            y->height = 1 + max(height(y->left), height(y->right));
        }
  
        void right_rotate(node *x) {
            node *y = x->left;
            if (y) {
                x->left = y->right;
                if (y->right) y->right->parent = x;
                y->parent = x->parent;
            }
            if (!x->parent) root = y;
            else if (x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;
            if (y) y->right = x;
            x->parent = y;
            x->height = 1 + max(height(x->left), height(x->right));
            y->height = 1 + max(height(y->left), height(y->right));
        } 
        int balanceFactor(node* h) {
            return height(h->left) - height(h->right);
        }
        void balance(node* x) {
            while (x && x->parent) {
                node *y = x->parent;
                y->height = 1 + max(height(y->left), height(y->right));
                if (balanceFactor(y) < -1) {
                    if (balanceFactor(y->right) > 0) {
                        right_rotate(y->right);
                    }
                    left_rotate(y);
                } else if (balanceFactor(y) > 1) {
                    if (balanceFactor(y->left) < 0) {
                        left_rotate(y->left);
                    }
                    right_rotate(y);
                }
                x = x->parent;
            }
        }
        void transplant(node *u, node* v) {
            if (u->parent == nullptr) {
                root = v;
            } else if (u == u->parent->left) {
                u->parent->left = v;
            } else {
                u->parent->right = v;
            }
            if (v != nullptr) {
                v->parent = u->parent;
            }
        }
        void eraseR(node* h) {
            node* t = h;
            if (h->left == nullptr)
                transplant(h, h->right);
            else if (h->right == nullptr) {
                transplant(h, h->left);
            } else {
                node* y = min(h->right);
                if (y->parent != h) {
                    transplant(y, y->right);
                    y->right = h->right;
                    y->right->parent = y;
                }
                transplant(h, y);
                y->left = h->left;
                y->left->parent = y;
            }
            delete t;
        }
        node* min(node* h) {
            node* x = h;
            while (x->left != nullptr) x = x->left;
            return x;
        }
        void inorder(node* h, T keys[], int& n) {
            if (h == nullptr) return;
            inorder(h->left, keys, n);
            keys[n++] = h->data.first;
            cout<<h->data.first<<" ";
            inorder(h->right, keys, n);
        }
        void cleanup(node* h) { 
            if (h != nullptr) {
                cleanup(h->left);
                cleanup(h->right);
                delete h;
            }
        }
    public:
        avlmap() {
            root = nullptr;
            N = 0;
        }
        avlmap(const avlmap& o) {
            root = nullptr;
            for (pair<T,T2> t : o) {
                put(t.first, t.second);
            }
        }
        ~avlmap() {
            cleanup(root);
        }
        void put(T key, T2 value) {
            node* x = root;
            node* p = x;
            while (x) {
                p = x;
                if (key == x->data.first) {
                    x->data.second = value;
                    return;
                }
                x = (key < x->data.first) ? x->left:x->right;
            }
            x = new node(make_pair(key, value));
            if (!p) root = x;
            else if (key < p->data.first) p->left = x;
            else p->right = x;
            x->parent = p;
            balance(x);
            N++;
        }
        void erase(T key) {
            node *x = root;
            bool found = false;
            while (x != nullptr) {
                if (x->data.first == key) {
                    found = true;
                    break;
                }
                x = (key < x->data.first) ? x->left:x->right;
            }
            if (found) {
                eraseR(x);
                balance(x);
                N--;
            }
        }
        bool contains(T key) {
            node* x = root;
            while (x != nullptr) {
                if (key == x->data.first)
                    return true;
                x = (key < x->data.first) ? x->left:x->right;
            }
            return false;
        }
        avlIterator find(T key) {
            node *x = root;
            while (x != nullptr) {
                if (key == x->data.first)
                    return avlIterator(x);
                x = (key < x->data.first) ? x->left:x->right;
            }
            return end();
        }
        bool empty() const {
            return root == nullptr;
        }
        int size() const {
            return N;
        }
        void clear() {
            cleanup(root);
            root = nullptr;
            N = 0;
        }
        avlIterator begin() noexcept {
            node* x = root;
            while (x->left != nullptr) x = x->left;
            return avlIterator(x);
        }
        avlIterator end() {
            return avlIterator(nullptr);
        }
        const avlIterator begin() const noexcept {
            node* x = root;
            while (x->left != nullptr) x = x->left;
            return avlIterator(x);
        }
        const avlIterator end() const {
            return avlIterator(nullptr);
        }
        T* sort() {
            T *keys = new T[N];
            int id = 0;
            inorder(root, keys, id);
            cout<<endl;
            return keys;
        }
        T2& operator[](T key) {
            if (!contains(key))
                put(key, T2());
            node* x = root;
            while (x != nullptr) {
                if (x->data.first == key)
                    break;
                x = (key < x->data.first) ? x->left:x->right;
            }
            return x->data.second;
        }
        avlmap& operator=(const avlmap& o) {
            root = nullptr;
            for (auto t : o) {
                put(t.first, t.second);
            }
            return *this;
        }
};

#endif