#include "SimScheduler.hpp"

#include <iostream>

SimScheduler::SimScheduler() : schedulerAdded_(false) {}

SimScheduler::~SimScheduler() {
  removeScheduler();  // Ensure scheduler is removed on destruction
}

void SimScheduler::addScheduler() {
  if (schedulerAdded_) {
    std::cout << "Cannot add another scheduler" << std::endl;
  } else {
    schedulerAdded_ = true;  // Update the flag
    std::cout << "Added scheduler" << std::endl;
  }
}

void SimScheduler::removeScheduler() {
  if (schedulerAdded_) {
    schedulerAdded_ = false;  // Update the flag
    std::cout << "Removed scheduler" << std::endl;
  }
}

bool SimScheduler::isSchedulerAdded() const { return schedulerAdded_; }
