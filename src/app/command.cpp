
#include "command.hpp"

#include <iostream>

#include "message.hpp"
#include "utils.hpp"
using namespace std;

/*
*****************************
  DO NOT MODIFY THIS FILE
*****************************
*/

const Command Command::TICKTOCK("TICKTOCK", 1,
                                "Increment the SimScheduler clock by <TICKS>.");
const Command Command::ADD_SCHEDULER("ADD_SCHEDULER", 0,
                                     "Add scheduler to SimScheduler.");
const Command Command::REMOVE_SCHEDULER("REMOVE_SCHEDULER", 0,
                                        "Remove scheduler from SimScheduler.");
const Command Command::ADD_CORE("ADD_CORE", 1,
                                "Add core of <TYPE> to SimScheduler.");
const Command Command::ADD_TASK(
    "ADD_TASK", 2, "Add task with <TASK_TIME> and <PRIORITY> to SimScheduler.");
const Command Command::SHOW_TASK("SHOW_TASK", 1,
                                 "Show info about task with <TASK_ID>.");
const Command Command::SHOW_CORE("SHOW_CORE", 1,
                                 "Show info about core with <CORE_ID>.");
const Command Command::REMOVE_TASK("REMOVE_TASK", 1,
                                   "Remove task <TASK_ID> from SimScheduler.");
const Command Command::REMOVE_CORE("REMOVE_CORE", 1,
                                   "Remove core <CORE_ID> from SimScheduler.");
const Command Command::HELP("HELP", 0, "Print usage");
const Command Command::EXIT("EXIT", 0, "Exit the application");
const Command Command::INVALID("_____INVALID____", 0,
                               "Placeholder for INVALID Command value");

std::initializer_list<Command> Command::commands = {
    Command::TICKTOCK,    Command::ADD_SCHEDULER, Command::REMOVE_SCHEDULER,
    Command::ADD_CORE,    Command::REMOVE_CORE,   Command::ADD_TASK,
    Command::REMOVE_TASK, Command::SHOW_TASK,     Command::SHOW_CORE,
    Command::HELP,        Command::EXIT};

Command::Command(const std::string &name, int num_args,
                 const std::string &message) {
  this->name_ = name;
  this->num_args_ = num_args;
  this->message_ = message;
}

Command::Command(const std::string &name, int num_args,
                 const std::string &message,
                 const std::vector<std::string> &option_prompts) {
  this->name_ = name;
  this->num_args_ = num_args;
  this->message_ = message;
  this->option_prompts_ = option_prompts;
}

std::string Command::GetName() const { return name_; }

bool Command::HasArguments() const { return num_args_ > 0; }

int Command::GetNumArgs() const { return num_args_; }

bool Command::HasOptions() const { return option_prompts_.size() > 0; }

int Command::GetNumOptions() const { return option_prompts_.size(); }

std::string Command::GetMessage() const { return message_; }

std::string Command::GetOptionPrompt(int index) const {
  return option_prompts_[index];
}

bool Command::operator==(const Command &other) const {
  return name_ == other.name_;
}

bool Command::operator!=(const Command &other) const {
  return !(*this == other);
}

std::string Command::GetHelp() {
  std::string help;
  std::string white;
  for (const Command &command : commands) {
    int num_args = command.GetNumArgs();
    int len_name = command.GetName().length();
    white.clear();
    for (int indent = 0; indent < 20 - len_name; indent++) white += " ";
    help += command.GetName() + white;
    help += "[" + to_string(command.GetNumArgs()) + " argument(s)]";
    help += "    ";
    help += command.GetMessage() + "\n";
  }
  return help;
}

bool Command::IsValidCommand(const std::string &cmd_input) {
  return Find(cmd_input) != Command::INVALID;
}

const Command &Command::Find(const std::string &cmd_input) {
  string cmd = Utils::GetFirstWord(cmd_input);
  cmd = Utils::GetUppercaseUnderscore(cmd);

  for (const Command &command : commands) {
    if (command.GetName() == cmd) {
      return command;
    }
  }

  return Command::INVALID;
}
