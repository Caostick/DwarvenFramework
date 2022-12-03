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
			uint32 m_TaskIndex;
			uint32 m_GroupId;
			uint32 m_GroupIndex;
			bool m_IsFirstTaskInGroup;
			bool m_IsLastTaskInGroup;
			void* m_SharedMemory;
		};

		struct Context {
			std::atomic<uint32> m_Counter { 0 };
		};

		Context* m_Context = nullptr;
		std::function<void(Task::Args)> m_TaskFunction;
		uint32 m_GroupId = 0;
		uint32 m_GroupTaskOffset = 0;
		uint32 m_GroupTaskEnd = 0;
		uint32 m_SharedMemorySize = 0;
	};

	class TaskManager {
	public:
		void Init();
		void Release();

		auto GetThreadCount() const->uint32;
		void Execute(Task::Context& ctx, const std::function<void(Task::Args)>& taskFunc);
		bool ExecuteOnWorkerThread(uint32 threadIndex, Task::Context& ctx, const std::function<void(Task::Args)>& taskFunction);
		void Dispatch(Task::Context& ctx, uint32 taskCount, uint32 groupSize, const std::function<void(Task::Args)>& taskFunc, size_t sharedMemorySize = 0);
		auto DispatchGroupCount(uint32 taskCount, uint32 groupSize) -> uint32;
		bool IsBusy(const Task::Context& ctx);
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