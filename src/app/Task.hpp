#ifndef TASK_HPP
#define TASK_HPP

class Task {
 private:
  int id_;        // Unique identifier for the task
  int time_;      // Time required to complete the task
  int priority_;  // Priority of the task
  Task* next_;    // Pointer to the next task in the linked list

 public:
  // Constructor
  Task(int id, int time, int priority);

  // Getters and Setters
  int GetID() const;
  int GetTime() const;
  int GetPriority() const;
  bool IsAssigned() const;
  void MarkAsAssigned();

  Task* GetNext() const;     // Get next task in the list
  void SetNext(Task* next);  // Set the next task
  bool assigned_ = false;

  // Destructor
  ~Task();
};

#endif  // TASK_HPP
