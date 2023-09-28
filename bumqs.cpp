#include <iostream>
#include <queue>
#include <stack>
using namespace std;

struct node {
    int info;
    node* next;
};

struct List {
    int count;
    node* head;
    node* tail;
    List() {
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    ~List() {
   
    }
    bool empty() {
        return head == nullptr;
    }
    int length() {
        return count;
    }
    List& add(int info) {
        node* t = new node;
        t->info = info;
        if (empty()) {
            head = t;
            tail = t;
        } else {
            tail->next = t;
            tail = t;
        }
        count++;
        return *this;
    }
    List& add(node* t) {
        if (empty()) {
            head = t;
            tail = t;
        } else {
            tail->next = t;
            tail = t;
        }
        count++;
        return *this;
    }
    node* takeNodeFromFront() {
        if (empty())
            return nullptr;
        node* t = head;
        head = head->next;
        count -= 1;
        return t;
    }
    void print() {
        for (node* t = head; t != nullptr; t = t->next)
            cout<<t->info<<" ";
        cout<<endl;
    }
};

List merge(List a, List b) {
    List merged;
    while (!a.empty() && !b.empty()) {
        if (a.head->info < b.head->info) {
            node* t = a.takeNodeFromFront();
            merged.add(t);
        } else {
            node* t = b.takeNodeFromFront();
            merged.add(t);
        }
    }
    while (!a.empty()) {
        node* t = a.takeNodeFromFront();
        merged.add(t);
    }
    while (!b.empty()) {
        node* t = b.takeNodeFromFront();
        merged.add(t);
    }
    return merged;
}

void sort(List& list) {
    queue<List> mergequeue;
    while (!list.empty()) {
        node* t = list.takeNodeFromFront();
        t->next = nullptr;
        mergequeue.push(List().add(t));
    }
    while (!mergequeue.empty()) {
        List a = mergequeue.front(); mergequeue.pop();
        List b = mergequeue.front(); mergequeue.pop();
        list = merge(a, b);
        mergequeue.push(list);
    }
}

int main() {
    List list;
    for (int i = 0; i < 23; i++) {
        list.add(rand() % 78);
    }
    list.print();
    sort(list);
    list.print();
    return 0;
}