#ifndef PRIORITYCORE_H
#define PRIORITYCORE_H

#include "Core.hpp"

class PriorityCore : public Core {
 private:
  // Define a linked list node for the task
  class TaskNode {
   public:
    int task_time_;
    int priority_;
    TaskNode* next_;

    TaskNode(int time, int priority)
        : task_time_(time), priority_(priority), next_(nullptr) {}
  };

  TaskNode* head_;  // Pointer to the head of the task list

 public:
  // Constructor
  PriorityCore(int id);

  // Override addTask for priority behavior
  void addTask(int task_time, int priority) override;

  // Destructor to clear the task list
  ~PriorityCore();
};

#endif
