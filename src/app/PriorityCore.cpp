#include "PriorityCore.hpp"

#include "Core.hpp"
// Constructor
PriorityCore::PriorityCore(int id) : Core(id), head_(nullptr) {}

// Override AddTask for priority behavior
void PriorityCore::AddTask(int task_time, int priority) {
  TaskNode* newTask = new TaskNode(task_time, priority);

  // If the list is empty or the new task has higher priority than the head
  if (!head_ || priority < head_->priority_) {
    newTask->next_ = head_;
    head_ = newTask;
  } else {
    // Find the correct position to insert the new task
    TaskNode* current = head_;
    while (current->next_ && current->next_->priority_ <= priority) {
      current = current->next_;
    }
    newTask->next_ = current->next_;
    current->next_ = newTask;
  }

  // Increase the pending time
  pending_time_ += task_time;
}

// Destructor to free memory
PriorityCore::~PriorityCore() {
  TaskNode* current = head_;
  while (current) {
    TaskNode* next = current->next_;
    delete current;
    current = next;
  }
}
