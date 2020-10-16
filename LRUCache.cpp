#include <list>
#include <unordered_map> 

/**
 * Least Recently Used (LRU) cache
 */

class LRUCache {
public:
    typedef std::list<std::pair<int, int>> list_t;
    typedef list_t::iterator list_iterator_t;
    typedef std::unordered_map<int, list_iterator_t> hash_t;
    typedef hash_t::iterator hash_iterator_t;

    int size_, capacity_;
    list_t list_;
    hash_t hash_;

    LRUCache(int capacity): size_(0), capacity_(capacity), list_(), hash_() {
    }

    int get(int key) {
        int value;
        auto it = hash_.find(key);
            
        if (it != hash_.end()) {
            value = it->second->second;

            list_.erase(it->second);
            list_.push_front(std::make_pair(key, value));
            hash_[key] = list_.begin();
        } else {
            value = -1;  
        }
        
        return value;
    }

    void put(int key, int value) {
        if (!capacity_) {
            return;
        }

        auto it = hash_.find(key);

        if (it != hash_.end()) {
            list_.erase(it->second);
            list_.push_front(std::make_pair(key, value));
            hash_[key] = list_.begin();
        } else {
            if (++size_ > capacity_) {
                hash_.erase(list_.back().first);
                list_.pop_back();

                --size_;
            }

            list_.push_front(std::make_pair(key, value));
            hash_.insert(std::make_pair(key, list_.begin()));
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param = obj->get(key);
 * obj->put(key,value);
 */

#include <iostream>

int main() {
    LRUCache* cache = new LRUCache(3 /* capacity */ );

    cache->put(1, 1);
    cache->put(2, 2);
    cache->put(3, 3);
    std::cout << cache->get(1) << '\n';     // returns 1
    cache->put(4, 4);                       // evicts key 2
    std::cout << cache->get(1) << '\n';     // returns 1
    std::cout << cache->get(2) << '\n';     // returns -1 (not found)
    std::cout << cache->get(3) << '\n';     // returns 3
    std::cout << cache->get(4) << '\n';     // returns 4
    cache->put(3, 3);
    cache->put(5, 5);                       // evicts key 4
    std::cout << cache->get(1) << '\n';     // returns -1 (not found)
    std::cout << cache->get(3) << '\n';     // returns 3
    std::cout << cache->get(4) << '\n';     // returns 4
    std::cout << cache->get(5) << '\n';     // returns 5

    return 0;
}
