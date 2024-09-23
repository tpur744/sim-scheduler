#include "SimScheduler.hpp"

#include <iostream>
#include <string>

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

int SimScheduler::GetNextCoreID() const {
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

int SimScheduler::AddTask(int time, int priority, int arrival_time) {
  Task* newTask = new Task(task_counter_, time, priority, arrival_time);
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

void SimScheduler::AssignTasks() {
  Task* currentTask = task_list_head_;

  while (currentTask) {
    Core* bestCore = nullptr;
    int lowestPendingTime = INT_MAX;  // Start with a high value for comparison

    // Evaluate all cores to find the best one for this task
    for (int i = 0; i < core_count_; ++i) {
      Core* core = cores_[i];
      if (core) {
        int pendingTime =
            core->getPendingTime();  // Get the pending time for this core

        // Check if this core is a better fit
        if (pendingTime < lowestPendingTime) {
          lowestPendingTime = pendingTime;
          bestCore = core;  // Update best core
        } else if (pendingTime == lowestPendingTime) {
          // If there's a tie, choose the one with the lower ID
          if (bestCore == nullptr || core->GetID() < bestCore->GetID()) {
            bestCore = core;
          }
        }
      }
    }

    // If a suitable core was found, assign the task
    if (bestCore) {
      bestCore->AssignTask(currentTask);
    }

    // Move to the next task in the list
    currentTask = currentTask->GetNext();
  }
}

void SimScheduler::TickTock(int numTicks) {
  for (int i = 0; i < numTicks; ++i) {
    Task* currentTask = task_list_head_;
    Task* previousTask = nullptr;

    while (currentTask) {
      if (currentTask->IsAssigned()) {
        currentTask->DecrementTime();  // Decrement task time

        // Add any additional logic for executing the task
      } else {
        // Increment waiting time if the task is not executed
        currentTask->IncrementWaitingTime();
      }

      // Check if the task is complete
      if (currentTask->GetTime() <= 0) {
        std::cout << "Removed task " << currentTask->GetID()
                  << " which executed after waiting "
                  << currentTask->GetWaitingTime() << "." << std::endl;

        // Remove the completed task
        if (previousTask) {
          previousTask->SetNext(currentTask->GetNext());
        } else {
          task_list_head_ = currentTask->GetNext();
        }
        delete currentTask;  // Free memory
        currentTask = previousTask ? previousTask->GetNext() : task_list_head_;
        continue;  // Skip to the next iteration
      }

      previousTask = currentTask;  // Move to the next task
      currentTask = currentTask->GetNext();
    }
  }
}

Task* SimScheduler::GetTask(int task_id) const {
  Task* currentTask = task_list_head_;
  while (currentTask) {
    if (currentTask->GetID() == task_id) {
      return currentTask;  // Return the task if found
    }
    currentTask = currentTask->GetNext();
  }
  return nullptr;  // Task not found
}

Task* SimScheduler::GetTaskAtFront() {
  return task_list_head_;  // Return the task at the front of the list
}

/*bool SimScheduler::IsTaskExecuting(int id) const {
  for (const auto& core : cores_) {  // Assuming cores_ is your list of cores
    Task* frontTask =
        core->GetTaskAtFront();  // Assume each core has this method
    if (frontTask != nullptr && frontTask->GetID() == id) {
      return true;  // The task is currently executing
    }
  }
  return false;  // The task is not executing on any core
}
*/
void SimScheduler::RemoveTask(int id) {
  Task* current = task_list_head_;
  Task* previous = nullptr;

  // Traverse the list to find the task with the given ID
  while (current) {
    if (current->GetID() == id) {
      // Remove the task from the linked list
      if (previous) {
        previous->SetNext(current->GetNext());  // Bypass the current task
      } else {
        task_list_head_ = current->GetNext();  // Remove the head
      }

      // Get waiting time before deletion for output
      int waiting_time = current->GetWaitingTime();
      bool was_executed = current->GetExecutedTime() > 0;

      // Delete the task and free memory
      delete current;

      // Output the removal message
      std::cout << "Removed task " << id << " which "
                << (was_executed ? "was" : "wasn't")
                << " executed after waiting " << waiting_time << " units."
                << std::endl;
      return;  // Exit after removal
    }

    previous = current;
    current = current->GetNext();  // Move to the next task
  }

  // If the task wasn't found
  std::cout << "No task with ID " << id << " found." << std::endl;
}
