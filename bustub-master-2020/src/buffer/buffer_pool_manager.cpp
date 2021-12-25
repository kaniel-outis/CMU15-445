//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// buffer_pool_manager.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/buffer_pool_manager.h"
#include <cassert>
#include <algorithm>

namespace bustub {

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager), log_manager_(log_manager) {
  // We allocate a consecutive memory space for the buffer pool.
  pages_ = new Page[pool_size_];
  replacer_ = new LRUReplacer(pool_size);

  // Initially, every page is in the free list.
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<frame_id_t>(i));
  }
}

BufferPoolManager::~BufferPoolManager() {
  delete[] pages_;
  delete replacer_;
}

Page *BufferPoolManager::FetchPageImpl(page_id_t page_id) {
  // 1.     Search the page table for the requested page (P).
  // 1.1    If P exists, pin it and return it immediately.
  // 1.2    If P does not exist, find a replacement page (R) from either the freelist or the replacer.
  //        Note that pages are always found from the free list first.
  // 2.     If R is dirty, write it back to the disk.
  // 3.     Delete R from the page table and insert P.
  // 4.     Update P's metadata, read in the page content from disk, and then return a pointer to
    
    latch_.lock();

  // 1.search the page table
  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);
    
  frame_id_t frame_id;
    //page exists
  if (it != page_table_.end()) {
      frame_id = it->second; //find frame id
    Page *page_f = &pages_[frame_id]; //find page
    page_f->pin_count_++;
    replacer_->Pin(frame_id);
    latch_.unlock();
    return page_f;
  }
    //page not exists
  if (!free_list_.empty()) {    // free_list not empty, return directly
    frame_id = free_list_.front();
    free_list_.pop_front();
  } else {  // if can't find victim, return nullptr
      if((replacer_->Victim(&frame_id)) == false){
          latch_.unlock();
          return nullptr;
      }
    }
  
  // frame_id:Victim  ---> find page in page table
  Page *victim_page = &pages_[frame_id];
    // 2. dirty
  if (victim_page->IsDirty()) {
    disk_manager_->WritePage(victim_page->GetPageId(), victim_page->GetData());
      victim_page->pin_count_ = 0;
      victim_page->is_dirty_ = false;
  }
    //3. delete and insert
  page_table_.erase(victim_page->GetPageId());
  page_table_[page_id]=frame_id;
 
// 4.update page
  Page *newPage = victim_page;
  disk_manager_->ReadPage(page_id, newPage->GetData());
  newPage->page_id_ = page_id;
  newPage->pin_count_++;
  newPage->is_dirty_ = false;
  replacer_->Pin(frame_id);
  latch_.unlock();
  return newPage;
}

bool BufferPoolManager::UnpinPageImpl(page_id_t page_id, bool is_dirty) {
  latch_.lock();
    // 1. check page table
  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);

  if (it == page_table_.end()) {
    latch_.unlock();
    return true;
  }
    //2. exsit, find page
  frame_id_t unpinned_frame_id = it->second;
  Page *unpinned_page = &pages_[unpinned_frame_id];

  if (is_dirty) {
      unpinned_page->is_dirty_ = true;
  }
  if (unpinned_page->pin_count_ == 0) {
    latch_.unlock();
    return false;
  }
  unpinned_page->pin_count_--;

  if (unpinned_page->GetPinCount() == 0) {
    replacer_->Unpin(unpinned_frame_id);
  }
  latch_.unlock();
  return true;
}

bool BufferPoolManager::FlushPageImpl(page_id_t page_id) {
  // Make sure you call DiskManager::WritePage!
  
  latch_.lock();
  assert(page_id != INVALID_PAGE_ID);


  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);

  if (it == page_table_.end()) {
    latch_.unlock();
    return false;
  }

  Page *flush_page = &pages_[it->second];
  disk_manager_->WritePage(flush_page->GetPageId(), flush_page->GetData());
  flush_page->is_dirty_ = false;
    
  latch_.unlock();
  return true;
}

