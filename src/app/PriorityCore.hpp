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

  // Override AddTask for priority behavior
  void AddTask(int task_time, int priority) override;

  void AssignTask(Task* task) override;

  CoreType GetCoreType() const override { return PRIORITY; }

  // Destructor to clear the task list
  ~PriorityCore();
};

#endif