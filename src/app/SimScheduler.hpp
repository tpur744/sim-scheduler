#ifndef SIMSCHEDULER_HPP
#define SIMSCHEDULER_HPP

#include "Core.hpp"
#include "FIFOCore.hpp"
#include "PriorityCore.hpp"
#include "Task.hpp"

class SimScheduler {
 private:
  bool schedulerAdded_;  // Flag to track if the scheduler is added
  Core* cores_[8];       // Fixed-size array for up to 8 cores
  int core_count_;
  Task* task_list_head_;

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
  bool HasCores() const;  // Ensure this is declared as const

  int getNextCoreId() const;

  bool AddCore(Core* core);

  bool RemoveCore(int core_id);

  Core* GetCore(int core_id) const;

  void addTask(int time, int priority);

  void removeTask(int id);

  Task* findTask(int id);

  void clearTasks();
};

#endif  // SIMSCHEDULER_HPP
