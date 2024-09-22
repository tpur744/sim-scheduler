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
  Task* taskListHead_;

 public:
  // Constructor
  SimScheduler();

  // Destructor
  ~SimScheduler();

  // Method to add the scheduler
  void addScheduler();

  // Method to remove the scheduler
  bool removeScheduler();

  // Method to check if the scheduler is added
  bool isSchedulerAdded() const;

  // Method to check if any cores are present
  bool hasCores() const;  // Ensure this is declared as const

  int getNextCoreId() const;

  bool addCore(Core* core);

  bool removeCore(int core_id);

  Core* getCore(int core_id) const;

  bool addTask(int taskTime, int priority);

  void assignTaskToCore(Task* task);
};

#endif  // SIMSCHEDULER_HPP
