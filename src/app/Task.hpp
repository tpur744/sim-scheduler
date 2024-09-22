#ifndef TASK_HPP
#define TASK_HPP

class Task {
 public:
  // Constructor
  Task(int id, int time, int priority);

  // Getters
  int getId() const;
  int getTime() const;
  int getPriority() const;

 private:
  int id_;        // Unique identifier for the task
  int time_;      // Time to execute the task
  int priority_;  // Priority of the task
};

#endif  // TASK_HPP