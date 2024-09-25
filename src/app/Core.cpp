#include "Core.hpp"

#include <iostream>

#include "Task.hpp"
#include "TaskNode.hpp"
// Constructor to initialize ID and pending time
Core::Core(int id)
    : id_(id),
      pending_time_(0),
      assigned_task_count_(0),
      completed_task_count_(0) {}

// Get the total pending time
int Core::GetPendingTime() const { return pending_time_; }

// Increment completed tasks and decrement assigned tasks
void Core::CompleteTask() {
  if (assigned_task_count_ >
      0) {  // Ensure there's an assigned task to complete
    completed_task_count_++;
    assigned_task_count_--;
  }
}

// Get the count of assigned tasks
int Core::GetAssignedTaskCount() const { return assigned_task_count_; }

// Get the count of completed tasks
int Core::GetCompletedTaskCount() const { return completed_task_count_; }

// Virtual destructor
Core::~Core() {}

// Get the ID of the core
int Core::GetID() const { return id_; }

void Core::IncrementCompletedTaskCount() { completed_task_count_++; }

// Ticksfoward

// Get the head task, deduct pending execution time by one
// increment waiting time by one  for the other tasks
// if the task is complete, remove the task and print the task id and waiting
// time

void Core::TickForward() {
  // Check if there is a task at the head (i.e., a task being executed)
  if (head_) {
    Task* current_task = head_->task_;

    // Decrement the execution time for the current task
    current_task->DecrementTime();

    // If the task is finished (execution time reaches 0)
    if (current_task->GetTime() <= 0) {
      std::cout << "Task " << current_task->GetID()
                << " completed after waiting " << current_task->GetWaitingTime()
                << "." << std::endl;

      // Remove the completed task
      RemoveTask(current_task->GetID(), false);
    }
  }

  // For all other tasks in the queue, increment their waiting time
  TaskNode* current_node =
      head_ ? head_->next_ : nullptr;  // Start from the next task
  while (current_node) {
    current_node->task_
        ->IncrementWaitingTime();  // Increase waiting time for queued tasks
    current_node = current_node->next_;  // Move to the next task node
  }
}

void Core::RemoveTask(int id, bool print_output) {
  TaskNode* current_node = head_;
  TaskNode* previous_node = nullptr;

  // Traverse the list to find the task with the given ID
  while (current_node) {
    if (current_node->task_->GetID() == id) {
      // Remove the task from the linked list
      if (previous_node) {
        previous_node->next_ = current_node->next_;  // Bypass the current task
      } else {
        head_ = current_node->next_;  // Remove the head
      }

      // Get waiting time before deletion for output
      int waiting_time = current_node->task_->GetWaitingTime();
      bool was_executed = current_node->task_->GetExecutedTime() > 0;

      // Free memory
      delete current_node->task_;  // Free the task
      delete current_node;         // Free the TaskNode

      // Output status if print_output is true
      if (print_output) {
        std::cout << "Removed task " << id
                  << (was_executed ? " which was executed"
                                   : " which was not execute")
                  << " after waiting " << waiting_time << "." << std::endl;
      }
      return;
    }

    previous_node = current_node;        // Move to the next task node
    current_node = current_node->next_;  // Move to the next TaskNode
  }

  std::cout << "No task with ID " << id << " found." << std::endl;
}

Task* Core::GetTask(int id) {
  TaskNode* current_node = head_;
  while (current_node) {
    if (current_node->task_->GetID() == id) {
      return current_node->task_;  // Return the task if found
    }
    current_node = current_node->next_;
  }
  return nullptr;  // Task not found
}
