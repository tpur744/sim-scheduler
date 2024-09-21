#ifndef CORE_H
#define CORE_H

class Core {
 protected:
  int id;
  int pending_time;

 public:
  // Constructor to initialize the Core with an ID
  Core(int id);

  // Pure virtual function for adding a task
  virtual void addTask(int task_time, int priority) = 0;

  // Get the total pending execution time
  int getPendingTime() const;

  // Virtual destructor for safe deletion
  virtual ~Core();
};

#endif
