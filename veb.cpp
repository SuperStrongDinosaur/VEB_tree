//
//  veb.cpp
//  proj
//
//  Created by Alex Shirvinsky on 01.04.17.
//  Copyright © 2017 Alex Shirvinsky. All rights reserved.
//

#include <vector>
#include <iostream>
#include <cmath>
#include <set>
#include "veb.h"
#define ull unsigned long long

template<unsigned int S>
class VEBTree: AbstractVEBTree<S> {
private:
    class node {
    public:
        int size;
        std::shared_ptr<node> aux;
        std::vector<std::shared_ptr<node>> children;
        ull max, min;
        
        node(int size1) : size(size1), max(NO), min(NO) {
            if (size > 2) {
                int child_size = (1 << (int)ceil(log(size) / (2 * log(2))));
                aux = std::make_shared<node>(child_size);
                children.resize(child_size);
                for(auto& i : children)
                    i = std::make_shared<node>(child_size);
            }
        }
    };
    
    std::shared_ptr<node> root;
    int k;
    
public:
    VEBTree() : k(S) {
        root = std::make_shared<node>(pow(2, k));
    }
    
    void add(ull x) {
        if(!find(x))
            add_rec(root, x);
    }
    
    void remove(ull x) {
        if(find(x))
            remove_rec(root, x);
    }
    
    bool find(ull x) const {
        return find_rec(root, x);
    }
    
    ull next(ull x) const {
        return next_rec(root, x);
    }
    
    ull prev(ull x) const {
        return prev_rec(root, x);
    }
    
    ull getMin() const {
        return root->min;
    }
    
    ull getMax() const {
        return root->max;
    }
    
private:
    bool empty(std::shared_ptr<node> t) const {
        return !t || t->min == NO;
    }
    
    inline int downRoot(int u) const {
        return (1 << (int)(log(u) / (2 * log(2))));
    }
    
    ull high(std::shared_ptr<node> t, ull key) const {
        //return (key >> ((t->size + 1) / 2));
        return key / downRoot(t->size);
    }
    
    ull low(std::shared_ptr<node> t, ull key) const {
        //return key & ((1ull << ((t->size + 1) / 2)) - 1);
        return key % downRoot(t->size);
    }
    
    ull merge(std::shared_ptr<node> t, ull high, ull low) const {
        //return (high << (((t->size) / 2 + 1))) | low;
        return high * downRoot(t->size) + low;
    }
    
    bool find_rec(std::shared_ptr<node> t, ull x) const {
        if (empty(t))
            return false;
        if (t->min == x || t->max == x)
            return true;
        return find_rec(t->children[high(t, x)], low(t, x));
    }
    
    void add_rec(std::shared_ptr<node>& t, ull x) {
        if (empty(t)) {
            t->min = x;
            t->max = x;
        } else if (t->min == t->max) {
            if (t->min > x)
                t->min = x;
            else
                t->max = x;
        } else {
            if (t->min > x)
                std::swap(t->min, x);
            if (t->max < x)
                std::swap(t->max, x);
            if (t->size > 2) {
                if(x != merge(t, high(t, x), low(t, x)))
                    std::cout << "ERROR" << x << " " << high(t, x) << " " << low(t, x) << std::endl;
                if (empty(t->children[high(t, x)]))
                    add_rec(t->aux, high(t, x));
                add_rec(t->children[high(t, x)], low(t, x));
            }
        }
    }
    
    void remove_rec(std::shared_ptr<node>& t, ull x) {
        if (t->min == x && t->max == x)  {
            t->min = NO;
            return;
        }
        if (t->min == x) {
            if (empty(t->aux)) {
                t->min = t->max;
                return;
            }
            x = merge(t, t->aux->min, t->children[t->aux->min]->min);
            t->min = x;
        }
        if (t->max == x) {
            if (empty(t->aux)) {
                t->max = t->min;
                return;
            }
            x = merge(t, t->aux->max, t->children[t->aux->max]->max);
            t->max = x;
        }
        if (empty(t->aux))
            return;
        remove_rec(t->children[high(t, x)], low(t, x));
        if (t->children.size() > high(t, x) && empty(t->children[high(t, x)]))
            remove_rec(t->aux, high(t, x));
    }
    
    ull next_rec(std::shared_ptr<node> t, ull x) const {
        if (empty(t) || t->max <= x)
            return NO;
        if (t->min > x)
            return t->min;
        if (empty(t->aux))
            return t->max;
        else {
            if (!empty(t->children[high(t, x)]) && t->children[high(t, x)]->max > low(t, x)) {
                return merge(t, high(t, x), next_rec(t->children[high(t, x)], low(t, x)));
            } else {
                ull nextHigh = next_rec(t->aux, high(t, x));
                if (nextHigh == NO)
                    return t->max;
                else
                    return merge(t, nextHigh, t->children[nextHigh]->min);
            }
        }
    }
    
    ull prev_rec(std::shared_ptr<node> t, ull x) const {
        if (empty(t) || t->min >= x)
            return NO;
        if (t->max < x)
            return t->max;
        if (empty(t->aux))
            return t->min;
        else {
            if (!empty(t->children[high(t, x)]) && t->children[high(t, x)]->min < low(t, x))
                return merge(t, high(t, x), prev_rec(t->children[high(t, x)], low(t, x)));
            else  {
                ull nextHigh = prev_rec(t->aux, high(t, x));
                if (nextHigh == NO)
                    return t->min;
                else
                    return merge(t, nextHigh, t->children[nextHigh]->max);
            }
        }
    }
};

int main() {
    VEBTree<10> tree;
    
    for(int i = 1; i < 500; i++) {
        tree.add(i);
        // std::cout << "added: " << i << " max: " << tree.getMax() << " is added: ";
        for(int j = 1; j <= i; j++)
            if(!tree.find(j))
                std::cout << j << " " << std::endl;
        //std::cout << std::endl;
    }
    for(int i = 1; i < 500; i++) {
        //std::cout << i << " is exist: " << tree.find(i) << " next: " << tree.next(i) << " prev: " << tree.prev(i) << std::endl;
        if(!tree.find(i) || tree.prev(i) != i - 1 || tree.next(i) - 1 != i)
            std::cout << i << " is exist: " << tree.find(i) << " next: " << tree.next(i) << " prev: " << tree.prev(i) << std::endl;
    }
    for(int i = 1; i < 500; i++) {
        tree.remove(i);
        //std::cout << "removed: " << i << " min: " << tree.getMin() << " is removed: " << tree.find(i) << std::endl;
    }
    
    tree.add(5);
    tree.add(11);
    tree.add(10);
    std::cout << tree.next(5) << std::endl;
    std::cout << tree.prev(11) << std::endl;
    tree.remove(10);
    std::cout << tree.next(5) << std::endl;
    VEBTree<20> a;
    std::set<ull> s;
    
    for (int i = 0; i < 1000; i++) {
        ull x = (rand() * RAND_MAX + rand());
        x %= 20000;
        a.add(x);
        s.insert(x);
        auto temp = s.find(x);
        auto curr = temp;
        auto temp2 = temp;
        auto next = ++temp;
        auto prev = --temp2;
        auto pp = a.prev(x);
        auto nn = a.next(x);
        if (curr != s.begin() && a.prev(x) != *prev) {
            std::cout << x << std::endl;
        }
        if (next != s.end() && a.next(x) != *next) {
            std::cout << x << std::endl;
        }
    }
    
    std::cout << "OK\n";
    
    return 0;
}
