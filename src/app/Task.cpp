#include "Task.hpp"

// Constructor
Task::Task(int time, int priority)
    : id_(0), time_(time), priority_(priority), next(nullptr) {}

// Getters
int Task::GetID() const { return id_; }

int Task::GetTime() const { return time_; }

int Task::GetPriority() const { return priority_; }

// Add a task to the list
void Task::AddTask(Task*& head, int time, int priority) {
  Task* newTask = new Task(time, priority);
  newTask->id_ = (head == nullptr) ? 0 : head->GetID() + 1;  // Assign unique ID

  if (!head) {
    head = newTask;  // First task
  } else {
    Task* current = head;
    while (current->next) {
      current = current->next;  // Move to the end
    }
    current->next = newTask;  // Link the new task
  }
}

// Remove a task by ID
void Task::RemoveTask(Task*& head, int id) {
  Task* current = head;
  Task* previous = nullptr;

  while (current) {
    if (current->GetID() == id) {
      if (previous) {
        previous->next = current->next;  // Bypass the current task
      } else {
        head = current->next;  // Remove the head
      }
      delete current;  // Free memory
      return;
    }
    previous = current;
    current = current->next;
  }
}

// Find a task by ID
Task* Task::FindTask(Task* head, int id) {
  Task* current = head;
  while (current) {
    if (current->GetID() == id) {
      return current;  // Task found
    }
    current = current->next;
  }
  return nullptr;  // Task not found
}

// Clear all tasks
void Task::ClearTasks(Task*& head) {
  Task* current = head;
  while (current) {
    Task* toDelete = current;
    current = current->next;
    delete toDelete;
  }
  head = nullptr;  // Reset head
}
