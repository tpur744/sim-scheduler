#include "Core.hpp"

// Constructor to initialize ID and pending time
Core::Core(int id) : id_(id), pending_time_(0) {}

// Get the total pending time
int Core::getPendingTime() const { return pending_time_; }

// Virtual destructor
Core::~Core() {}
