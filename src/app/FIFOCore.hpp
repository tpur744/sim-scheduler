#ifndef FIFOCore_H
#define FIFOCore_H

#include "Core.hpp"
#include "TaskNode.hpp"

class FIFOCore : public Core {
 private:
  TaskNode* tail_;  // Pointer to the tail of the task list

 public:
  // Constructor
  FIFOCore(int id);

  // Override AddTask for FIFO behavior
  void AddTask(Task* task) override;

  void AssignTask(Task* task) override;

  // Destructor to clear the task list
  ~FIFOCore();

  CoreType GetCoreType() const override { return FIFO; }
};

#endif