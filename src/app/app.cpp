#include "app.hpp"

#include <iostream>

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

void App::RemoveScheduler() {}

void App::AddCore(const std::string &core_type) {}

void App::RemoveCore(const std::string &core_id) {}

void App::AddTask(const std::string &task_time, const std::string &priority) {}

void App::RemoveTask(const std::string &task_id) {}

void App::ShowCore(const std::string &core_id) const {}

void App::ShowTask(const std::string &task_id) const {}
