#ifndef SIMSCHEDULER_HPP
#define SIMSCHEDULER_HPP

#include "Core.hpp"
#include "FIFOCore.hpp"
#include "PriorityCore.hpp"
#include "Task.hpp"

class SimScheduler {
 private:
  bool scheduler_added_;  // Flag to track if the scheduler is added
  Core* cores_[8];        // Fixed-size array for up to 8 cores
  int core_count_;
  TaskNode* task_list_head_;
  int task_counter_;

 public:
  // Constructor
  SimScheduler();

  // Destructor
  ~SimScheduler();

  // Method to add the scheduler
  void AddScheduler();

  // Method to remove the scheduler
  bool RemoveScheduler();

  // Method to check if the scheduler is added
  bool IsSchedulerAdded() const;

  // Method to check if any cores are present
  bool HasCores() const;

  int GetNextCoreID() const;

  bool AddCore(Core* core);

  bool RemoveCore(int core_id);

  Core* GetCore(int core_id) const;

  void ClearTasks();

  void TickTock(int num_ticks);

  Task* GetTask(int task_id) const;

  void RemoveTask(int task_id);

  bool IsTaskExecuting(int id) const;
  void AddTaskToCore(int task_time, int priority, int arrival_time);

  void ShowTask(int task_id) const;
};

#endif  // SIMSCHEDULER_HPP
