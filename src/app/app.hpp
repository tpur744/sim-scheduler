#ifndef APP_H
#define APP_H

#include <string>

#include "SimScheduler.hpp"
/*
*****************************
  This is where you start!
  However, make sure you do not not write all your code in a single file!
*****************************
*/

class App {
 private:
  SimScheduler simScheduler;

 public:
  App(void);
  ~App(void);
  void AddScheduler(void);
  void TickTock(const std::string &num_ticktock);
  void RemoveScheduler(void);
  void AddCore(const std::string &core_type);
  void AddTask(const std::string &consult_time, const std::string &priority);
  void RemoveTask(const std::string &task_id);
  void RemoveCore(const std::string &core_id);
  void ShowTask(const std::string &task_id) const;
  void ShowCore(const std::string &core_id) const;
};

#endif  // APP_H
