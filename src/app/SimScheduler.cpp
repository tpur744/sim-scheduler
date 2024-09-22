#include "SimScheduler.hpp"

#include <iostream>

#include "FIFOCore.hpp"
#include "PriorityCore.hpp"

// Constructor
SimScheduler::SimScheduler() : schedulerAdded_(false), core_count_(0) {
  for (int i = 0; i < 8; ++i) {
    cores_[i] = nullptr;  // Initialize pointers to null
  }
}

// Destructor
SimScheduler::~SimScheduler() {
  removeScheduler();  // Ensure scheduler is removed on destruction
  for (int i = 0; i < core_count_; i++) {
    delete cores_[i];  // Clean up allocated cores
  }
}

// Method to add the scheduler
void SimScheduler::addScheduler() {
  if (!schedulerAdded_) {
    schedulerAdded_ = true;  // Update the flag
  }
}

bool SimScheduler::hasCores() const {
  for (int i = 0; i < core_count_; ++i) {
    if (cores_[i] != nullptr) {
      return true;  // Found a core that still exists
    }
  }
  return false;  // No active cores
}

// Method to remove the scheduler
bool SimScheduler::removeScheduler() {
  if (schedulerAdded_) {
    if (hasCores()) {
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
bool SimScheduler::isSchedulerAdded() const { return schedulerAdded_; }

// Method to add a core
bool SimScheduler::addCore(Core* core) {
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

Core* SimScheduler::getCore(int id) const {
  return cores_[id];  // Return the core at the given index
}

bool SimScheduler::removeCore(int core_id) {
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
