#include "SimScheduler.hpp"

#include <iostream>
#include <string>

#include "FIFOCore.hpp"
#include "PriorityCore.hpp"
#include "Task.hpp"
#include "TaskNode.hpp"

// Constructor
SimScheduler::SimScheduler()
    : scheduler_added_(false),
      core_count_(0),
      task_list_head_(nullptr),
      task_counter_(0) {
  for (int i = 0; i < 8; ++i) {
    cores_[i] = nullptr;  // Initialize pointers to null
  }
}

// Destructor
SimScheduler::~SimScheduler() {
  RemoveScheduler();  // Ensure scheduler is removed on destruction
  for (int i = 0; i < core_count_; i++) {
    delete cores_[i];  // Clean up allocated cores
  }
}

// Method to add the scheduler
void SimScheduler::AddScheduler() {
  if (!scheduler_added_) {
    scheduler_added_ = true;  // Update the flag
  }
}

bool SimScheduler::HasCores() const {
  for (int i = 0; i < core_count_; ++i) {
    if (cores_[i] != nullptr) {
      return true;  // Found a core that still exists
    }
  }
  return false;  // No active cores
}

// Method to remove the scheduler
bool SimScheduler::RemoveScheduler() {
  if (scheduler_added_) {
    if (HasCores()) {
      return false;  // Cannot remove scheduler while cores are present
    }

    // Free memory for each core
    for (int i = 0; i < core_count_; ++i) {
      delete cores_[i];
      cores_[i] = nullptr;  // Set to nullptr after deletion
    }
    core_count_ = 0;           // Reset core count
    scheduler_added_ = false;  // Mark the scheduler as removed
    return true;               // Indicate success
  }
  return false;  // Indicate no scheduler was present
}

// Method to check if the scheduler is added
bool SimScheduler::IsSchedulerAdded() const { return scheduler_added_; }

// Method to add a core
bool SimScheduler::AddCore(Core* core) {
  if (core_count_ < 8) {  // Check if there's space for more cores
    cores_[core_count_] = core;
    core_count_++;
    std::cout << "adding core to address " << core << std::endl;
    return true;  // Indicate successful addition
  }
  return false;  // Indicate failure to add core
}

int SimScheduler::GetNextCoreID() const {
  return core_count_;  // Returns the next available ID based on the current
                       // count
}

Core* SimScheduler::GetCore(int id) const {
  std::cout << "returning core at address " << cores_[id] << std::endl;
  return cores_[id];  // Return the core at the given index
}

bool SimScheduler::RemoveCore(int core_id) {
  if (core_id < 0 || core_id >= core_count_) {
    return false;  // Invalid core ID
  }

  if (cores_[core_id] != nullptr) {
    delete cores_[core_id];     // Free the memory for the core
    cores_[core_id] = nullptr;  // Mark the core as removed
    return true;                // Indicate success
  }

  return false;  // Core was already removed
}

int SimScheduler::AddTask(int time, int priority, int arrival_time) {
  // Create a new Task object
  Task* new_task = new Task(task_counter_, time, priority, arrival_time);
  task_counter_++;

  // Create a new TaskNode to hold the Task
  TaskNode* new_task_node =
      new TaskNode(new_task);  // Correctly initialize TaskNode

  if (task_list_head_ == nullptr) {
    task_list_head_ = new_task_node;  // Set the head to the new TaskNode
  } else {
    TaskNode* current_node = task_list_head_;
    while (current_node->next_ != nullptr) {
      current_node = current_node->next_;  // Traverse to the end
    }
    current_node->next_ = new_task_node;  // Link the new TaskNode
  }

  return new_task->GetID();  // Return the ID of the newly added task
}

