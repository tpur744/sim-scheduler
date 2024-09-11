#include "launcher.hpp"

#include <iostream>
#include <vector>

#include "message.hpp"
#include "utils.hpp"

using namespace std;

/*
*****************************
  DO NOT MODIFY THIS FILE
*****************************
*/

string Launcher::COMMAND_PREFIX = "simscheduler> ";

Launcher::Launcher() {}

bool Launcher::ProcessCommand(const std::string &cmd_input) {
  // Check it is a valid command name
  string cmd_input_name = Utils::GetFirstWord(cmd_input);
  Command command = Command::Find(cmd_input_name);
  if (command == Command::INVALID) {
    Message::COMMAND_NOT_FOUND.PrintMessage({cmd_input_name});
    return true;
  }

  // Check the number of arguments provided match as the command expects
  vector<string> args = Utils::SplitString(cmd_input);
  int num_args_provided = args.size() - 1;
  int num_args_expected = command.GetNumArgs();
  if (num_args_provided != num_args_expected) {
    string provided = to_string(num_args_provided) + " argument" +
                      (num_args_provided == 1 ? "" : "s");
    string expected = to_string(num_args_expected) + " argument" +
                      (num_args_expected == 1 ? "" : "s");
    cout << Message::WRONG_ARGUMENT_COUNT.GetMessage({command.GetName()}) +
                " Expected " + expected + ", but " + provided + " provided."
         << endl;
    return true;
  }

  if (command == Command::TICKTOCK) {
    app_.TickTock(args[1]);
  } else if (command == Command::ADD_SCHEDULER) {
    app_.AddScheduler();
  } else if (command == Command::REMOVE_SCHEDULER) {
    app_.RemoveScheduler();
  } else if (command == Command::ADD_CORE) {
    app_.AddCore(args[1]);
  } else if (command == Command::REMOVE_CORE) {
    app_.RemoveCore(args[1]);
  } else if (command == Command::ADD_TASK) {
    app_.AddTask(args[1], args[2]);
  } else if (command == Command::SHOW_TASK) {
    app_.ShowTask(args[1]);
  } else if (command == Command::SHOW_CORE) {
    app_.ShowCore(args[1]);
  } else if (command == Command::REMOVE_TASK) {
    app_.RemoveTask(args[1]);
  } else if (command == Command::HELP) {
    cout << Command::GetHelp() << endl;
  } else if (command == Command::EXIT) {
    Message::END.PrintMessage();
    return false;
  } else {
    Message::COMMAND_NOT_FOUND.PrintMessage({cmd_input});
  }
  return true;
}

void Launcher::Start() {
  PrintBanner();
  cout << Command::GetHelp() << endl;

  // Keep reading commands until the user exits
  string cmd_input;
  do {
    cout << COMMAND_PREFIX;
    std::getline(std::cin, cmd_input);
    cmd_input = Utils::TrimString(cmd_input);
  } while (ProcessCommand(cmd_input));
}

void Launcher::PrintBanner() const {
  std::string banner = "Welcome to SimScheduler!";
  cout << endl << banner << endl << endl;
}

std::string Launcher::GetCommandPrefix() const { return COMMAND_PREFIX; }
