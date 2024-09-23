#ifndef CORE_H
#define CORE_H
#include "Task.hpp"

enum CoreType { FIFO, PRIORITY };
class Core {
 protected:
  int id_;
  int pending_time_;
  int assigned_task_count_ = 0;  // Track assigned tasks
  int completed_task_count_ = 0;

 public:
  // Constructor to initialize the Core with an ID
  Core(int id);

  virtual CoreType GetCoreType() const = 0;

  // Pure virtual function for adding a task
  virtual void AddTask(int task_time, int priority) = 0;

  virtual void AssignTask(Task* task) = 0;

  // Get the total pending execution time
  int getPendingTime() const;

  // Virtual destructor for safe deletion
  virtual ~Core();
  int GetAssignedTaskCount() const;
  int GetCompletedTaskCount() const;
  void CompleteTask();

  int GetID() const;

  Task* GetCurrentTask() const;

  void IncrementCompletedTaskCount();
};

#endif