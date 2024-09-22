#include "SimScheduler.hpp"

#include <iostream>

#include "FIFOCore.hpp"
#include "PriorityCore.hpp"
#include "Task.hpp"

// Constructor
SimScheduler::SimScheduler()
    : schedulerAdded_(false),
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
  if (!schedulerAdded_) {
    schedulerAdded_ = true;  // Update the flag
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
  if (schedulerAdded_) {
    if (HasCores()) {
      return false;  // Cannot remove scheduler while cores are present
    }

    // Free memory for each core
    for (int i = 0; i < core_count_; ++i) {
      delete cores_[i];
      cores_[i] = nullptr;  // Set to nullptr after deletion
    }
    core_count_ = 0;          // Reset core count
    schedulerAdded_ = false;  // Mark the scheduler as removed
    return true;              // Indicate success
  }
  return false;  // Indicate no scheduler was present
}

// Method to check if the scheduler is added
bool SimScheduler::IsSchedulerAdded() const { return schedulerAdded_; }

// Method to add a core
bool SimScheduler::AddCore(Core* core) {
  if (core_count_ < 8) {  // Check if there's space for more cores
    cores_[core_count_] = core;
    core_count_++;
    return true;  // Indicate successful addition
  }
  return false;  // Indicate failure to add core
}

int SimScheduler::getNextCoreId() const {
  return core_count_;  // Returns the next available ID based on the current
                       // count
}

Core* SimScheduler::GetCore(int id) const {
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

int SimScheduler::AddTask(int time, int priority) {
  Task* newTask = new Task(task_counter_, time, priority);
  task_counter_++;

  if (task_list_head_ == nullptr) {
    task_list_head_ = newTask;
  } else {
    Task* current = task_list_head_;
    while (current->GetNext() != nullptr) {
      current = current->GetNext();
    }
    current->SetNext(newTask);
  }

  return newTask->GetID();  // Return the ID of the newly added task
}