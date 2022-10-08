#pragma once

#include "base.h"

class LRUKReplacer {
   public:
    using frame_id_t = int32_t;

    struct DLinkedNode {
        size_t k_;
        frame_id_t id_;
        bool is_evictable_;
        std::shared_ptr<DLinkedNode> next_;
        std::weak_ptr<DLinkedNode> prev_;
        std::deque<std::chrono::system_clock::rep> records_;

        DLinkedNode(size_t k) : k_(k), is_evictable_(false) { update(); }

        void update() { records_.emplace_back(std::chrono::system_clock::now().time_since_epoch().count()); }

        int64_t getKDis() {
            if (records_.size() < k_) {
                return INT64_MAX;
            }
            while (records_.size() > k_) {
                records_.pop_front();
            }
            return records_.back() - records_.front();
        }
    };

    class DLinkedList {
       public:
        void print() {
            for (auto ptr = head_->next_; ptr != tail_; ptr = ptr->next_) {
                std::cout << "{" << ptr->id_ << ", " << ptr->is_evictable_ << ", " << ptr->getKDis() << "}"
                          << ", ";
            }
            std::cout << std::endl;
        }

        DLinkedList() {
            head_ = std::make_shared<DLinkedNode>(0);
            tail_ = std::make_shared<DLinkedNode>(0);
            head_->next_ = tail_;
            tail_->prev_ = head_;
        }

        ~DLinkedList() = default;

        bool removeNode(std::shared_ptr<DLinkedNode> node) {
            auto prev = node->prev_.lock();
            prev->next_ = node->next_;
            node->next_->prev_ = node->prev_;
            return true;
        }

        bool insertNode(std::shared_ptr<DLinkedNode> node) {
            std::shared_ptr<DLinkedNode> ptr;
            for (ptr = head_->next_; ptr != tail_; ptr = ptr->next_) {
                if (ptr->getKDis() > node->getKDis()) {
                    break;
                }
                if (ptr->getKDis() == node->getKDis() && ptr->records_.back() <= node->records_.back()) {
                    break;
                }
            }
            auto prev = ptr->prev_.lock();
            prev->next_ = node;
            node->prev_ = prev;
            node->next_ = ptr;
            ptr->prev_ = node;
            return true;
        }

        std::shared_ptr<DLinkedNode> removeTarget() {
            auto target = tail_->prev_.lock();
            for (; target != head_; target = target->prev_.lock()) {
                if (target->is_evictable_) {
                    break;
                }
            }
            if (target == head_) {
                return nullptr;
            }
            auto prev = target->prev_.lock();
            prev->next_ = target->next_;
            target->next_->prev_ = target->prev_;
            return target;
        }

       private:
        std::shared_ptr<DLinkedNode> head_;
        std::shared_ptr<DLinkedNode> tail_;
    };

    LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {
        list = std::make_shared<DLinkedList>();
    }

    auto Evict(frame_id_t *frame_id) -> bool {
        std::scoped_lock<std::mutex> lock(latch_);
        if (curr_size_ == 0) {
            return false;
        }
        std::shared_ptr<DLinkedNode> target = list->removeTarget();
        if (!target) {
            return false;
        }
        LRUCache.erase(target->id_);
        *frame_id = target->id_;
        curr_size_--;
        return true;
    }

    void RecordAccess(frame_id_t frame_id) {
        std::scoped_lock<std::mutex> lock(latch_);
        if (LRUCache.find(frame_id) == LRUCache.end()) {
            std::shared_ptr<DLinkedNode> node = std::make_shared<DLinkedNode>(k_);
            node->id_ = frame_id;
            list->insertNode(node);
            LRUCache[frame_id] = node;
        } else {
            LRUCache[frame_id]->update();
            list->removeNode(LRUCache[frame_id]);
            list->insertNode(LRUCache[frame_id]);
        }
    }

