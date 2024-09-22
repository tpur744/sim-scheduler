#include "app.hpp"

#include <iostream>

#include "Core.hpp"
#include "FIFOCore.hpp"
#include "PriorityCore.hpp"
#include "SimScheduler.hpp"
#include "message.hpp"
#include "utils.hpp"

using namespace std;

/*
*****************************
  This is where you start!
  However, make sure you do not not write all your code in a single file!
*****************************
*/

App::App() {}

App::~App() {}

void App::TickTock(const std::string &num_ticktock) {}

void App::AddScheduler() {
  if (simScheduler.IsSchedulerAdded()) {
    // Print error message if the scheduler is already added
    cout << "Cannot add another scheduler." << endl;
  } else {
    // Add the scheduler and print suc cess message
    simScheduler.AddScheduler();
    cout << "Added scheduler." << endl;
  }
}

void App::RemoveScheduler() {
  if (!simScheduler.IsSchedulerAdded()) {
    // Print error message if the scheduler is not added
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }
  if (simScheduler.RemoveScheduler()) {
    std::cout << "Removed scheduler." << std::endl;
  } else {
    std::cout << "Cannot perform that operation without first removing core(s)."
              << std::endl;
  }
}

void App::AddCore(const std::string &core_type) {
  if (!simScheduler.IsSchedulerAdded()) {
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }

  if (simScheduler.getNextCoreId() >= 8) {
    cout << "Cannot add another core." << endl;
    return;
  }

  Core *newCore = nullptr;
  std::string lowerCoreType = Utils::GetLowercase(core_type);
  if (lowerCoreType == "fifo") {
    newCore = new FIFOCore(simScheduler.getNextCoreId());
  } else if (lowerCoreType == "priority") {
    newCore = new PriorityCore(simScheduler.getNextCoreId());
  } else {
    cout << "Specified core type is unknown." << endl;
    return;
  }

  simScheduler.AddCore(newCore);
  cout << "Added core of type '" << lowerCoreType << "' with ID "
       << simScheduler.getNextCoreId() - 1 << "." << endl;
}

void App::RemoveCore(const std::string &core_id) {
  if (!simScheduler.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!simScheduler.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(core_id)) {
    std::cout << "Specified core type is unknown." << std::endl;
    return;
  }

  int id = std::stoi(core_id);  // Convert core_id from string to integer
  if (id < 0 || id >= simScheduler.getNextCoreId()) {
    std::cout << "No core with ID " << id << "." << std::endl;
    return;
  }

  if (simScheduler.RemoveCore(id)) {
    std::cout << "Removed core " << id << "." << std::endl;
  } else {
    std::cout << "Core " << id << "is currently executing a task." << std::endl;
  }
}

void App::AddTask(const std::string &task_time, const std::string &priority) {
  if (!simScheduler.IsSchedulerAdded()) {
    std::cout << "Cannot add a task without a scheduler." << std::endl;
    return;
  }

  if (!simScheduler.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(task_time) || !Utils::IsInteger(priority)) {
    std::cout << "Invalid time or priority." << std::endl;
    return;
  }

  int time = std::stoi(task_time);
  int prio = std::stoi(priority);

  // Add the task and retrieve its ID
  int task_id =
      simScheduler.AddTask(time, prio);  // Modify AddTask to return task ID

  std::cout << "Added task with ID " << task_id << ", task time of " << time
            << ", and priority of " << prio << "." << std::endl;
}

void App::RemoveTask(const std::string &task_id) {}

void App::ShowCore(const std::string &excore_id) const {
  if (!simScheduler.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!simScheduler.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(excore_id)) {
    std::cout << "Specified core type is unknown." << std::endl;
    return;
  }

  int id = std::stoi(excore_id);
  if (id < 0 || id >= simScheduler.getNextCoreId()) {
    std::cout << "No core with ID " << excore_id << "." << std::endl;
    return;
  }

  if (simScheduler.GetCore(id) == nullptr) {
    std::cout << "No core with ID " << excore_id << "." << std::endl;
    return;
  }

  std::cout << "Core " << excore_id
            << " is currently assigned 0 task(s) and has completed 0 task(s)."
            << std::endl;
}

void App::ShowTask(const std::string &task_id) const {}