void SimScheduler::AssignTasks() {
  TaskNode* current_node = task_list_head_;

  while (current_node) {
    Core* best_core = nullptr;
    int lowest_pending_time =
        INT_MAX;  // Start with a high value for comparison

    // Evaluate all cores to find the best one for this task
    for (int i = 0; i < core_count_; ++i) {
      Core* core = cores_[i];
      if (core) {
        int pending_time =
            core->getPendingTime();  // Get the pending time for this core

        // Check if this core is a better fit
        if (pending_time < lowest_pending_time) {
          lowest_pending_time = pending_time;
          best_core = core;  // Update best core
        } else if (pending_time == lowest_pending_time) {
          // If there's a tie, choose the one with the lower ID
          if (best_core == nullptr || core->GetID() < best_core->GetID()) {
            best_core = core;
          }
        }
      }
    }

    if (best_core) {
      best_core->AssignTask(current_node->task_);  // Use task_ from TaskNode
    }
    std::cout << "Assigning task to core " << best_core->GetID() << std::endl;
    std::cout << "pending time: " << best_core->getPendingTime() << std::endl;

    // Move to the next TaskNode in the list
    current_node = current_node->next_;
  }
}

void SimScheduler::TickTock(int num_ticks) {
  for (int i = 0; i < num_ticks; ++i) {
    TaskNode* current_node = task_list_head_;
    TaskNode* previous_node = nullptr;

    while (current_node) {
      Task* current_task =
          current_node->task_;  // Get the task from the current TaskNode

      if (current_task->IsAssigned()) {
        current_task->DecrementTime();  // Decrement task time

        // Add any additional logic for executing the task
      } else {
        // Increment waiting time if the task is not executed
        current_task->IncrementWaitingTime();
      }

      // Check if the task is complete
      if (current_task->GetTime() <= 0) {
        std::cout << "Removed task " << current_task->GetID()
                  << " which executed after waiting "
                  << current_task->GetWaitingTime() << "." << std::endl;

        // Remove the completed task
        if (previous_node) {
          previous_node->next_ =
              current_node->next_;  // Bypass the current task
        } else {
          task_list_head_ = current_node->next_;  // Remove the head
        }

        // Free memory
        delete current_task;  // Free the task
        delete current_node;  // Free the TaskNode

        current_node = previous_node ? previous_node->next_ : task_list_head_;
        continue;  // Skip to the next iteration
      }

      previous_node = current_node;        // Move to the next task node
      current_node = current_node->next_;  // Move to the next TaskNode
    }
  }
}

Task* SimScheduler::GetTask(int task_id) const {
  TaskNode* current_node = task_list_head_;
  while (current_node) {
    if (current_node->task_->GetID() == task_id) {
      return current_node->task_;  // Return the task if found
    }
    current_node = current_node->next_;
  }
  return nullptr;  // Task not found
}

Task* SimScheduler::GetTaskAtFront() {
  return task_list_head_ ? task_list_head_->task_
                         : nullptr;  // Return the task at the front of the
                                     // list, or nullptr if empty
}

void SimScheduler::RemoveTask(int id) {
  TaskNode* current_node = task_list_head_;
  TaskNode* previous_node = nullptr;

  // Traverse the list to find the task with the given ID
  while (current_node) {
    if (current_node->task_->GetID() == id) {
      // Remove the task from the linked list
      if (previous_node) {
        previous_node->next_ = current_node->next_;  // Bypass the current task
      } else {
        task_list_head_ = current_node->next_;  // Remove the head
      }

      // Get waiting time before deletion for output
      int waiting_time = current_node->task_->GetWaitingTime();
      bool was_executed = current_node->task_->GetExecutedTime() > 0;

      // Free memory
      delete current_node->task_;  // Free the task
      delete current_node;         // Free the TaskNode

      // Output status
      std::cout << "Removed task " << id
                << (was_executed ? " which executed" : " which did not execute")
                << " after waiting " << waiting_time << "." << std::endl;

      return;  // Exit the method after removal
    }

    previous_node = current_node;  // Move to the next node
    current_node = current_node->next_;
  }
}
