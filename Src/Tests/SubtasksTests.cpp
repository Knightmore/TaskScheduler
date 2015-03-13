#include "Tests.h"
#include "../../TestFramework/UnitTest++/UnitTest++.h"
#include "../Scheduler.h"


SUITE(SubtasksTests)
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//namespace DeepSubtaskQueue
//{
//	template<size_t N>
//	void MT_CALL_CONV Fibonacci(MT::FiberContext& context, void* userData)
//	{
//		int& result = *(int*)userData;
//
//		MT::TaskDesc tasks[2];
//
//		int a = -100;
//		tasks[0] = MT::TaskDesc(Fibonacci<N - 1>, &a);
//
//		int b = -100;
//		tasks[1] = MT::TaskDesc(Fibonacci<N - 2>, &b);
//
//		context.RunSubtasks(&tasks[0], ARRAY_SIZE(tasks));
//
//		result = a + b;
//	}
//
//	template<>
//	void MT_CALL_CONV Fibonacci<0>(MT::FiberContext&, void* userData)
//	{
//		*(int*)userData = 0;
//	}
//
//	template<>
//	void MT_CALL_CONV Fibonacci<1>(MT::FiberContext&, void* userData)
//	{
//		*(int*)userData = 1;
//	}
//}
//
//// Checks one simple task
//TEST(DeepSubtaskQueue)
//{
//	printf("DeepSubtaskQueue\n");
//
//	MT::TaskScheduler scheduler;
//
//	int result = 0;
//	MT::TaskDesc task(DeepSubtaskQueue::Fibonacci<12>, &result);
//
//	scheduler.RunTasks(MT::TaskGroup::GROUP_0, &task, 1);
//
//	CHECK(scheduler.WaitAll(200));
//
//	CHECK_EQUAL(result, 144);
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace SubtaskGroup
{
	static MT::TaskGroup::Type sourceGroup = MT::TaskGroup::GROUP_1;
	static MT::TaskGroup::Type resultGroup = MT::TaskGroup::GROUP_UNDEFINED;

	void MT_CALL_CONV Subtask(MT::FiberContext& context, void*)
	{
		resultGroup = context.currentTask->taskGroup;
	}

	void MT_CALL_CONV Task(MT::FiberContext& context, void*)
	{
		MT::TaskDesc task(Subtask, 0);
		context.RunSubtasks(&task, 1);
	}
}

// Checks one simple task
TEST(SubtaskGroup)
{
	printf("SubtaskGroup\n");

	MT::TaskScheduler scheduler;

	MT::TaskDesc task(SubtaskGroup::Task, 0);
	scheduler.RunTasks(SubtaskGroup::sourceGroup, &task, 1);

	CHECK(scheduler.WaitAll(200));

	CHECK_EQUAL(SubtaskGroup::sourceGroup, SubtaskGroup::resultGroup);
}

// Checks one simple task
TEST(SubsequentSubtasks)
{
	printf("SubsequentSubtasks\n");

	MT::TaskScheduler scheduler;

	while (true)
	{
		MT::TaskDesc task(SubtaskGroup::Task, 0);
		scheduler.RunTasks(SubtaskGroup::sourceGroup, &task, 1);
		CHECK(scheduler.WaitAll(200));
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}