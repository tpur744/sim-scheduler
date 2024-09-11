#include "app.hpp"

#include <iostream>

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

void App::AddScheduler() {}

void App::RemoveScheduler() {}

void App::AddCore(const std::string &core_type) {}

void App::RemoveCore(const std::string &core_id) {}

void App::AddTask(const std::string &task_time, const std::string &priority) {}

void App::RemoveTask(const std::string &task_id) {}

void App::ShowCore(const std::string &core_id) const {}

void App::ShowTask(const std::string &task_id) const {}
