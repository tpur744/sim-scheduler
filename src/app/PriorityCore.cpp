#include "PriorityCore.hpp"

#include <iostream>
#include <string>

#include "Core.hpp"

// Constructor
PriorityCore::PriorityCore(int id) : Core(id), head_(nullptr) {}

// Override AddTask for priority behavior
void PriorityCore::AddTask(Task* task) {
  TaskNode* newTask = new TaskNode(task);  // Use the pointer to Task

  // If the list is empty or the new task has higher priority than the head
  if (!head_ || task->GetPriority() < head_->task_->GetPriority()) {
    newTask->next_ = head_;
    head_ = newTask;
  } else {
    // Find the correct position to insert the new task
    TaskNode* current = head_;
    while (current->next_ &&
           current->next_->task_->GetPriority() <= task->GetPriority()) {
      current = current->next_;
    }
    newTask->next_ = current->next_;
    current->next_ = newTask;
  }

  // Increase the pending time
  pending_time_ += task->GetTime();
  // assigned_task_count_++;  // Increment assigned task count
}

void PriorityCore::AssignTask(Task* task) {
  if (!task->IsAssigned()) {  // Check if the task is already assigned
    AddTask(task);            // Increment assigned task count
    task->MarkAsAssigned();   // Mark the task as assigned
    assigned_task_count_++;
    std::cout << "assigned task count: " << assigned_task_count_ << std::endl;
    std::cout << "Assigning task to Priority core" << std::endl;
  }
}

// Destructor to free memory
PriorityCore::~PriorityCore() {
  TaskNode* current = head_;
  while (current) {
    TaskNode* next = current->next_;
    delete current;  // Clean up the current node
    current = next;  // Move to the next node
  }
}
