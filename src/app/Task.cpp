#include "Task.hpp"

// Constructor
Task::Task(int id, int time, int priority)
    : id_(id), time_(time), priority_(priority) {}

// Getters
int Task::getId() const { return id_; }

int Task::getTime() const { return time_; }

int Task::getPriority() const { return priority_; }