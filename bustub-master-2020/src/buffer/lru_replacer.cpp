//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {}

LRUReplacer::~LRUReplacer() = default;

/**
 * 根据LRU策略从LRU lst中找到牺牲页,(lst 最前面的元素是最早加进来的)
 * 1.如果没有可以牺牲的page直接返回false
 * 2.如果有的话选择在链表尾部的page, remove它即可. 这里的删除涉及链表和hash表两个数据结构的删除
 * 3.为了防止并发错误,以下操作均需要加锁
 */
bool LRUReplacer::Victim(frame_id_t *frame_id) {
    latch.lock();

    if(!list_t.empty()){
        frame_id_t lru_frame = list_t.back();
        hashmap_t.erase(lru_frame);
        list_t.pop_back();
        *frame_id = lru_frame; //return parameter
        latch.unlock();
        return true;
    }else {
        latch.unlock();
        return false;
    }
}

void LRUReplacer::Pin(frame_id_t frame_id) {
    latch.lock();
    if(hashmap_t.count(frame_id)){
        list_t.erase(hashmap_t[frame_id]);
        hashmap_t.erase(frame_id);
    }
    latch.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
    latch.lock();
    
    if(hashmap_t.count(frame_id)!=0){
        latch.unlock();
        return;
    }
    list_t.push_front(frame_id);
    hashmap_t[frame_id] = list_t.begin();
    latch.unlock();
}

size_t LRUReplacer::Size() {
    return hashmap_t.size();
}
}  // namespace bustub
