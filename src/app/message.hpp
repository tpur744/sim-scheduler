#ifndef MESSAGE_H
#define MESSAGE_H

#include <initializer_list>
#include <string>

/*
*****************************
  DO NOT MODIFY THIS FILE
*****************************
*/

class Message {
 private:
  std::string msg_;

 public:
  static const Message ERROR_SECOND_SCHEDULER;
  static const Message ERROR_NO_SCHEDULERS;
  static const Message ERROR_NO_CORES;
  static const Message ERROR_CORES;
  static const Message ERROR_NO_CORE;
  static const Message ERROR_NO_TASK;
  static const Message ERROR_CORE_NOT_FREE;
  static const Message SHOW_TASK;
  static const Message SHOW_CORE;
  static const Message SCHEDULER_ADDED;
  static const Message SCHEDULER_REMOVED;
  static const Message ERROR_MAX_CORES;
  static const Message ERROR_CORE_TYPE;
  static const Message CORE_ADDED;
  static const Message CORE_REMOVED;
  static const Message TASK_ADDED;
  static const Message TASK_REMOVED;
  static const Message TASK_NOT_REMOVED;
  static const Message SIMSCHEDULER_CLOCK;

  static const Message COMMAND_NOT_FOUND;
  static const Message WRONG_ARGUMENT_COUNT;
  static const Message END;

  Message(const std::string &msg);
  std::string GetMessage() const;
  std::string GetMessage(std::initializer_list<std::string> args) const;
  void PrintMessage() const;
  void PrintMessage(std::initializer_list<std::string> args) const;
};

#endif  // MESSAGE_H
