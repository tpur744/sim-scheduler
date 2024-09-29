option(CUSTOM_CASES_PART_1 "Run the Custom Test Cases for Part 1" ON)
option(CUSTOM_CASES_PART_2 "Run the Custom Test Cases for Part 1" ON)

# Add the test executable and link it with the library targets
add_executable(tester_private tester.cpp)

# Include the "src" directory to access the source files
target_include_directories(tester_private PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Link the subdirectory targets to the test executable
target_link_libraries(tester_private SrcTarget SrcAppSupportTarget)

if (CUSTOM_CASES_PART_1)
    # Tests for Adding and Removing Scheduler
    add_test(NAME Part_1_AddRemoveScheduler COMMAND tester_private 1)
    add_test(NAME Part_1_RemoveSchedulerDoesNotExist COMMAND tester_private 2)
    add_test(NAME Part_1_AddTwiceScheduler COMMAND tester_private 3)
    add_test(NAME Part_1_AddRemoveTwiceScheduler COMMAND tester_private 4)
    add_test(NAME Part_1_AddOnceRemoveTwiceScheduler COMMAND tester_private 5)

    # Tests for Adding and Removing FIFO Core
    add_test(NAME Part_1_AddSingleFifoCoreCapital COMMAND tester_private 6)
    add_test(NAME Part_1_AddSingleFifoCoreLowercase COMMAND tester_private 7)
    add_test(NAME Part_1_AddSingleFifoCoreMixedCase COMMAND tester_private 8)
    add_test(NAME Part_1_AddTwoFifoCore COMMAND tester_private 9)
    add_test(NAME Part_1_AddSingleFifoCoreRemoveTwice COMMAND tester_private 10)
    add_test(NAME Part_1_AddSingleFifoCoreNoRemoval COMMAND tester_private 11)

    # Tests for Adding and Removing Priority Core
    add_test(NAME Part_1_AddSinglePriorityCoreCapital COMMAND tester_private 12)
    add_test(NAME Part_1_AddSinglePriorityCoreLowercase COMMAND tester_private 13)
    add_test(NAME Part_1_AddSinglePriorityCoreMixedCase COMMAND tester_private 14)
    add_test(NAME Part_1_AddTwoPriorityCore COMMAND tester_private 15)
    add_test(NAME Part_1_AddSinglePriorityCoreRemoveTwice COMMAND tester_private 16)
    add_test(NAME Part_1_AddSinglePriorityCoreNoRemoval COMMAND tester_private 17)

    # Tests for Adding and Removing Both Cores
    add_test(NAME Part_1_AddBothCores COMMAND tester_private 18)
    add_test(NAME Part_1_AddBothCoresRemoveFIFO COMMAND tester_private 19)
    add_test(NAME Part_1_AddBothCoresRemovePriority COMMAND tester_private 20)
    add_test(NAME Part_1_AddBothCoresRemoveBoth COMMAND tester_private 21)
    add_test(NAME Part_1_AddBothCoresRemoveOutOfOrder COMMAND tester_private 22)
    add_test(NAME Part_1_AddBothCoresRemoveUnknown COMMAND tester_private 23)
    add_test(NAME Part_1_AddBothCoresRemoveNone COMMAND tester_private 24)

    # Tests for Other Core Cases
    add_test(NAME Part_1_AddUnknownCore COMMAND tester_private 25)
    add_test(NAME Part_1_RemoveCoreDoesNotExist COMMAND tester_private 26)
    add_test(NAME Part_1_AddCoreNoScheduler COMMAND tester_private 27)

    # Tests for Showing the Cores
    add_test(NAME Part_1_ShowSingleFifoCore COMMAND tester_private 28)
    add_test(NAME Part_1_ShowSinglePriorityCore COMMAND tester_private 29)
    add_test(NAME Part_1_ShowBothCores COMMAND tester_private 30)
    add_test(NAME Part_1_ShowCoreDoesNotExist COMMAND tester_private 31)
    add_test(NAME Part_1_ShowCoreNoScheduler COMMAND tester_private 32)
endif ()

if (CUSTOM_CASES_PART_2)
    # Tests for adding and removing tasks
    add_test(NAME Part_2_AddSingleTaskSingleFifoCore COMMAND tester_private 33)
    add_test(NAME Part_2_AddSingleTaskSinglePriorityCore COMMAND tester_private 34)
    add_test(NAME Part_2_AddMultipleTasksSingleFifoCore COMMAND tester_private 35)
    add_test(NAME Part_2_AddMultipleTasksSinglePriorityCore COMMAND tester_private 36)
    add_test(NAME Part_2_AddMultipleTasksTwoFifoCores COMMAND tester_private 37)
    add_test(NAME Part_2_AddMultipleTasksTwoPriorityCores COMMAND tester_private 38)
    add_test(NAME Part_2_AddMultipleTasksMixedCores COMMAND tester_private 39)
    add_test(NAME Part_2_AddMultipleTasksMixedCores2 COMMAND tester_private 40)
    add_test(NAME Part_2_AddSingleTaskNoScheduler COMMAND tester_private 41)
    add_test(NAME Part_2_AddSingleTaskNoCores COMMAND tester_private 42)
    add_test(NAME Part_2_AddSingleTaskNoRemovalCoreRemoval COMMAND tester_private 43)
    add_test(NAME Part_2_AddSingleTaskNoRemovalSchedulerRemoval COMMAND tester_private 44)
    add_test(NAME Part_2_RemoveTaskDoesNotExist COMMAND tester_private 45)

    # Tests for updating the scheduler clock
    add_test(NAME Part_2_UpdateSchedulerClockNoScheduler COMMAND tester_private 46)
    add_test(NAME Part_2_UpdateSchedulerClockNoCores COMMAND tester_private 47)
    add_test(NAME Part_2_UpdateSchedulerClockNoTasks COMMAND tester_private 48)

    # Tests for showing cores with tasks
    add_test(NAME Part_2_ShowSingleFifoCoreWithTasks COMMAND tester_private 49)
    add_test(NAME Part_2_ShowSinglePriorityCoreWithTasks COMMAND tester_private 50)
    add_test(NAME Part_2_ShowBothCoresWithTasks COMMAND tester_private 51)
    add_test(NAME Part_2_ShowTasksNoCore COMMAND tester_private 52)
    add_test(NAME Part_2_ShowTasksNoScheduler COMMAND tester_private 53)

    # Tests for showing cores that have completed tasks
    add_test(NAME Part_2_ShowSingleFifoCoreWithCompletedTasks COMMAND tester_private 54)
    add_test(NAME Part_2_ShowSinglePriorityCoreWithCompletedTasks COMMAND tester_private 55)
    add_test(NAME Part_2_ShowBothCoresWithCompletedTasks COMMAND tester_private 56)

    # Tests for other cases
    add_test(NAME Part_2_Add12Tasks3FifoCores COMMAND tester_private 57)
    add_test(NAME Part_2_Add12Tasks3PriorityCores COMMAND tester_private 58)
    add_test(NAME Part_2_Add12Tasks6MixedCores COMMAND tester_private 59)
    add_test(NAME Part_2_AddTasksSingleFifoCoreShowAll COMMAND tester_private 60)
    add_test(NAME Part_2_AddTasksSinglePriorityCoreShowAll COMMAND tester_private 61)

    # Tests for showing the tasks
    add_test(NAME Part_2_ShowSingleTask COMMAND tester_private 62)
    add_test(NAME Part_2_ShowMultipleTasks COMMAND tester_private 63)

    # Tests for showing the tasks that have completed
    add_test(NAME Part_2_Add12Tasks3FifoCoresCompleted COMMAND tester_private 64)
    add_test(NAME Part_2_Add12Tasks3PriorityCoresCompleted COMMAND tester_private 65)
    add_test(NAME Part_2_Add12Tasks6MixedCoresCompleted COMMAND tester_private 66)

    # Tests other edge cases
    add_test(NAME Part_2_AddSchedulerRemoveSchedulerTwice COMMAND tester_private 67)
    add_test(NAME Part_2_AddCoreRemoveCoreTwice COMMAND tester_private 68)
    add_test(NAME Part_2_AddSchedulerRemoveSchedulerTwiceWithTasks COMMAND tester_private 69)
    add_test(NAME Part_2_AddCoreRemoveCoreTwiceWithTasks COMMAND tester_private 70)
    add_test(NAME Part_2_AddFifoWhileExecuting COMMAND tester_private 71)
    add_test(NAME Part_2_AddPriorityWhileExecuting COMMAND tester_private 72)
    add_test(NAME Part_2_AddMaxCores COMMAND tester_private 73)
    add_test(NAME Part_2_AddMaxCoresRemoveAddAgain COMMAND tester_private 74)

    # Tests for showing tasks mid-execution TODO: Unsure if this implementation is correct, need to check
    add_test(NAME Part_2_ShowTasksMidExecutionFifoCore COMMAND tester_private 75)
    add_test(NAME Part_2_ShowTasksMidExecutionPriorityCore COMMAND tester_private 76)
    add_test(NAME Part_2_ShowTasksMidExecutionMixedCores COMMAND tester_private 77)

    # Tests for removing tasks not first in the queue
    add_test(NAME Part_2_RemoveTaskNotFirstInQueueFifo COMMAND tester_private 78)
    add_test(NAME Part_2_RemoveTaskNotFirstInQueuePriority COMMAND tester_private 79)



endif ()
