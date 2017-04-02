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
#define ull unsigned long long

template<unsigned int S>
class VEBTree: AbstractVEBTree<S> {
private:
    class node {
        int size;
    public:
        std::shared_ptr<node> aux;
        std::vector<std::shared_ptr<node>> children;
        ull max, min;
        
        node(int size1) : size(size1), max(NO), min(NO) {
            if (size > 2) {
                int child_size = pow(2, (int)((log2(size)) / 2 + 0.5));
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
    VEBTree() : k(S){
        root = std::make_shared<node>(pow(2, k));
    }
    
    void add(ull x) {
        add_rec(root, x);
    }
    
    void remove(ull x) {
        remove_rec(root, x);
    }
    
    bool find(ull x) {
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
        return t->min == NO;
    }
    
    ull high(ull key) const {
        return (key >> (k >> 1));
    }
    
    ull low(ull key) const {
        return (key & ((1ull << (k >> 1)) - 1));
    }
    
    ull merge(ull high, ull low) const {
        return ((high << (k >> 1)) | low);
    }
    
    bool find_rec(std::shared_ptr<node> t, ull x) {
        if (empty(t))
            return false;
        if (t->min == x || t->max == x)
            return true;
        return find_rec(t->children[high(x)], low(x));
    }
    
    void add_rec(std::shared_ptr<node> t, ull x) {
        if (empty(t)) {
            t->min = x;
            t->max = x;
        } else if (t->min == t->max) {
            if (t->min < x)
                t->max = x;
            else
                t->min = x;
        } else {
            if (t->min > x)
                std::swap(t->min, x);
            if (t->max < x)
                std::swap(t->max, x);
            if (k != 1) {
                if (empty(t->children[high(x)])) {
                    add_rec(t->aux, high(x));
                    add_rec(t->children[high(x)], low(x));
                }
            }
        }
    }
    
    void remove_rec(std::shared_ptr<node> t, ull x) {
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
    
    ull next_rec(std::shared_ptr<node> t, ull x) const {
        if (empty(t) || t->max <= x)
            return NO;
        if (t->min > x)
            return t->min;
        if (empty(t->aux))
            return t->max;
        else {
            if (!empty(t->children[high(x)]) && t->children[high(x)]->max > low(x)) {
                return merge(high(x), next_rec(t->children[high(x)], low(x)));
            } else {
                ull nextHigh = next_rec(t->aux, high(x));
                if (nextHigh == NO)
                    return t->max;
                else
                    return merge(nextHigh, t->children[nextHigh]->min);
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
            if (!empty(t->children[high(x)]) && t->children[high(x)]->min > low(x))
                return merge(high(x), prev_rec(t->children[high(x)], low(x)));
            else  {
                ull nextHigh = prev_rec(t->aux, high(x));
                if (nextHigh == NO)
                    return t->min;
                else
                    return merge(nextHigh, t->children[nextHigh]->max);
            }
        }
    }
};

int main() {
    VEBTree<20> tree;
    
    for(int i = 1; i < 10; i++) {
        tree.add(i);
        std::cout << "added: " << i << " max: " << tree.getMax() << " is added: " << tree.find(i) <<std::endl;
    }
    for(int i = 1; i < 10; i++) {
        std::cout << "is exist: " << tree.find(i) << " next: " << tree.next(i) << " prev: " << tree.prev(i) << std::endl;
    }
    for(int i = 1; i < 10; i++) {
        tree.remove(i);
        std::cout << "removed: " << i << " min: " << tree.getMin() << " is removed: " << tree.find(i) << std::endl;
    }
    
    /* tree.add(5);
     tree.add(11);
     tree.add(10);
     std::cout << tree.next(5) << std::endl;
     std::cout << tree.prev(11) << std::endl;
     tree.remove(10);
     std::cout << tree.next(5) << std::endl;*/
    return 0;
}
