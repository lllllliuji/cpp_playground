#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

class Bucket {
   public:
    Bucket(size_t sz, size_t depth = 0) : size_(sz), depth_(depth) {}
    bool insert(const int& key, const std::string& value) {
        auto iter = std::find_if(list_.begin(), list_.end(),
                                 [&](const std::pair<int, std::string>& item) { return item.first == key; });
        if (iter == list_.end()) {
            list_.emplace_back(std::make_pair(key, value));
        } else {
            iter->second = value;
        }
        return true;
    }
    bool remove(const int& key) {
        auto iter = std::find_if(list_.begin(), list_.end(),
                                 [&](const std::pair<int, std::string>& item) { return item.first == key; });
        if (iter == list_.end()) {
            return false;
        }
        list_.erase(iter);
        return true;
    }
    bool find(const int& key, std::string& value) {
        auto iter = std::find_if(list_.begin(), list_.end(),
                                 [&](const std::pair<int, std::string>& item) { return item.first == key; });
        if (iter == list_.end()) {
            return false;
        }
        value = iter->second;
        return true;
    }
    void print() {
        std::for_each(list_.begin(), list_.end(),
                      [](const auto& item) { std::cout << "{" << item.first << ", " << item.second << "} "; });
        std::cout << std::endl;
    }
    auto getItems() -> std::list<std::pair<int, std::string>>& { return list_; }
    void incrementDepth() { this->depth_++; }
    bool isFull() { return size_ == list_.size(); }
    size_t size() { return list_.size(); }
    size_t getDepth() { return depth_; }

   private:
    std::list<std::pair<int, std::string>> list_;
    size_t depth_;
    size_t size_;

   private:
    auto find_iter(const int& key) {
        return std::find_if(list_.begin(), list_.end(), [&](auto&& item) { return item.first == key; });
    }
};
class extendible_hashtable {
   public:
    extendible_hashtable(size_t bucket_size) : bucket_size_(bucket_size), global_depth_(0), num_buckets_(1) {
        dirs_.emplace_back(std::make_shared<Bucket>(bucket_size_));
    }
    size_t IndexOf(const int& key) {
        int mask = (1 << global_depth_) - 1;
        return std::hash<int>()(key) & mask;
    }
    size_t getGlobalDepth() { return global_depth_; }
    size_t getLocalDepth(int index) { return dirs_.at(index)->getDepth(); }
    bool remove(const int& key) {
        std::scoped_lock<std::mutex> lock(latch_);
        int index = IndexOf(key);
        std::shared_ptr<Bucket> target_bucket = dirs_.at(index);
        return target_bucket->remove(key);
    }
    bool find(const int& key, std::string& value) {
        std::scoped_lock<std::mutex> lock(latch_);

        int index = IndexOf(key);
        std::shared_ptr<Bucket> target_bucket = dirs_.at(index);
        return target_bucket->find(key, value);
    }
    bool insert(const int& key, const std::string& value) {
        std::scoped_lock<std::mutex> lock(latch_);
        int index = IndexOf(key);
        std::shared_ptr<Bucket> target_bucket = dirs_.at(index);
        while (target_bucket->isFull()) {
            size_t local_depth = target_bucket->getDepth();
            if (local_depth == global_depth_) {
                size_t length = dirs_.size();
                for (size_t i = 0; i < length; i++) {
                    dirs_.emplace_back(dirs_.at(i));
                }
                global_depth_++;
            }
            int mask = 1 << target_bucket->getDepth();
            auto new_bucket = std::make_shared<Bucket>(bucket_size_, local_depth + 1);
            auto& new_list = new_bucket->getItems();
            auto& old_list = target_bucket->getItems();
            auto divide_point =
                std::partition(old_list.begin(), old_list.end(),
                               [&](const std::pair<int, std::string>& item) { return IndexOf(item.first) & mask; });
            new_list.splice(new_list.end(), old_list, old_list.begin(), divide_point);
            for (size_t i = 0; i < dirs_.size(); i++) {
                if (dirs_.at(i) == target_bucket) {
                    if (i & mask) {
                        dirs_.at(i) = new_bucket;
                    }
                }
            }
            num_buckets_++;
            target_bucket->incrementDepth();
            index = IndexOf(key);
            target_bucket = dirs_.at(index);
        }
        return target_bucket->insert(key, value);
    }
    void print() {
        int i = 0;
        std::for_each(dirs_.begin(), dirs_.end(), [&](const auto& ptr) {
            std::cout << i++ << " : ";
            ptr->print();
        });
        std::cout << std::endl;
    }

   private:
    size_t global_depth_;
    size_t bucket_size_;
    int num_buckets_;
    mutable std::mutex latch_;
    std::vector<std::shared_ptr<Bucket>> dirs_;
};

void extendibleHashtableTest() {
    auto table = std::make_unique<extendible_hashtable>(2);

    table->insert(1, "a");
    table->insert(2, "b");
    table->insert(3, "c");
    table->insert(4, "d");
    table->insert(5, "e");
    table->insert(6, "f");
    table->insert(7, "g");
    table->insert(8, "h");
    table->insert(9, "i");

    table->print();

    assert(2 == table->getLocalDepth(0));
    assert(3 == table->getLocalDepth(1));
    assert(2 == table->getLocalDepth(2));
    assert(2 == table->getLocalDepth(3));

    std::string result;
    table->find(9, result);
    assert("i" == result);
    table->find(8, result);
    assert("h" == result);
    table->find(2, result);
    assert("b" == result);
    assert(false == table->find(10, result));

    assert(true == table->remove(8));
    assert(true == table->remove(4));
    assert(true == table->remove(1));
    assert(false == table->remove(20));

    table->print();
}
void extendibleHashTableConcurrentTest() {
    const int num_runs = 500;
    const int num_threads = 3;

    // Run concurrent test multiple times to guarantee correctness.
    for (int run = 0; run < num_runs; run++) {
        auto table = std::make_unique<extendible_hashtable>(2);
        std::vector<std::thread> threads;
        threads.reserve(num_threads);

        for (int tid = 0; tid < num_threads; tid++) {
            threads.emplace_back([tid, &table]() { table->insert(tid, std::to_string(tid)); });
        }
        for (int i = 0; i < num_threads; i++) {
            threads[i].join();
        }

        assert(table->getGlobalDepth() == 1);
        for (int i = 0; i < num_threads; i++) {
            std::string val;
            assert(table->find(i, val) == true);
            assert(val == std::to_string(i));
        }
    }

}