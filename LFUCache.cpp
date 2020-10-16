#include <list>
#include <unordered_map>

/**
 * Least Frequently Used (LFU) cache
 */

class LFUCache {
  public:
    typedef std::list<std::pair<int, int>> lru_list_t;
    typedef lru_list_t::iterator lru_list_iterator_t;

    typedef std::list<std::pair<int, lru_list_t>> lfu_list_t;
    typedef lfu_list_t::iterator lfu_list_iterator_t;
        
    typedef std::unordered_map<int,
        std::pair<lfu_list_iterator_t, lru_list_iterator_t>> hash_t;
    typedef hash_t::iterator hash_iterator_t;

    int size_, capacity_;
    lfu_list_t list_;
    hash_t hash_;

    LFUCache(int capacity): size_(0), capacity_(capacity), list_(), hash_() {
    }

    int get(int key) {
        int value;
        auto it = hash_.find(key);

        if (it != hash_.end()) {
            value = it->second.second->second;
            update(key, value, it);
        } else {
            value = -1;  
        }
        
        return value;
    }

    void put(int key, int value) {
        using std::make_pair;

        if (!capacity_) {
            return;
        }

        int frequency = 1;
        auto it = hash_.find(key);

        if (it != hash_.end()) {
            update(key, value, it);
        } else {
            if (++size_ > capacity_) {
                hash_.erase(list_.front().second.back().first);
                list_.front().second.pop_back();
                if (list_.front().second.empty()) {
                    list_.pop_front();
                }

                --size_;
            }

            if (list_.begin() != list_.end() && list_.begin()->first == frequency) {
                list_.begin()->second.push_front(make_pair(key, value));
            } else {
                list_.push_front(
                    make_pair(frequency, lru_list_t(1, make_pair(key, value))));
            }

            hash_.insert(make_pair(key,
                make_pair(list_.begin(), list_.begin()->second.begin())));
        }
    }

  private:
    void update(int key, int value, hash_iterator_t &it) {
        using std::make_pair;

        lfu_list_iterator_t lfu_new_it;
        lru_list_iterator_t lru_new_it;

        auto lfu_it  = it->second.first;
        auto lru_it  = it->second.second;
        auto next_fu = next(lfu_it);
        int frequency = lfu_it->first + 1;

        if (next_fu != list_.end() && next_fu->first == frequency) {
            next_fu->second.push_front(make_pair(key, value));

            lfu_new_it = next_fu;
            lru_new_it = next_fu->second.begin();
        } else {
            lfu_new_it = list_.emplace(next_fu,
                make_pair(frequency, lru_list_t(1, make_pair(key, value))));

            lru_new_it = lfu_new_it->second.begin();
        }
            
        lfu_it->second.erase(lru_it);
        if (lfu_it->second.empty()) {
            list_.erase(lfu_it);
        }

        hash_[key] = make_pair(lfu_new_it, lru_new_it);   
    }
};


/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param = obj->get(key);
 * obj->put(key,value);
 */

#include <iostream>

int main() {
    LFUCache* cache = new LFUCache(3 /* capacity */ );

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
    std::cout << cache->get(1) << '\n';     // returns 1
    std::cout << cache->get(3) << '\n';     // returns 3
    std::cout << cache->get(4) << '\n';     // returns -1 (not found)
    std::cout << cache->get(5) << '\n';     // returns 5

    return 0;
}
