#ifndef TASKNODE_H
#define TASKNODE_H

class TaskNode {
 public:
  int task_time_;
  int priority_;
  TaskNode* next_;

  TaskNode(int time, int priority)
      : task_time_(time), priority_(priority), next_(nullptr) {}
};

#endif  // TASKNODE_H
