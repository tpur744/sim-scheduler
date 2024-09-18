#ifndef SIMSCHEDULER_HPP
#define SIMSCHEDULER_HPP

class SimScheduler {
 private:
  bool schedulerAdded_;  // Flag to track if the scheduler is added

 public:
  // Constructor
  SimScheduler();

  // Destructor
  ~SimScheduler();

  // Method to add the scheduler
  void addScheduler();

  // Method to remove the scheduler
  void removeScheduler();

  // Method to check if the scheduler is added
  bool isSchedulerAdded() const;
};

#endif  // SIMSCHEDULER_HPP
