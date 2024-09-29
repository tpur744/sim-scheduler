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

App::App() : current_time_(0) {}

App::~App() {}

void App::TickTock(const std::string &num_ticktock) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }
  int num_ticks = std::stoi(num_ticktock);
  current_time_ += num_ticks;
  sim_scheduler_.TickTock(num_ticks);
  std::cout << "SimScheduler clock is now " << current_time_ << "."
            << std::endl;
}

void App::AddScheduler() {
  if (sim_scheduler_.IsSchedulerAdded()) {
    // Print error message if the scheduler is already added
    cout << "Cannot add another scheduler." << endl;
  } else {
    // Add the scheduler and print success message
    sim_scheduler_.AddScheduler();
    cout << "Added scheduler." << endl;
  }
}

void App::RemoveScheduler() {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    // Print error message if the scheduler is not added
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }
  if (sim_scheduler_.RemoveScheduler()) {
    std::cout << "Removed scheduler." << std::endl;
  } else {
    std::cout << "Cannot perform that operation without first removing core(s)."
              << std::endl;
  }
}

void App::AddCore(const std::string &core_type) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    cout << "Cannot perform that operation without a scheduler." << endl;
    return;
  }

  int next_core_id =
      sim_scheduler_
          .GetNextCoreID();  // Get the next available core ID before adding

  if (next_core_id == -1) {
    cout << "Cannot add another core." << endl;
    return;
  }

  Core *newCore = nullptr;
  std::string lower_core_type = Utils::GetLowercase(core_type);
  if (lower_core_type == "fifo") {
    newCore = new FIFOCore(next_core_id);  // Pass the stored ID to the new core
  } else if (lower_core_type == "priority") {
    newCore =
        new PriorityCore(next_core_id);  // Pass the stored ID to the new core
  } else {
    cout << "Specified core type is unknown." << endl;
    return;
  }

  sim_scheduler_.AddCore(newCore);
  cout << "Added core of type '" << lower_core_type << "' with ID "
       << next_core_id << "." << endl;  // Use the stored ID in the output
}

void App::RemoveCore(const std::string &core_id) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!sim_scheduler_.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(core_id)) {
    std::cout << "Specified core type is unknown." << std::endl;
    return;
  }

  int id = std::stoi(core_id);  // Convert core_id from string to integer
  if (id < 0 || id >= 8) {
    std::cout << "No core with ID " << id << "." << std::endl;
    return;
  }

  if (sim_scheduler_.RemoveCore(id)) {
    std::cout << "Removed core " << id << "." << std::endl;
  }
}

void App::AddTask(const std::string &task_time, const std::string &priority) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!sim_scheduler_.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(task_time) || !Utils::IsInteger(priority)) {
    std::cout << "Invalid time or priority." << std::endl;
    return;
  }

  sim_scheduler_.AddTaskToCore(std::stoi(task_time), std::stoi(priority),
                               current_time_);
}

void App::RemoveTask(const std::string &task_id) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!sim_scheduler_.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  int id = std::stoi(task_id);

  // Now remove the task
  sim_scheduler_.RemoveTask(id);
}

void App::ShowCore(const std::string &excore_id) const {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!sim_scheduler_.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  if (!Utils::IsInteger(excore_id)) {
    std::cout << "Specified core type is unknown." << std::endl;
    return;
  }

  int id = std::stoi(excore_id);
  if (id < 0 || id >= 8) {
    std::cout << "No core with ID " << excore_id << "." << std::endl;
    return;
  }

  Core *core = sim_scheduler_.GetCore(id);
  if (core == nullptr) {
    std::cout << "No core with ID " << excore_id << "." << std::endl;
    return;
  }

  std::cout << "Core " << excore_id << " is currently assigned "
            << core->GetAssignedTaskCount() << " task(s) and has completed "
            << core->GetCompletedTaskCount() << " task(s)." << std::endl;
}

void App::ShowTask(const std::string &task_id) const {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot perform that operation without a scheduler."
              << std::endl;
    return;
  }

  if (!sim_scheduler_.HasCores()) {
    std::cout << "Cannot perform that operation without a core." << std::endl;
    return;
  }

  // Convert task_id to integer
  int id = std::stoi(task_id);
  sim_scheduler_.ShowTask(id);
}
