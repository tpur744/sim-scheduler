#ifndef TASKNODE_H
#define TASKNODE_H

#include "Task.hpp"  // Include the Task header

class TaskNode {
 public:
  Task* task_;      // Pointer to the Task object
  TaskNode* next_;  // Pointer to the next node

  TaskNode(Task* task)
      : task_(task), next_(nullptr) {}  // Initialize with a Task pointer
};

#endif  // TASKNODE_H
