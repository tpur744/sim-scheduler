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
  int numTicks = std::stoi(num_ticktock);
  current_time_ += numTicks;
  sim_scheduler_.TickTock(numTicks);
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

  if (sim_scheduler_.GetNextCoreID() >= 8) {
    cout << "Cannot add another core." << endl;
    return;
  }

  Core *newCore = nullptr;
  std::string lowerCoreType = Utils::GetLowercase(core_type);
  if (lowerCoreType == "fifo") {
    newCore = new FIFOCore(sim_scheduler_.GetNextCoreID());
  } else if (lowerCoreType == "priority") {
    newCore = new PriorityCore(sim_scheduler_.GetNextCoreID());
  } else {
    cout << "Specified core type is unknown." << endl;
    return;
  }

  sim_scheduler_.AddCore(newCore);
  cout << "Added core of type '" << lowerCoreType << "' with ID "
       << sim_scheduler_.GetNextCoreID() - 1 << "." << endl;
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
  if (id < 0 || id >= sim_scheduler_.GetNextCoreID()) {
    std::cout << "No core with ID " << id << "." << std::endl;
    return;
  }

  if (sim_scheduler_.RemoveCore(id)) {
    std::cout << "Removed core " << id << "." << std::endl;
  }
}

void App::AddTask(const std::string &task_time, const std::string &priority) {
  if (!sim_scheduler_.IsSchedulerAdded()) {
    std::cout << "Cannot add a task without a scheduler." << std::endl;
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
  // REMOVE THIS CODE AT SOME POINT
  // int time = std::stoi(task_time);
  // int prio = std::stoi(priority);

  // // Add the task and retrieve its ID
  // int task_id = sim_scheduler_.AddTask(time, prio, current_time_);

  // std::cout << "Added task with ID " << task_id << ", task time of " << time
  //           << ", and priority of " << prio << "." << std::endl;

  // // Now assign the tasks to available cores
  // sim_scheduler_.AssignTasks();
  // END REMOVE

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
  if (id < 0 || id >= sim_scheduler_.GetNextCoreID()) {
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

  // // Retrieve the task from the SimScheduler
  // Task *task = sim_scheduler_.GetTask(id);
  // if (task == nullptr) {
  //   std::cout << "No task with ID " << task_id << "." << std::endl;
  //   return;
  // }

  // int elapsed_time =
  //     current_time_ - task->GetExecutedTime();  // Adjust if needed
  // int pending_execution_time = task->GetTime() - elapsed_time;
  // if (pending_execution_time < 0) {
  //   pending_execution_time = 0;  // Ensure it doesn't go negative
  // }

  // // Output task details
  // std::cout << "Task " << task->GetID() << ", time arrival "
  //           << task->GetArrivalTime() << ", task time " << task->GetTime()
  //           << ", pending execution time " << pending_execution_time
  //           << ", priority " << task->GetPriority() << "." << std::endl;
}
