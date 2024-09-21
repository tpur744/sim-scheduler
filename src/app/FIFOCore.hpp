#ifndef FIFOCore_H
#define FIFOCore_H

#include "Core.hpp"

class FIFOCore : public Core {
 private:
  // Define a linked list node within the class
  class TaskNode {
   public:
    int task_time;
    int priority;
    TaskNode* next;

    TaskNode(int time, int priority)
        : task_time(time), priority(priority), next(nullptr) {}
  };

  TaskNode* head;  // Pointer to the head of the task list
  TaskNode* tail;  // Pointer to the tail of the task list

 public:
  // Constructor
  FIFOCore(int id);

  // Override addTask for FIFO behavior
  void addTask(int task_time, int priority) override;

  // Destructor to clear the task list
  ~FIFOCore();
};

#endif
