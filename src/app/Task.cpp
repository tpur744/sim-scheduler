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

// Destructor (if you plan to dynamically allocate memory, otherwise can be
// omitted)
Task::~Task() {
  // Clean up if necessary (though not much to do in this case)
}
