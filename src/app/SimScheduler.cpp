#include "SimScheduler.hpp"

#include <climits>
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
  for (int i = 0; i < 8; ++i) {  // Iterate over all core slots (fixed size 8)
    if (cores_[i] != nullptr) {
      return true;  // Found at least one core
    }
  }
  return false;  // No active cores found
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
  int next_core_id =
      GetNextCoreID();  // Get the next available ID (recycled or new)
  if (next_core_id != -1) {
    cores_[next_core_id] = core;  // Assign the core to the recycled ID
    core_count_++;
    return true;  // Indicate successful addition
  }
  return false;  // No space left to add the core
}

int SimScheduler::GetNextCoreID() const {
  for (int i = 0; i < 8; i++) {
    if (cores_[i] == nullptr) {
      return i;  // Return the first available core ID
    }
  }
  return -1;  // No available core ID
}

Core* SimScheduler::GetCore(int id) const {
  return cores_[id];  // Return the core at the given index
}

bool SimScheduler::RemoveCore(int core_id) {
  if (core_id < 0 || core_id >= 8) {
    std::cout << "No core with ID " << core_id << "." << std::endl;
    return false;  // Invalid core ID
  }

  if (cores_[core_id] == nullptr) {
    std::cout << "No core with ID " << core_id << "." << std::endl;
    return false;  // Core was already removed
  }
  // Check if the core is empty before removal
  if (cores_[core_id]->IsEmpty()) {
    delete cores_[core_id];     // Free the memory for the core
    cores_[core_id] = nullptr;  // Mark the core as removed
    core_count_--;
    return true;  // Indicate success

  } else {
    std::cout << "Core " << core_id << " is currently executing a task."
              << std::endl;
    return false;  // Core has tasks, so it cannot be removed
  }

  return false;  // Core was already removed or was null
}

void SimScheduler::TickTock(int num_ticks) {
  // go through the number of ticks
  // loop through the cores
  // run the core tick forward function
  for (int i = 0; i < num_ticks; i++) {
    for (int j = 0; j < core_count_; j++) {
      if (cores_[j] != nullptr) {
        cores_[j]->TickForward();
      }
      //   cores_[j]->TickForward();
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

void SimScheduler::RemoveTask(int id) {
  bool task_removed = false;  // Flag to track if any task was removed

  for (int i = 0; i < core_count_; i++) {
    if (cores_[i]->HasTask(id)) {  // Check if the task exists in the core
      cores_[i]->RemoveTask(id);   // Attempt to remove the task
      task_removed = true;         // Update flag if removal was successful
    }
  }

  // Output a message only if the task was not found in any core
  if (!task_removed) {
    std::cout << "No task with ID " << id << "." << std::endl;
  }
}

void SimScheduler::AddTaskToCore(int task_time, int priority,
                                 int arrival_time) {
  // Find the core with the lowest execution time
  Core* best_core = nullptr;
  int lowest_pending_time = INT_MAX;

  for (int i = 0; i < core_count_; ++i) {
    Core* core = cores_[i];
    if (core) {
      int pending_time = core->GetPendingTime();
      if (pending_time < lowest_pending_time) {
        lowest_pending_time = pending_time;
        best_core = core;
      } else if (pending_time == lowest_pending_time) {
        if (best_core == nullptr || core->GetID() < best_core->GetID()) {
          best_core = core;
        }
      }
    }
  }
  best_core->AddTask(
      new Task(task_counter_, task_time, priority, arrival_time));
  task_counter_++;
}

void SimScheduler::ShowTask(int task_id) const {
  // Go through each of the cores and find the task with the given ID
  for (int i = 0; i < core_count_; i++) {
    Task* task = cores_[i]->GetTask(task_id);
    if (task) {
      std::cout << "Task " << task->GetID() << ", time arrival "
                << task->GetArrivalTime() << ", task time " << task->GetTime()
                << ", pending execution time " << task->GetPendingTime()
                << ", priority " << task->GetPriority() << "." << std::endl;
      return;
    }
  }

  std::cout << "No task with ID " << task_id << "." << std::endl;
}

bool SimScheduler::HasAvailableCoreSlot() const {
  for (int i = 0; i < 8; i++) {
    if (cores_[i] == nullptr) {
      return true;  // Found an available slot
    }
  }
  return false;  // No available slots
}
