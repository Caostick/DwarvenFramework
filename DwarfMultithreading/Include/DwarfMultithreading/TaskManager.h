#pragma once

#include <functional>
#include <atomic>
#include <mutex>

#include <DwarvenCore/ThreadSafeRingBuffer.h>
#include <DwarvenCore/Vector.h>
#include <DwarvenCore/Types.h>

namespace df {
	struct Task {
		struct Args {
			uint32 m_TaskIndex;			// Job index relative to dispatch (like SV_DispatchThreadID in HLSL)
			uint32 m_GroupId;			// Group index relative to dispatch (like SV_GroupID in HLSL)
			uint32 m_GroupIndex;		// Job index relative to group (like SV_GroupIndex in HLSL)
			bool m_IsFirstTaskInGroup;	// Is the current job the first one in the group?
			bool m_IsLastTaskInGroup;	// Is the current job the last one in the group?
			void* m_SharedMemory;		// Stack memory shared within the current group (jobs within a group execute serially)
		};

		struct Context {
			std::atomic<uint32> m_Counter { 0 };
		};

		Context* m_Context;
		std::function<void(Task::Args)> m_TaskFunction;
		uint32 m_GroupId;
		uint32 m_GroupTaskOffset;
		uint32 m_GroupTaskEnd;
		uint32 m_SharedMemorySize;
	};

	class TaskManager {
	public:
		void Init();
		void Release();

		auto GetThreadCount() const->uint32;

		// Add a task to execute asynchronously. Any idle thread will execute this.
		void Execute(Task::Context& ctx, const std::function<void(Task::Args)>& taskFunc);

		// Add a task to execute asynchronously for certain thread.
		bool ExecuteOnWorkerThread(uint32 threadIndex, Task::Context& ctx, const std::function<void(Task::Args)>& taskFunction);

		// Divide a task onto multiple jobs and execute in parallel.
		//	jobCount	: how many jobs to generate for this task.
		//	groupSize	: how many jobs to execute per thread. Jobs inside a group execute serially. It might be worth to increase for small jobs
		//	task		: receives a wiJobArgs as parameter
		void Dispatch(Task::Context& ctx, uint32 taskCount, uint32 groupSize, const std::function<void(Task::Args)>& taskFunc, size_t sharedMemorySize = 0);

		// Returns the amount of job groups that will be created for a set number of jobs and group size
		auto DispatchGroupCount(uint32 taskCount, uint32 groupSize) -> uint32;

		// Check if any threads are working currently or not
		bool IsBusy(const Task::Context& ctx);

		// Wait until all threads become idle
		void Wait(const Task::Context& ctx);

	private:
		bool Work();
		bool WorkThread(uint32 threadIndex);

	private:
		bool m_IsWorking = false;

		uint32 m_WorkerThreadCount;

		df::ThreadSafeRingBuffer<Task, 256> m_TaskQueue;
		df::ThreadSafeRingBuffer<Task, 256>* m_PerThreadTaskQueue;

		std::condition_variable m_WakeCondition;
		std::mutex m_WakeMutex;
	};
}