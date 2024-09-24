#include "FIFOCore.hpp"

#include <iostream>
#include <string>

#include "Core.hpp"

// Constructor
FIFOCore::FIFOCore(int id) : Core(id), head_(nullptr), tail_(nullptr) {}

// Add a task to the end of the linked list (FIFO behavior)
void FIFOCore::AddTask(Task* task) {
  // Create a new TaskNode using the task pointer
  TaskNode* newTask = new TaskNode(task);

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
  pending_time_ += task->GetTime();
}

void FIFOCore::AssignTask(Task* task) {
  if (!task->IsAssigned()) {  // Check if the task is already assigned
    AddTask(task);            // Add the task using the new signature
    assigned_task_count_++;   // Increment assigned task count
    task->MarkAsAssigned();   // Mark the task as assigned
  }
}

// Destructor to free memory
FIFOCore::~FIFOCore() {
  TaskNode* current = head_;
  while (current) {
    TaskNode* next = current->next_;
    delete current;  // Only delete the TaskNode, not the Task object
    current = next;
  }
}
