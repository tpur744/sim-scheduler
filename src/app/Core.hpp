#ifndef CORE_H
#define CORE_H
#include "Task.hpp"
#include "TaskNode.hpp"
enum CoreType { FIFO, PRIORITY };
class Core {
 protected:
  int id_;
  int pending_time_;
  int assigned_task_count_;
  int completed_task_count_;
  TaskNode* head_;  // Pointer to the head of the task list

 public:
  // Constructor to initialize the Core with an ID
  Core(int id);

  virtual CoreType GetCoreType() const = 0;

  // Pure virtual function for adding a task
  virtual void AddTask(Task* task) = 0;

  virtual void AssignTask(Task* task) = 0;

  // Get the total pending execution time
  int GetPendingTime() const;

  // Virtual destructor for safe deletion
  virtual ~Core();
  int GetAssignedTaskCount() const;
  int GetCompletedTaskCount() const;
  void CompleteTask();

  int GetID() const;

  Task* GetCurrentTask() const;

  void IncrementCompletedTaskCount();

  void TickForward();

  void RemoveTask(int id, bool print_output = true, bool allow_removal = false);

  Task* GetTask(int id);
};

#endif