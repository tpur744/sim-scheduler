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

// Method to remove the scheduler
void SimScheduler::removeScheduler() {
  if (schedulerAdded_) {
    schedulerAdded_ = false;  // Update the flag
                              // No output here
  }
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