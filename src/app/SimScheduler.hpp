#ifndef SIMSCHEDULER_HPP
#define SIMSCHEDULER_HPP

#include "Core.hpp"
#include "FIFOCore.hpp"
#include "PriorityCore.hpp"

class SimScheduler {
 private:
  bool schedulerAdded_;  // Flag to track if the scheduler is added
  Core* cores_[8];       // Fixed-size array for up to 8 cores
  int coreCount_;

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
};

#endif  // SIMSCHEDULER_HPP
