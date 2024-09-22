#ifndef FIFOCore_H
#define FIFOCore_H

#include "Core.hpp"

class FIFOCore : public Core {
 private:
  // Define a linked list node within the class
  class TaskNode {
   public:
    int task_time_;
    int priority_;
    TaskNode* next_;

    TaskNode(int time, int priority)
        : task_time_(time), priority_(priority), next_(nullptr) {}
  };

  TaskNode* head_;  // Pointer to the head of the task list
  TaskNode* tail_;  // Pointer to the tail of the task list

 public:
  // Constructor
  FIFOCore(int id);

  // Override AddTask for FIFO behavior
  void AddTask(int task_time, int priority) override;

  void AssignTask(Task* task) override;

  // Destructor to clear the task list
  ~FIFOCore();

  CoreType GetCoreType() const override { return FIFO; }
};

#endif
