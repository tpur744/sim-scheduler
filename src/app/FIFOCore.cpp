#include "FIFOCore.hpp"

#include "Core.hpp"
// Constructor
FIFOCore::FIFOCore(int id) : Core(id), head_(nullptr), tail_(nullptr) {}

// Add a task to the end of the linked list (FIFO behavior)
void FIFOCore::addTask(int task_time, int priority) {
  TaskNode* newTask = new TaskNode(task_time, priority);

  if (!head_) {
    // If the list is empty, the new task is both head and tail
    head_ = newTask;
    tail_ = newTask;
  } else {
    // Add the new task to the end of the list
    tail_->next_ = newTask;
    tail_ = newTask;
  }

  // Increase the pending time
  pending_time_ += task_time;
}

// Destructor to free memory
FIFOCore::~FIFOCore() {
  TaskNode* current = head_;
  while (current) {
    TaskNode* next = current->next_;
    delete current;
    current = next;
  }
}
