#include <algorithm>
#include <cctype>
#include <cstring>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "app/launcher.hpp"
#include "app/utils.hpp"

using namespace std;

enum TestResult { TEST_RESULT_PASS, TEST_RESULT_FAIL };

#include <sstream>
#include <string>
#include <vector>

#define RUN_COMMAND(command_with_args)                           \
  cout << app.GetCommandPrefix() << command_with_args << "\n\r"; \
  app.ProcessCommand(command_with_args);

#define RUN_COMMAND_WITH_OPTIONS(command_with_args, options_str)              \
  {                                                                           \
    std::vector<std::string> _options = Utils::SplitString(options_str, ";"); \
    cout << app.GetCommandPrefix() << command_with_args << "\n\r";            \
    app.ProcessCommand(command_with_args, _options);                          \
  }

#define __FILENAME__ \
  (std::strrchr(__FILE__, '/') ? std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define EXPECT_CONTAINS(expected)                                              \
  if (still_capturing_cout) {                                                  \
    still_capturing_cout = false;                                              \
    output = replaceNewlineWithCRLF(buffer.str());                             \
    std::cout.rdbuf(old);                                                      \
    std::cout << output << "\n\r";                                             \
  }                                                                            \
  if (output.find(expected) == std::string::npos) {                            \
    cerr << "FAILED Test " << test_number << " in " << __FILENAME__            \
         << " on line " << __LINE__ << ":\n\r" << "  Expected '" << expected   \
         << "' in the output, but it was not found." << "\n\r" << "\n\r"       \
         << "----------------------------------------------------------------" \
            "------------"                                                     \
         << "\n\r" << "\n\r";                                                  \
    return TEST_RESULT_FAIL;                                                   \
  }

#define EXPECT_DOES_NOT_CONTAINS(expected, ignore_case)                      \
  output = replaceNewlineWithCRLF(buffer.str());                             \
  if (Utils::Contains(output, expected, ignore_case)) {                      \
    cerr << "FAILED Test " << test_number << " in " << __FILENAME__          \
         << " on line " << __LINE__ << ":\n\r" << "\tExpected '" << expected \
         << "' to NOT be in the output, but it was." << "\n\r" << "\n\r";    \
    return TEST_RESULT_FAIL;                                                 \
  }

std::string replaceNewlineWithCRLF(const std::string &input) {
  std::string replacedStr;
  for (char ch : input) {
    if (ch == '\n') {
      replacedStr += "\n\r";
    } else {
      replacedStr += ch;
    }
  }
  return replacedStr;
}

int main(int argc, char *argv[]) {
  Launcher app;
  string output = "";
  bool still_capturing_cout = true;
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

  int test_number = std::stoi(argv[1]);
  std::cout << "Running test " << test_number << "\n\r";
  switch (test_number) {
    case 1:  // Add Remove Scheduler
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 2:  // Remove Scheduler Does Not Exist
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      EXPECT_DOES_NOT_CONTAINS("Removed scheduler.", false);
      break;

    case 3:  // Add Twice Scheduler
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Cannot add another scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 4:  // Add Remove Twice Scheduler
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 5:  // Add Once Remove Twice Scheduler
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      break;

    case 6:  // Add Single FIFO Core Capital
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core FIFO");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 7:  // Add Single FIFO Core Lowercase
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 8:  // Add Single FIFO Core Mixed Case
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core FiFo");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 9:  // Add Two FIFO Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 10:  // Add Single FIFO Core Remove Twice
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 11:  // Add Single FIFO Core No Removal
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 12:  // Add Single Priority Core Capital
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core PRIORITY");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 13:  // Add Single Priority Core Lowercase
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 14:  // Add Single Priority Core Mixed Case
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core PrIoRiTy");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 15:  // Add Two Priority Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 16:  // Add Single Priority Core Remove Twice
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 17:  // Add Single Priority Core No Removal
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 18:  // Add Both Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 19:  // Add Both Cores Remove FIFO
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 20:  // Add Both Cores Remove Priority
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 21:  // Add Both Cores Remove Both
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 22:  // Add Both Cores Remove Out Of Order
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 23:  // Add Both Cores Remove Unknown
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("No core with ID 2.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 24:  // Add Both Cores Remove None
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 25:  // Add Unknown Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core abc");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Specified core type is unknown.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 26:  // Remove Core Does Not Exist
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("No core with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 27:  // Add Core No Scheduler
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      break;

    case 28:  // Show Single Fifo Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 0 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 29:  // Show Single Priority Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 0 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 30:  // Show Both Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 0 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 0 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 31:  // Show Core Does Not Exist
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 32:  // Show Core No Scheduler
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("remove_scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 33:  // Add Single Task Single FIFO Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("remove-core 0");  // HERE
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      break;

    case 34:  // Add Single Task Single Priority Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      break;

    case 35:  // Add Multiple Tasks Single FIFO Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      break;

    case 36:  // Add Multiple Tasks Single Priority Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      break;

    case 37:  // Add Multiple Tasks Two FIFO Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      EXPECT_CONTAINS("Core 1 is currently executing a task.");
      break;

    case 38:  // Add Multiple Tasks Two Priority Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      EXPECT_CONTAINS("Core 1 is currently executing a task.");
      break;

    case 39:  // Add Multiple Tasks Mixed Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      EXPECT_CONTAINS("Core 1 is currently executing a task.");
      break;

    case 40:  // Add Multiple Tasks Mixed Cores 2
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      EXPECT_CONTAINS("Core 1 is currently executing a task.");
      break;

    case 41:  // Add Single Task No Scheduler
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      break;

    case 42:  // Add Single Task No Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 43:  // Add Single Task No Removal Core Removal
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("Core 0 is currently executing a task.");
      break;

    case 44:  // Add Single Task No Removal Scheduler Removal
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Cannot perform that operation without first removing core(s).");
      break;

    case 45:  // Remove Task Does Not Exist
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-task 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("No task with ID 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 46:  // Update Scheduler Clock No Scheduler
      RUN_COMMAND("ticktock 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      break;

    case 47:  // Update Scheduler Clock No Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("ticktock 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("SimScheduler clock is now 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 48:  // Update Scheduler Clock No Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("ticktock 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 1.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 49:  // Show Single FIFO Core With Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      break;

    case 50:  // Show Single Priority Core With Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      break;

    case 51:  // Show Both Cores With Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      break;

    case 52:  // Show Tasks No Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Cannot perform that operation without a core.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 53:  // Show Tasks No Scheduler
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Cannot perform that operation without a scheduler.");
      break;

    case 54:  // Show Single FIFO Core With Completed Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("ticktock 20")
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 1 task(s) and has completed 1 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 10.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 55:  // Show Single Priority Core With Completed Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 2");
      RUN_COMMAND("add-task 20 1");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("ticktock 20")
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 1.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 20.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 56:  // Show Both Cores With Completed Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("ticktock 20");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 1 task(s) and has completed 1 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 20.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 1 task(s) and has completed 1 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 0 task(s) and has completed 1 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 10.");
      EXPECT_CONTAINS("SimScheduler clock is now 40.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 0 task(s) and has completed 2 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 0 task(s) and has completed 1 "
          "task(s).");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 57:  // Add 12 Tasks 3 Fifo Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 50 2");
      RUN_COMMAND("add-task 60 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 80 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 100 1");
      RUN_COMMAND("add-task 110 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 2.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 50, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 60, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 80, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 100, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 110, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      break;

    case 58:  // Add 12 Tasks 3 Priority Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 50 2");
      RUN_COMMAND("add-task 60 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 80 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 100 1");
      RUN_COMMAND("add-task 110 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 50, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 60, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 80, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 100, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 110, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      break;

    case 59:  // Add 12 Tasks 6 Mixed Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 5 2");
      RUN_COMMAND("add-task 6 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 8 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 10 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("show-core 3");
      RUN_COMMAND("show-core 4");
      RUN_COMMAND("show-core 5");
      RUN_COMMAND("show-task 0")
      RUN_COMMAND("show-task 1")
      RUN_COMMAND("show-task 2")
      RUN_COMMAND("show-task 3")
      RUN_COMMAND("show-task 4")
      RUN_COMMAND("show-task 5")
      RUN_COMMAND("show-task 6")
      RUN_COMMAND("show-task 7")
      RUN_COMMAND("show-task 8")
      RUN_COMMAND("show-task 9")
      RUN_COMMAND("show-task 10")
      RUN_COMMAND("show-task 11")
      RUN_COMMAND("ticktock 200")
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 3.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 4.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 5.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 5, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 6, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 8, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 10, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 3 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 4 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 5 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 0, task time 20, pending execution time 20, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 0, task time 30, pending execution time 30, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 3, time arrival 0, task time 40, pending execution time 40, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 4, time arrival 0, task time 5, pending execution time 5, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 5, time arrival 0, task time 6, pending execution time 6, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 6, time arrival 0, task time 70, pending execution time 70, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 7, time arrival 0, task time 8, pending execution time 8, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 8, time arrival 0, task time 90, pending execution time 90, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 9, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 10, time arrival 0, task time 10, pending execution time 10, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 11, time arrival 0, task time 120, pending execution time 120, "
          "priority 3.");
      break;

    case 60:  // Add Tasks Single Fifo Core Show All
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1")
      RUN_COMMAND("add-task 30 2")
      RUN_COMMAND("add-task 40 5")
      RUN_COMMAND("add-task 20 3")
      RUN_COMMAND("show-task 0")
      RUN_COMMAND("show-task 1")
      RUN_COMMAND("show-task 3")
      RUN_COMMAND("show-task 2")
      RUN_COMMAND("exit")
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 0, task time 30, pending execution time 30, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 3, time arrival 0, task time 20, pending execution time 20, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 0, task time 40, pending execution time 40, "
          "priority 5.");
      break;

    case 61:  // Add Tasks Single Fifo Core Show All
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1")
      RUN_COMMAND("add-task 30 2")
      RUN_COMMAND("add-task 40 5")
      RUN_COMMAND("add-task 20 3")
      RUN_COMMAND("show-task 0")
      RUN_COMMAND("show-task 1")
      RUN_COMMAND("show-task 2")
      RUN_COMMAND("show-task 3")
      RUN_COMMAND("exit")
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 0, task time 30, pending execution time 30, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 0, task time 40, pending execution time 40, "
          "priority 5.");
      EXPECT_CONTAINS(
          "Task 3, time arrival 0, task time 20, pending execution time 20, "
          "priority 3.");
      break;

    case 62:  // Show Single Task
      RUN_COMMAND("add-scheduler")
      RUN_COMMAND("add-core priority")
      RUN_COMMAND("add-task 10 1")
      RUN_COMMAND("show-task 0")
      RUN_COMMAND("ticktock 10")
      RUN_COMMAND("remove-core 0")
      RUN_COMMAND("remove-scheduler")
      RUN_COMMAND("exit")
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 63:  // Show Multiple Tasks
      RUN_COMMAND("add-scheduler")
      RUN_COMMAND("add-core fifo")
      RUN_COMMAND("add-task 10 1")
      RUN_COMMAND("add-task 20 2")
      RUN_COMMAND("add-task 30 3")
      RUN_COMMAND("ticktock 10")
      RUN_COMMAND("ticktock 20")
      RUN_COMMAND("ticktock 30")
      RUN_COMMAND("remove-core 0")
      RUN_COMMAND("remove-scheduler")
      RUN_COMMAND("exit")
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 10.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 30.");
      EXPECT_CONTAINS("SimScheduler clock is now 60.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 64:  // Add 12 Tasks 3 Fifo Cores Completed
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 50 2");
      RUN_COMMAND("add-task 60 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 80 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 100 1");
      RUN_COMMAND("add-task 110 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("ticktock 500");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-scheduler")
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 2.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 50, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 60, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 80, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 100, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 110, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 3 which was executed after waiting 10.");
      EXPECT_CONTAINS("Removed task 6 which was executed after waiting 50.");
      EXPECT_CONTAINS("Removed task 9 which was executed after waiting 120.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 4 which was executed after waiting 20.");
      EXPECT_CONTAINS("Removed task 7 which was executed after waiting 70.");
      EXPECT_CONTAINS("Removed task 10 which was executed after waiting 150.");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 5 which was executed after waiting 30.");
      EXPECT_CONTAINS("Removed task 8 which was executed after waiting 90.");
      EXPECT_CONTAINS("Removed task 11 which was executed after waiting 180.");
      EXPECT_CONTAINS("SimScheduler clock is now 500.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed core 2.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 65:  // Add 12 Tasks 3 Priority Cores Completed
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 50 2");
      RUN_COMMAND("add-task 60 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 80 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 100 1");
      RUN_COMMAND("add-task 110 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("ticktock 500");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-scheduler")
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 50, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 60, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 80, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 100, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 110, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 3 which was executed after waiting 10.");
      EXPECT_CONTAINS("Removed task 6 which was executed after waiting 50.");
      EXPECT_CONTAINS("Removed task 9 which was executed after waiting 120.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 4 which was executed after waiting 20.");
      EXPECT_CONTAINS("Removed task 7 which was executed after waiting 70.");
      EXPECT_CONTAINS("Removed task 10 which was executed after waiting 150.");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 5 which was executed after waiting 30.");
      EXPECT_CONTAINS("Removed task 8 which was executed after waiting 90.");
      EXPECT_CONTAINS("Removed task 11 which was executed after waiting 180.");
      EXPECT_CONTAINS("SimScheduler clock is now 500.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed core 2.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 66:  // Add 12 Tasks 6 Mixed Cores Completed
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("add-task 5 2");
      RUN_COMMAND("add-task 6 3");
      RUN_COMMAND("add-task 70 1");
      RUN_COMMAND("add-task 8 2");
      RUN_COMMAND("add-task 90 3");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 10 2");
      RUN_COMMAND("add-task 120 3");
      RUN_COMMAND("show-core 0");
      RUN_COMMAND("show-core 1");
      RUN_COMMAND("show-core 2");
      RUN_COMMAND("show-core 3");
      RUN_COMMAND("show-core 4");
      RUN_COMMAND("show-core 5");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("show-task 1");
      RUN_COMMAND("show-task 2");
      RUN_COMMAND("show-task 3");
      RUN_COMMAND("show-task 4");
      RUN_COMMAND("show-task 5");
      RUN_COMMAND("show-task 6");
      RUN_COMMAND("show-task 7");
      RUN_COMMAND("show-task 8");
      RUN_COMMAND("show-task 9");
      RUN_COMMAND("show-task 10");
      RUN_COMMAND("show-task 11");
      RUN_COMMAND("ticktock 200");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-core 3");
      RUN_COMMAND("remove-core 4");
      RUN_COMMAND("remove-core 5");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 3.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 4.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 5.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 40, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 4, task time of 5, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 5, task time of 6, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 6, task time of 70, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 7, task time of 8, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 8, task time of 90, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 9, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 10, task time of 10, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 11, task time of 120, and priority of 3.");
      EXPECT_CONTAINS(
          "Core 0 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 1 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 2 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 3 is currently assigned 1 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 4 is currently assigned 2 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Core 5 is currently assigned 4 task(s) and has completed 0 "
          "task(s).");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 0, task time 20, pending execution time 20, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 0, task time 30, pending execution time 30, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 3, time arrival 0, task time 40, pending execution time 40, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 4, time arrival 0, task time 5, pending execution time 5, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 5, time arrival 0, task time 6, pending execution time 6, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 6, time arrival 0, task time 70, pending execution time 70, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 7, time arrival 0, task time 8, pending execution time 8, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 8, time arrival 0, task time 90, pending execution time 90, "
          "priority 3.");
      EXPECT_CONTAINS(
          "Task 9, time arrival 0, task time 10, pending execution time 10, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 10, time arrival 0, task time 10, pending execution time 10, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 11, time arrival 0, task time 120, pending execution time 120, "
          "priority 3.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 8 which was executed after waiting 10.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 10 which was executed after waiting 20.");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 3 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 6 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 4 which was executed after waiting 70.");
      EXPECT_CONTAINS("Removed task 9 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 7 which was executed after waiting 10.");
      EXPECT_CONTAINS("Removed task 5 which was executed after waiting 18.");
      EXPECT_CONTAINS("Removed task 11 which was executed after waiting 24.");
      EXPECT_CONTAINS("SimScheduler clock is now 200.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed core 2.");
      EXPECT_CONTAINS("Removed core 3.");
      EXPECT_CONTAINS("Removed core 4.");
      EXPECT_CONTAINS("Removed core 5.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 67:  // Add Scheduler Remove Scheduler Twice
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Removed scheduler.");
      EXPECT_DOES_NOT_CONTAINS("Scheduler not found.", false);
      break;

    case 68:  // Add Core Remove Core Twice
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler")
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_DOES_NOT_CONTAINS("Core not found.", false);
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 69:  // Add Scheduler Remove Scheduler Twice With Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("ticktock 10")
      RUN_COMMAND("remove-core 0")
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("ticktock 20")
      RUN_COMMAND("remove-core 0")
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 20, and priority of 2.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 20.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 70:  // Add Core Remove Core Twice With Tasks
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("ticktock 10")
      RUN_COMMAND("remove-core 0")
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("ticktock 20")
      RUN_COMMAND("remove-core 1")
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 71:  // Add Fifo While Executing
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 20 1");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("add-task 40 2");
      RUN_COMMAND("ticktock 50");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 20, and priority of 1.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 40, and priority of 2.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 10.");
      EXPECT_CONTAINS("SimScheduler clock is now 60.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 72:  // Add Priority While Executing
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("add-task 40 1");
      RUN_COMMAND("ticktock 50");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 20, and priority of 2.");
      EXPECT_CONTAINS("SimScheduler clock is now 10.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 40, and priority of 1.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 40.");
      EXPECT_CONTAINS("SimScheduler clock is now 60.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 73:  // Add Max Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-core 3");
      RUN_COMMAND("remove-core 4");
      RUN_COMMAND("remove-core 5");
      RUN_COMMAND("remove-core 6");
      RUN_COMMAND("remove-core 7");
      RUN_COMMAND("remove-scheduler");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 3.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 4.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 5.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 6.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 7.");
      EXPECT_CONTAINS("Cannot add another core.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed core 2.");
      EXPECT_CONTAINS("Removed core 3.");
      EXPECT_CONTAINS("Removed core 4.");
      EXPECT_CONTAINS("Removed core 5.");
      EXPECT_CONTAINS("Removed core 6.");
      EXPECT_CONTAINS("Removed core 7.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 74:  // Add Max Cores Remove Add Again
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("remove-core 2");
      RUN_COMMAND("remove-core 3");
      RUN_COMMAND("remove-core 4");
      RUN_COMMAND("remove-core 5");
      RUN_COMMAND("remove-core 6");
      RUN_COMMAND("remove-core 7");
      RUN_COMMAND("remove-core 8");
      RUN_COMMAND("remove-core 9");
      RUN_COMMAND("remove-scheduler");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 2.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 3.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 4.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 5.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 6.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 7.");
      EXPECT_CONTAINS("Cannot add another core.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 8.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 9.");
      EXPECT_CONTAINS("Cannot add another core.");
      EXPECT_CONTAINS("Removed core 2.");
      EXPECT_CONTAINS("Removed core 3.");
      EXPECT_CONTAINS("Removed core 4.");
      EXPECT_CONTAINS("Removed core 5.");
      EXPECT_CONTAINS("Removed core 6.");
      EXPECT_CONTAINS("Removed core 7.");
      EXPECT_CONTAINS("Removed core 8.");
      EXPECT_CONTAINS("Removed core 9.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 75:  // Show Tasks Mid-Execution Fifo Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("ticktock 5");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("show-task 1");
      RUN_COMMAND("ticktock 25");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("SimScheduler clock is now 5.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 5, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 5, task time 20, pending execution time 20, "
          "priority 2.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 5.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 76:  // Show Tasks Mid-Execution Priority Core
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 2");
      RUN_COMMAND("ticktock 5");
      RUN_COMMAND("add-task 20 1");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("show-task 1");
      RUN_COMMAND("ticktock 25");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 2.");
      EXPECT_CONTAINS("SimScheduler clock is now 5.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 1.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 5, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 5, task time 20, pending execution time 20, "
          "priority 1.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 20.");
      EXPECT_CONTAINS("SimScheduler clock is now 30.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 77:  // Show Task Mid-Execution Mixed Cores
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("ticktock 5");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 40 4");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("show-task 1");
      RUN_COMMAND("show-task 2");
      RUN_COMMAND("show-task 0");
      RUN_COMMAND("ticktock 5");
      RUN_COMMAND("add-task 20 1")
      RUN_COMMAND("show-task 1");
      RUN_COMMAND("show-task 2");
      RUN_COMMAND("show-task 3");
      RUN_COMMAND("ticktock 45");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-core 1");
      RUN_COMMAND("remove-scheduler");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 1.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS("SimScheduler clock is now 5.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 40, and priority of 4.");
      EXPECT_CONTAINS(
          "Task 0, time arrival 0, task time 10, pending execution time 5, "
          "priority 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 5, task time 20, pending execution time 20, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 5, task time 40, pending execution time 40, "
          "priority 4.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.")
      EXPECT_CONTAINS("SimScheduler clock is now 10.")
      EXPECT_CONTAINS(
          "Added task with ID 3, task time of 20, and priority of 1.");
      EXPECT_CONTAINS(
          "Task 1, time arrival 5, task time 20, pending execution time 15, "
          "priority 2.");
      EXPECT_CONTAINS(
          "Task 2, time arrival 5, task time 40, pending execution time 40, "
          "priority 4.");
      EXPECT_CONTAINS(
          "Task 3, time arrival 10, task time 20, pending execution time 20, "
          "priority 1.");
      EXPECT_CONTAINS("Removed task 3 which was executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 1 which was executed after waiting 20.");
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 5.");
      EXPECT_CONTAINS("SimScheduler clock is now 55.");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed core 1.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 78:  // Remove Task Not First In Queue FIFO
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core fifo");
      RUN_COMMAND("add-task 10 1");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 3");
      RUN_COMMAND("remove-task 0");
      RUN_COMMAND("remove-task 1");
      RUN_COMMAND("remove-task 2");
      RUN_COMMAND("ticktock 10");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'fifo' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 1.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 3.");
      EXPECT_CONTAINS("Task 0 is currently being executed.")
      EXPECT_CONTAINS("Removed task 1 which was not executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 2 which was not executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 0 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 10");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    case 79:  // Remove Task Not First In Queue Priority
      RUN_COMMAND("add-scheduler");
      RUN_COMMAND("add-core priority");
      RUN_COMMAND("add-task 10 3");
      RUN_COMMAND("add-task 20 2");
      RUN_COMMAND("add-task 30 1");
      RUN_COMMAND("remove-task 0");
      RUN_COMMAND("remove-task 1");
      RUN_COMMAND("remove-task 2");
      RUN_COMMAND("ticktock 30");
      RUN_COMMAND("remove-core 0");
      RUN_COMMAND("remove-scheduler");
      RUN_COMMAND("exit");
      EXPECT_CONTAINS("Added scheduler.");
      EXPECT_CONTAINS("Added core of type 'priority' with ID 0.");
      EXPECT_CONTAINS(
          "Added task with ID 0, task time of 10, and priority of 3.");
      EXPECT_CONTAINS(
          "Added task with ID 1, task time of 20, and priority of 2.");
      EXPECT_CONTAINS(
          "Added task with ID 2, task time of 30, and priority of 1.");
      EXPECT_CONTAINS("Removed task 0 which was not executed after waiting 0.");
      EXPECT_CONTAINS("Removed task 1 which was not executed after waiting 0.");
      EXPECT_CONTAINS("Task 2 is currently being executed.")
      EXPECT_CONTAINS("Removed task 2 which was executed after waiting 0.");
      EXPECT_CONTAINS("SimScheduler clock is now 30");
      EXPECT_CONTAINS("Removed core 0.");
      EXPECT_CONTAINS("Removed scheduler.");
      break;

    default:
      RUN_COMMAND("exit")
      cerr << "Test case not found: #" << test_number << "\n\r";
      return TEST_RESULT_FAIL;
  }

  cerr << "PASSED Test " << test_number << "." << "\n\r" << "\n\r"
       << "--------------------------------------------------------------------"
          "--------"
       << "\n\r" << "\n\r";
  return TEST_RESULT_PASS;
}
