#include "Core.hpp"

// Constructor to initialize ID and pending time
Core::Core(int id)
    : id_(id),
      pending_time_(0),
      assigned_task_count_(0),
      completed_task_count_(0) {}

// Get the total pending time
int Core::getPendingTime() const { return pending_time_; }

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
