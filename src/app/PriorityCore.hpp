#ifndef PRIORITYCORE_H
#define PRIORITYCORE_H

#include "Core.hpp"
#include "TaskNode.hpp"

class PriorityCore : public Core {
 private:
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