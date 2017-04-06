//
//  test.cpp
//  proj
//
//  Created by Alex Shirvinsky on 06.04.17.
//  Copyright © 2017 Alex Shirvinsky. All rights reserved.
//

#include "veb.cpp"
#include "assert.h"

using namespace std;

int w = 21;
long maxValue = (1L << w) - 1;

vector<ull> generate(ull n, ull mx) {
    vector<ull> a;
    for(size_t i = 0; i < n; i++) {
        ull x = abs((rand() * RAND_MAX + rand())) % mx;
        a.push_back(x);
    }
    return a;
}

bool contains(VEBTree<21>& set, long val) {
    if (val == 0) {
        return set.prev(val + 1) == val;
    }
    return set.next(val - 1) == val;
}

void test1_addRemovePrevNext() {
    cout << "testing addRemovePrevNext\n";
    int n = 10000;
    vector<ull> data = generate(n, n);
    VEBTree<21> veb;
    cout << "\tcontains done\n";
    for(auto i : data)
        veb.add(i);
    for (auto val : data) {
        if (val != 0) {
            assert(veb.next(val - 1) == val);
        }
        if (val != maxValue) {
            assert(veb.prev(val + 1) == val);
        }
    }
    cout << "\tcontains done\n";
    for(auto i : data)
        veb.remove(i);
    for (auto val : data) {
        assert(!contains(veb, val));
    }
   cout << "\tremove done\n";
}


void test2_randomContains() {
    cout << "testing randomContains\n";
    int n = 10000;
    VEBTree<21> veb;
    vector<ull> data = generate(n / 2, n);
    set<ull> set;
    for(auto i : data) {
        veb.add(i);
        set.insert(i);
    }
    for (int i = 0; i < n; i++) {
        ull val = abs((rand() * RAND_MAX + rand())) % n;
        assert(contains(veb, val) == (set.count(val) > 0));
    }
}

void test3_walk() {
    cout << "testing walk\n";
    int n = 10000;
    vector<ull> data1 = generate(n, n);
    set<ull> data;
    for(auto i : data1)
        data.insert(i);
    VEBTree<21> veb;
    for(auto i : data)
        veb.add(i);
    ull curr = veb.getMin();
    for (auto val : data) {
        assert(curr == val);
        curr = veb.next(curr);
    }
    cout << "\tstart to end done\n";
    assert(curr == NO);
    curr = veb.getMax();
    
    data1.clear();
    for(auto i : data) {
        data1.push_back(i);
    }
    reverse(data1.begin(), data1.end());
    for (long val : data1) {
        assert(curr == val);
        curr = veb.prev(curr);
    }
    cout << "\tend to start done\n";
}

int main() {
    test1_addRemovePrevNext();
    test2_randomContains();
    test3_walk();
    
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

/*long speedAdd(VEBTree<21> set, vector<ull> data) {
    long start = System.currentTimeMillis();
    data.forEach(set::add);
    return System.currentTimeMillis() - start;
}

long speedWalk(IntegerSet set) {
    long start = System.currentTimeMillis();
    long curr = set.getMin();
    while (curr != IntegerSet.NO) {
        curr = set.next(curr);
    }
    curr = set.getMax();
    while (curr != IntegerSet.NO) {
        curr = set.prev(curr);
    }
    return System.currentTimeMillis() - start;
}

long speedRemove(IntegerSet set, List<Long> data) {
    long start = System.currentTimeMillis();
    data.forEach(set::remove);
    return System.currentTimeMillis() - start;
}


void test4_performance() {
    System.out.println("testing performance, note: no assertions here, you decide if it is passed or not :)");
    int n = 5_000_000;
    List<Long> data = generate(n);
    System.gc();
    {
        IntegerSet veb = new VEBTree(w);
        long add = speedAdd(veb, data);
        System.out.format("\tVEB add: %d ms\n", add);
        long walk = speedWalk(veb);
        System.out.format("\tVEB walk: %d ms\n", walk);
        long remove = speedRemove(veb, data);
        System.out.format("\tVEB remove: %d ms\n", remove);
    }
    System.gc();
    {
        IntegerSet set = new IntegerTreeSet();
        long add = speedAdd(set, data);
        System.out.format("\tTreeSet add: %d ms\n", add);
        long walk = speedWalk(set);
        System.out.format("\tTreeSet walk: %d ms\n", walk);
        long remove = speedRemove(set, data);
        System.out.format("\tTreeSet remove: %d ms\n", remove);
    }
    
}*/
