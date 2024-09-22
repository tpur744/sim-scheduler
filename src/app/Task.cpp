#include "Task.hpp"

// Constructor
Task::Task(int id, int time, int priority)
    : id_(id), time_(time), priority_(priority), next_(nullptr) {}

// Getters
int Task::GetID() const { return id_; }
int Task::GetTime() const { return time_; }
int Task::GetPriority() const { return priority_; }
Task* Task::GetNext() const { return next_; }

// Setter for next task pointer
void Task::SetNext(Task* next) { next_ = next; }

bool Task::IsAssigned() const { return assigned_; }

void Task::MarkAsAssigned() { assigned_ = true; }

Task::~Task() {}
