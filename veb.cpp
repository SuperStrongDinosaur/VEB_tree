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
#include "veb.h"

using namespace std;

template<unsigned int S>
class VEBTree: AbstractVEBTree<S> {
private:
    class node {
        int size;
    public:
        shared_ptr<node> aux;
        vector<shared_ptr<node>> children;
        unsigned long long max = NO;
        unsigned long long min = NO;
        
        node(int size1) {
            size = size1;
            if (size < 3) {
            } else {
                int child_size = pow(2, (int)((log2(size)) / 2 + 0.5));
                aux = make_shared<node>(child_size);
                children.resize(child_size);
                for(auto& i : children)
                    i = make_shared<node>(child_size);
            }
        }
    };
    
    shared_ptr<node> root;
    int k;
    
public:
    VEBTree(int k) : k(k){
        root = make_shared<node>(pow(2, k));
    }
    
    void add(unsigned long long x) {
        add_rec(root, x);
    }
    
    void remove(unsigned long long x) {
        remove_rec(root, x);
    }
    
    unsigned long long next(unsigned long long x) const {
        return next_rec(root, x);
    }
    
    unsigned long long prev(unsigned long long x) const {
        return prev_rec(root, x);
    }
    
    unsigned long long getMin() const {
        return root->min;
    }
    
    unsigned long long getMax() const {
        return root->max;
    }
    
private:
    bool empty(shared_ptr<node> t) const {
        return t->min == NO;
    }
    
    unsigned long long high(unsigned long long key) const {
        return (key >> (k >> 1));
    }
    
    unsigned long long low(unsigned long long key) const {
        return (key & ((1L << (k >> 1)) - 1L));
    }

    unsigned long long merge(unsigned long long high, unsigned long long low) const {
        return (high << (k >> 1)) + low;
    }
    
    void add_rec(shared_ptr<node> t, unsigned long long x) {
        if (empty(t))  {
            t->min = x;
            t->max = x;
        } else if (t->min == t->max)   {
            if (t->min < x)
                t->max = x;
            else
                t->min = x;
        } else {
            if (t->min > x)
                swap(t->min, x);
            if (t->max < x)
                swap(t->max, x);
            if (k != 1) {
                if (empty(t->children[high(x)])) {
                    add_rec(t->aux, high(x));
                    add_rec(t->children[high(x)], low(x));
                }
            }
        }
    }
 
    void remove_rec(shared_ptr<node> t, unsigned long long x) {
        if (t->min == x && t->max == x)  {
            t->min = NO;
            return;
        }
       if (t->min == x) {
            if (empty(t->aux)) {
                t->min = t->max;
                return;
            }
            x = merge(t->aux->min, t->children[t->aux->min]->min);
            t->min = x;
        }
        if (t->max == x) {
            if (empty(t->aux)) {
                t->max = t->min;
                return;
            }
            x = merge(t->aux->max, t->children[t->aux->max]->max);
            t->max = x;
        }
        if (empty(t->aux))
            return;
        remove_rec(t->children[high(x)], low(x));
        if (empty(t->children[high(x)]))
            remove_rec(t->aux, high(x));
    }
    
    unsigned long long next_rec(shared_ptr<node> t, unsigned long long x) const {
        if (empty(t) || t->max <= x)
            return NO;
        if (t->min > x)
            return t->min;
        if (empty(t->aux))
            return t->max;
        else {
            if (!empty(t->children[high(x)]) && t->children[high(x)]->max > low(x))
                return merge(high(x), next_rec(t->children[high(x)], low(x)));
            else  {
                unsigned long long nextHigh = next_rec(t->aux, high(x));
                if (nextHigh == NO)
                    return t->max;
                else
                    return merge(nextHigh, t->children[nextHigh]->min);
            }
        }
    }
    
    unsigned long long prev_rec(shared_ptr<node> t, unsigned long long x) const {
        if (empty(t) || t->min >= x)
            return NO;
        if (t->max < x)
            return t->max;
        if (empty(t->aux))
            return t->min;
        else {
            if (!empty(t->children[high(x)]) && t->children[high(x)]->min > low(x))
                return merge(high(x), prev_rec(t->children[high(x)], low(x)));
            else  {
                unsigned long long nextHigh = prev_rec(t->aux, high(x));
                if (nextHigh == NO)
                    return t->min;
                else
                    return merge(nextHigh, t->children[nextHigh]->max);
            }
        }
    }
};

int main() {
    VEBTree<20> tree(9);
    tree.add(5);
    tree.add(11);
    tree.add(10);
    std::cout << tree.next(5) << std::endl;
    std::cout << tree.prev(11) << std::endl;
    tree.remove(10);
    std::cout << tree.next(5) << std::endl;
    return 0;
}
