#ifndef PRIORITYCORE_H
#define PRIORITYCORE_H

#include "Core.hpp"
#include "TaskNode.hpp"

class PriorityCore : public Core {
 private:
 public:
  // Constructor
  PriorityCore(int id);

  // Override AddTask for priority behavior (single parameter)
  void AddTask(Task* task) override;

  void AssignTask(Task* task) override;

  CoreType GetCoreType() const override { return PRIORITY; }

  // Destructor to clear the task list
  ~PriorityCore();
};

#endif
