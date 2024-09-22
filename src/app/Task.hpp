#ifndef TASK_HPP
#define TASK_HPP

class Task {
 public:
  // Constructor
  Task(int time, int priority);

  // Getters
  int GetID() const;
  int GetTime() const;
  int GetPriority() const;

  // Linked list management
  void AddTask(Task*& head, int time, int priority);
  void RemoveTask(Task*& head, int id);
  Task* FindTask(Task* head, int id);
  void ClearTasks(Task*& head);

  // Pointer to the next task in the list
  Task* next;

 private:
  int id_;        // Unique identifier for the task
  int time_;      // Time to execute the task
  int priority_;  // Priority of the task
};

#endif  // TASK_HPP
