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
  if (simScheduler.isSchedulerAdded()) {
    // Print error message if the scheduler is already added
    cout << "Cannot add another scheduler." << endl;
  } else {
    // Add the scheduler and print suc cess message
    simScheduler.addScheduler();
    cout << "Added scheduler." << endl;
  }
}

void App::RemoveScheduler() {
  if (!simScheduler.isSchedulerAdded()) {
    // Print error message if the scheduler is not added
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }
  if (simScheduler.removeScheduler()) {
    std::cout << "Removed scheduler." << std::endl;
  } else {
    std::cout << "Cannot perform that operation without first removing core(s)."
              << std::endl;
  }
}

void App::AddCore(const std::string &core_type) {
  if (!simScheduler.isSchedulerAdded()) {
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }

  if (simScheduler.getNextCoreId() >= 8) {
    cout << "Cannot add another core." << endl;
    return;
  }

  Core *newCore = nullptr;
  std::string upperCoreType = Utils::GetUppercase(core_type);
  if (upperCoreType == "FIFO") {
    newCore = new FIFOCore(simScheduler.getNextCoreId());
  } else if (upperCoreType == "PRIORITY") {
    newCore = new PriorityCore(simScheduler.getNextCoreId());
  } else {
    cout << "Specified core type is unknown." << endl;
    return;
  }

  simScheduler.addCore(newCore);
  cout << "Added core of type: " << core_type
       << " with ID: " << simScheduler.getNextCoreId() - 1 << endl;
}

void App::RemoveCore(const std::string &core_id) {}

void App::AddTask(const std::string &task_time, const std::string &priority) {}

void App::RemoveTask(const std::string &task_id) {}

void App::ShowCore(const std::string &core_id) const {}

void App::ShowTask(const std::string &task_id) const {}
