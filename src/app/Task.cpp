#include "Task.hpp"

// Constructor
// Constructor
Task::Task(int id, int time, int priority, int arrival_time)
    : id_(id),
      time_(time),
      priority_(priority),
      next_(nullptr),
      waiting_time_(0),
      executed_time_(0),
      arrival_time_(arrival_time),
      assigned_(false) {}  // Initialize assigned_

// Getters
int Task::GetID() const { return id_; }
int Task::GetTime() const { return time_; }
int Task::GetPriority() const { return priority_; }
Task* Task::GetNext() const { return next_; }

// Setter for next task pointer
void Task::SetNext(Task* next) { next_ = next; }

bool Task::IsAssigned() const { return assigned_; }

void Task::MarkAsAssigned() { assigned_ = true; }

void Task::DecrementTime() {
  if (time_ > 0) {
    time_--;
    executed_time_++;
  }
}

int Task::GetWaitingTime() const { return waiting_time_; }
void Task::IncrementWaitingTime() { waiting_time_++; }
int Task::GetExecutedTime() const { return executed_time_; }
int Task::GetArrivalTime() const { return arrival_time_; }
Task::~Task() {}
