#include "SimScheduler.hpp"

#include <iostream>

SimScheduler::SimScheduler() : schedulerAdded_(false) {}

SimScheduler::~SimScheduler() {
  removeScheduler();  // Ensure scheduler is removed on destruction
}

void SimScheduler::addScheduler() {
  if (schedulerAdded_) {
    return;
  } else {
    schedulerAdded_ = true;  // Update the flag
  }
}

void SimScheduler::removeScheduler() {
  if (schedulerAdded_) {
    schedulerAdded_ = false;  // Update the flag
    std::cout << "Removed scheduler" << std::endl;
  }
}

bool SimScheduler::isSchedulerAdded() const { return schedulerAdded_; }
