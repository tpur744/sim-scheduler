
#include "message.hpp"

#include <iostream>

using namespace std;

/*
*****************************
  DO NOT MODIFY THIS FILE
*****************************
*/

const Message Message::ERROR_SECOND_SCHEDULER("Cannot add another scheduler.");
const Message Message::ERROR_NO_SCHEDULERS(
    "Cannot perform that operation without a scheduler.");
const Message Message::ERROR_NO_CORES(
    "Cannot perform that operation without a core.");
const Message Message::ERROR_CORES(
    "Cannot perform that operation without first removing core(s).");
const Message Message::ERROR_NO_TASK("No task with ID %s.");
const Message Message::ERROR_NO_CORE("No core with ID %s.");
const Message Message::ERROR_CORE_NOT_FREE(
    "Core %s is currently executing a task.");
const Message Message::SHOW_TASK(
    "Task %s, time arrival %s, task time %s, pending execution time %s, "
    "priority %s.");
const Message Message::SHOW_CORE(
    "Core %s is currently assigned %s task(s) and has completed %s task(s).");
const Message Message::SCHEDULER_ADDED("Added scheduler.");
const Message Message::SCHEDULER_REMOVED("Removed scheduler.");
const Message Message::ERROR_MAX_CORES("Cannot add another core.");
const Message Message::ERROR_CORE_TYPE("Specified core type is unknown.");
const Message Message::CORE_ADDED("Added core of type '%s' with ID %s.");
const Message Message::CORE_REMOVED("Removed core %s.");
const Message Message::TASK_ADDED(
    "Added task with ID %s, task time of %s, and priority of %s.");
const Message Message::TASK_REMOVED(
    "Removed task %s which %s executed after waiting %s.");
const Message Message::TASK_NOT_REMOVED("Task %s is currently being executed.");
const Message Message::SIMSCHEDULER_CLOCK("SimScheduler clock is now %s.");

const Message Message::COMMAND_NOT_FOUND(
    "Command '%s' not found. Run 'help' for the list of available commands.");
const Message Message::WRONG_ARGUMENT_COUNT(
    "Incorrect number of arguments for %s.");
const Message Message::END("Program ended.");

Message::Message(const std::string &msg) { this->msg_ = msg; }

std::string Message::GetMessage() const { return GetMessage({}); }

std::string Message::GetMessage(std::initializer_list<std::string> args) const {
  std::string result = msg_;
  for (const string &arg : args) {
    size_t pos = result.find("%s");
    if (pos != std::string::npos) {
      result.replace(pos, 2, arg);
    }
  }
  return result;
}

void Message::PrintMessage() const { PrintMessage({}); }

void Message::PrintMessage(std::initializer_list<std::string> args) const {
  cout << GetMessage(args) << endl;
}