    void SetEvictable(frame_id_t frame_id, bool set_evictable) {
        std::scoped_lock<std::mutex> lock(latch_);
        if (LRUCache.find(frame_id) == LRUCache.end() || LRUCache[frame_id]->is_evictable_ == set_evictable) {
            return;
        }
        if (LRUCache[frame_id]->is_evictable_) {
            LRUCache[frame_id]->is_evictable_ = set_evictable;
            curr_size_--;
        } else {
            LRUCache[frame_id]->is_evictable_ = set_evictable;
            curr_size_++;
        }
    }

    void Remove(frame_id_t frame_id) {
        std::scoped_lock<std::mutex> lock(latch_);
        if (LRUCache.find(frame_id) == LRUCache.end()) {
            return;
        }
        if (!LRUCache[frame_id]->is_evictable_) {
            return;
        }
        list->removeNode(LRUCache[frame_id]);
        LRUCache.erase(frame_id);
        curr_size_--;
    }

    auto Size() -> size_t {
        std::scoped_lock<std::mutex> lock(latch_);
        return curr_size_;
    }

    ~LRUKReplacer() = default;

    void print() { list->print(); }

   private:
    size_t curr_size_{0};
    size_t replacer_size_;
    size_t k_;
    std::shared_ptr<DLinkedList> list;
    std::unordered_map<frame_id_t, std::shared_ptr<DLinkedNode>> LRUCache;
    std::mutex latch_;
};

void lru_k_repacer_test() {
    LRUKReplacer lru_replacer(7, 2);
    lru_replacer.RecordAccess(1);
    lru_replacer.RecordAccess(2);
    lru_replacer.RecordAccess(3);
    lru_replacer.RecordAccess(4);
    lru_replacer.RecordAccess(5);
    lru_replacer.RecordAccess(6);
    lru_replacer.SetEvictable(1, true);
    lru_replacer.SetEvictable(2, true);
    lru_replacer.SetEvictable(3, true);
    lru_replacer.SetEvictable(4, true);
    lru_replacer.SetEvictable(5, true);
    lru_replacer.SetEvictable(6, false);
    assert(5 == lru_replacer.Size());

    lru_replacer.RecordAccess(1);

    int value;
    lru_replacer.Evict(&value);
    assert(2 == value);
    lru_replacer.Evict(&value);
    assert(3 == value);
    lru_replacer.Evict(&value);
    assert(4 == value);
    assert(2 == lru_replacer.Size());

    lru_replacer.RecordAccess(3);
    lru_replacer.RecordAccess(4);
    lru_replacer.RecordAccess(5);
    lru_replacer.RecordAccess(4);
    lru_replacer.SetEvictable(3, true);
    lru_replacer.SetEvictable(4, true);
    assert(4 == lru_replacer.Size());

    lru_replacer.Evict(&value);
    assert(3 == value);
    assert(3 == lru_replacer.Size());

    lru_replacer.SetEvictable(6, true);
    assert(4 == lru_replacer.Size());
    lru_replacer.Evict(&value);
    assert(6 == value);
    assert(3 == lru_replacer.Size());

    lru_replacer.SetEvictable(1, false);
    assert(2 == lru_replacer.Size());
    assert(true == lru_replacer.Evict(&value));
    assert(5 == value);
    assert(1 == lru_replacer.Size());

    // Update access history for 1. Now we have [4,1]. Next victim is 4.
    lru_replacer.RecordAccess(1);
    lru_replacer.RecordAccess(1);
    lru_replacer.SetEvictable(1, true);
    assert(2 == lru_replacer.Size());
    assert(true == lru_replacer.Evict(&value));
    assert(value == 4);

    assert(1 == lru_replacer.Size());
    lru_replacer.Evict(&value);
    assert(value == 1);
    assert(0 == lru_replacer.Size());

    // These operations should not modify size
    assert(false == lru_replacer.Evict(&value));
    assert(0 == lru_replacer.Size());
    lru_replacer.Remove(1);
    assert(0 == lru_replacer.Size());
}