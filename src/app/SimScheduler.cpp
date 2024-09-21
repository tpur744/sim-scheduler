#include "SimScheduler.hpp"

#include <iostream>

#include "FIFOCore.hpp"
#include "PriorityCore.hpp"

// Constructor
SimScheduler::SimScheduler() : schedulerAdded_(false), coreCount_(0) {
  for (int i = 0; i < 8; ++i) {
    cores_[i] = nullptr;  // Initialize pointers to null
  }
}

// Destructor
SimScheduler::~SimScheduler() {
  removeScheduler();  // Ensure scheduler is removed on destruction
  for (int i = 0; i < coreCount_; i++) {
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
  return coreCount_ > 0;  // Check if any cores are present
}

// Method to remove the scheduler
bool SimScheduler::removeScheduler() {
  if (schedulerAdded_) {
    if (hasCores()) {
      return false;  // Cannot remove scheduler while cores are present
    }

    // Free memory for each core
    for (int i = 0; i < coreCount_; ++i) {
      delete cores_[i];
      cores_[i] = nullptr;  // Set to nullptr after deletion
    }
    coreCount_ = 0;           // Reset core count
    schedulerAdded_ = false;  // Mark the scheduler as removed
    return true;              // Indicate success
  }
  return false;  // Indicate no scheduler was present
}

// Method to check if the scheduler is added
bool SimScheduler::isSchedulerAdded() const { return schedulerAdded_; }

// Method to add a core
bool SimScheduler::addCore(Core* core) {
  if (coreCount_ < 8) {  // Check if there's space for more cores
    cores_[coreCount_] = core;
    coreCount_++;
    return true;  // Indicate successful addition
  }
  return false;  // Indicate failure to add core
}

int SimScheduler::getNextCoreId() const {
  return coreCount_;  // Returns the next available ID based on the current
                      // count
}