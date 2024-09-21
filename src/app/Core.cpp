#include "Core.hpp"

// Constructor to initialize ID and pending time
Core::Core(int id) : id(id), pending_time(0) {}

// Get the total pending time
int Core::getPendingTime() const { return pending_time; }

// Virtual destructor
Core::~Core() {}
