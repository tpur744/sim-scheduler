#include "FIFOCore.hpp"

// Constructor
FIFOCore::FIFOCore(int id) : Core(id), head(nullptr), tail(nullptr) {}

// Add a task to the end of the linked list (FIFO behavior)
void FIFOCore::addTask(int task_time, int priority) {
  TaskNode* newTask = new TaskNode(task_time, priority);

  if (!head) {
    // If the list is empty, the new task is both head and tail
    head = newTask;
    tail = newTask;
  } else {
    // Add the new task to the end of the list
    tail->next = newTask;
    tail = newTask;
  }

  // Increase the pending time
  pending_time += task_time;
}

// Destructor to free memory
FIFOCore::~FIFOCore() {
  TaskNode* current = head;
  while (current) {
    TaskNode* next = current->next;
    delete current;
    current = next;
  }
}