Page *BufferPoolManager::NewPageImpl(page_id_t *page_id) {
  // 0.   Make sure you call DiskManager::AllocatePage!
  // 1.   If all the pages in the buffer pool are pinned, return nullptr.
  // 2.   Pick a victim page P from either the free list or the replacer. Always pick from the
  // free list first.
  // 3.   Update P's metadata, zero out memory and add P to the page table.
  // 4.   Set the page ID output parameter. Return a pointer to P.
  latch_.lock();
    //1.
  bool all_pinned = true;
  for (int i = 0; i < static_cast<int>(pool_size_); i++) {
    if (pages_[i].GetPinCount() <= 0) {
      all_pinned = false;
      break;
    }
  }

  if (all_pinned) {
    latch_.unlock();
    return nullptr;
  }
    //2.find victim
  frame_id_t frame_id;

  if (!free_list_.empty()) {
      frame_id = free_list_.front();
    free_list_.pop_front();
  } else {
      if((replacer_->Victim(&frame_id)) == false){
          latch_.unlock();
          return nullptr;
      }
  }
  Page *victim_page = &pages_[frame_id];
      // 3. dirty
  if (victim_page->IsDirty()) {
    disk_manager_->WritePage(victim_page->GetPageId(), victim_page->GetData());
    victim_page->pin_count_ = 0;
    victim_page->is_dirty_ = false;
  }
    //4.
  page_table_.erase(victim_page->GetPageId());
  page_id_t new_page_id = disk_manager_->AllocatePage();
  victim_page->page_id_ = new_page_id;
  victim_page->pin_count_++;
  victim_page->ResetMemory();
  replacer_->Pin(frame_id);
  page_table_[new_page_id] = frame_id;
  *page_id = new_page_id;
  disk_manager_->WritePage(victim_page->GetPageId(), victim_page->GetData());
  latch_.unlock();
  return victim_page;
}

// delete a page from bufferPool
bool BufferPoolManager::DeletePageImpl(page_id_t page_id) {
  // 0.   Make sure you call DiskManager::DeallocatePage!
  // 1.   Search the page table for the requested page (P).
  // 1.   If P does not exist, return true.
  // 2.   If P exists, but has a non-zero pin-count, return false. Someone is using the page.
  // 3.   Otherwise, P can be deleted. Remove P from the page table, reset its metadata and return
  // it to the free list.
  latch_.lock();
    //1.
  std::unordered_map<page_id_t, frame_id_t>::iterator it = page_table_.find(page_id);

  if (it == page_table_.end()) {
    latch_.unlock();
    return true;
  }
    //2.
  frame_id_t frame_id = it->second;
  Page *page_f = &pages_[frame_id];
  if (page_f->GetPinCount()) {
    latch_.unlock();
    return false;
  }

  if (page_f->IsDirty()) {
    disk_manager_->WritePage(page_f->GetPageId(), page_f->GetData());
      page_f->is_dirty_ = false;
  }

  page_table_.erase(page_f->GetPageId());

  disk_manager_->DeallocatePage(page_id);

  page_f->ResetMemory();
  page_f->page_id_ = INVALID_PAGE_ID;
  page_f->pin_count_ = 0;
  page_f->is_dirty_ = false;

  free_list_.push_back(frame_id);
  latch_.unlock();
  return true;
}

void BufferPoolManager::FlushAllPagesImpl() {
  // You can do it!
  latch_.lock();

  for (size_t i = 0; i < pool_size_; i++) {
    Page *page = &pages_[i];
    if (page->page_id_ != INVALID_PAGE_ID && page->IsDirty()) {
      disk_manager_->WritePage(page->GetPageId(), page->GetData());
        page->is_dirty_ = false;
    }
  }
 latch_.unlock();
 return;
}

}  // namespace bustub
