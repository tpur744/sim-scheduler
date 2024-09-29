#include "FIFOCore.hpp"

#include <iostream>
#include <string>

#include "Core.hpp"
#include "message.hpp"

// Constructor
FIFOCore::FIFOCore(int id) : Core(id), tail_(nullptr) {}

// Add a task to the end of the linked list (FIFO behavior)
void FIFOCore::AddTask(Task* task) {
  // Create a new TaskNode using the task pointer
  TaskNode* new_task = new TaskNode(task);

  if (!head_) {
    // If the list is empty, the new task is both head and tail
    head_ = new_task;
    tail_ = new_task;
  } else {
    // Add the new task to the end of the list
    tail_->next_ = new_task;
    tail_ = new_task;
  }

  // Increase the pending time
  pending_time_ += task->GetTime();
  assigned_task_count_++;
  std::cout << "Added task with ID " << task->GetID() << ", task time of "
            << task->GetTime() << ", and priority of " << task->GetPriority()
            << "." << std::endl;
}

// REMOVE THIS CODE AT SOME POINT
void FIFOCore::AssignTask(Task* task) {
  if (!task->IsAssigned()) {  // Check if the task is already assigned
    AddTask(task);            // Increment assigned task count
    task->MarkAsAssigned();   // Mark the task as assigned
    assigned_task_count_++;
  }
}
// END REMOVE

// Destructor to free memory
FIFOCore::~FIFOCore() {
  TaskNode* current = head_;
  while (current) {
    TaskNode* next = current->next_;
    delete current;  // Only delete the TaskNode, not the Task object
    current = next;
  }
}
