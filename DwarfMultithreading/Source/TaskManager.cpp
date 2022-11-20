#include <DwarfMultithreading/TaskManager.h>

#include <DwarvenCore/Assert.h>
#include <DwarvenCore/New.h>

#include <thread>
#include <condition_variable>
#include <sstream>
#include <algorithm>

#include <wtypes.h>

void df::TaskManager::Init() {
	m_IsWorking = true;

	// Retrieve the number of hardware threads in this system
	const auto numCores = std::thread::hardware_concurrency();

	// Calculate the actual number of worker threads we want (-1 main thread)
	m_WorkerThreadCount = max(1u, numCores - 1u);
	m_PerThreadTaskQueue = DFNew df::ThreadSafeRingBuffer<Task, 256>[m_WorkerThreadCount];

	for (uint32_t threadID = 0; threadID < m_WorkerThreadCount; ++threadID) {
		std::thread worker([this, threadID] {
			while (m_IsWorking) {
				if (!WorkThread(threadID) && !Work()) {
					// No job, put thread to sleep
					std::unique_lock<std::mutex> lock(m_WakeMutex);
					m_WakeCondition.wait(lock);
				}
			}
		});

#ifdef PLATFORM_WINDOWS
		// Do Windows-specific thread setup
		const HANDLE handle = (HANDLE)worker.native_handle();

		// Put each thread on to dedicated core
		const DWORD_PTR affinityMask = 1ull << threadID;
		const DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
		DFAssert(affinity_result > 0, "Couldn't set affinity mask!");

		// Increase thread priority
		const bool priority_result = SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
		DFAssert(priority_result != 0, "Couldn't set thread priority!");

		// Name the thread
		std::wstringstream wss;
		wss << "df::TaskManagerThread_" << threadID;
		const HRESULT hr = SetThreadDescription(handle, wss.str().c_str());
		DFAssert(SUCCEEDED(hr), "Couldn't set thread description!");
#endif // PLATFORM_WOINDOWS

		worker.detach();
	}
}

void df::TaskManager::Release() {
	m_IsWorking = false;
	m_WakeCondition.notify_all();

	DFDelete[] m_PerThreadTaskQueue;
}

auto df::TaskManager::GetThreadCount() const->uint32 {
	return m_WorkerThreadCount;
}

void df::TaskManager::Execute(df::Task::Context& ctx, const std::function<void(df::Task::Args)>& taskFunction) {
	// Context state is updated
	ctx.m_Counter.fetch_add(1);

	df::Task task;
	task.m_Context = &ctx;
	task.m_TaskFunction = taskFunction;
	task.m_GroupId = 0;
	task.m_GroupTaskOffset = 0;
	task.m_GroupTaskEnd = 1;
	task.m_SharedMemorySize = 0;

	// Try to push a new job until it is pushed successfully
	while (!m_TaskQueue.PushBack(task)) {
		m_WakeCondition.notify_all();
		Work();
	}

	// Wake any one thread that might be sleeping
	m_WakeCondition.notify_one();
}

bool df::TaskManager::ExecuteOnWorkerThread(uint32 threadIndex, Task::Context& ctx, const std::function<void(Task::Args)>& taskFunction) {
	DFAssert(threadIndex < m_WorkerThreadCount, "Invalid thread index!");

	// Context state is updated
	ctx.m_Counter.fetch_add(1);

	df::Task task;
	task.m_Context = &ctx;
	task.m_TaskFunction = taskFunction;
	task.m_GroupId = 0;
	task.m_GroupTaskOffset = 0;
	task.m_GroupTaskEnd = 1;
	task.m_SharedMemorySize = 0;

	// Try to push a new job until it is pushed successfully
	return m_PerThreadTaskQueue[threadIndex].PushBack(task);
}

void df::TaskManager::Dispatch(df::Task::Context& ctx, uint32 taskCount, uint32 groupSize, const std::function<void(df::Task::Args)>& taskFunction, size_t sharedmemory_size) {
	if (taskCount == 0 || groupSize == 0) {
		return;
	}

	const uint32_t groupCount = DispatchGroupCount(taskCount, groupSize);

	// Context state is updated
	ctx.m_Counter.fetch_add(groupCount);

	df::Task task;
	task.m_Context = &ctx;
	task.m_TaskFunction = taskFunction;
	task.m_SharedMemorySize = (uint32_t)sharedmemory_size;

	for (uint32_t groupID = 0; groupID < groupCount; ++groupID) {
		// For each group, generate one real job
		task.m_GroupId = groupID;
		task.m_GroupTaskOffset = groupID * groupSize;
		task.m_GroupTaskEnd = min(task.m_GroupTaskOffset + groupSize, taskCount);

		// Try to push a new job until it is pushed successfully
		while (!m_TaskQueue.PushBack(task)) {
			m_WakeCondition.notify_all();
			Work();
		}
	}

	// Wake any threads that might be sleeping
	m_WakeCondition.notify_all();
}

auto df::TaskManager::DispatchGroupCount(uint32 taskCount, uint32 groupSize) -> uint32 {
	// Calculate the amount of job groups to dispatch (overestimate, or "ceil")
	return (taskCount + groupSize - 1) / groupSize;
}

bool df::TaskManager::IsBusy(const df::Task::Context& ctx) {
	// Whenever the context label is greater than zero, it means that there is still work that needs to be done
	return ctx.m_Counter.load() > 0;
}

void df::TaskManager::Wait(const df::Task::Context& ctx) {
	// Wake any threads that might be sleeping:
	m_WakeCondition.notify_all();

	// Waiting will also put the current thread to good use by working on an other job if it can
	while (IsBusy(ctx)) {
		Work();
	}
}

bool df::TaskManager::Work() {
	df::Task job;
	if (m_TaskQueue.PopFront(job)) {
		df::Task::Args args;
		args.m_GroupId = job.m_GroupId;
		if (job.m_SharedMemorySize > 0) {
			args.m_SharedMemory = _malloca(job.m_SharedMemorySize);
		} else {
			args.m_SharedMemory = nullptr;
		}

		for (uint32_t i = job.m_GroupTaskOffset; i < job.m_GroupTaskEnd; ++i) {
			args.m_TaskIndex = i;
			args.m_GroupIndex = i - job.m_GroupTaskOffset;
			args.m_IsFirstTaskInGroup = (i == job.m_GroupTaskOffset);
			args.m_IsLastTaskInGroup = (i == job.m_GroupTaskEnd - 1);
			job.m_TaskFunction(args);
		}

		job.m_Context->m_Counter.fetch_sub(1);
		return true;
	}
	return false;
}

bool df::TaskManager::WorkThread(uint32 threadIndex) {
	df::Task job;
	if (m_PerThreadTaskQueue[threadIndex].PopFront(job)) {
		df::Task::Args args;
		args.m_GroupId = job.m_GroupId;
		if (job.m_SharedMemorySize > 0) {
			args.m_SharedMemory = _malloca(job.m_SharedMemorySize);
		} else {
			args.m_SharedMemory = nullptr;
		}

		for (uint32_t i = job.m_GroupTaskOffset; i < job.m_GroupTaskEnd; ++i) {
			args.m_TaskIndex = i;
			args.m_GroupIndex = i - job.m_GroupTaskOffset;
			args.m_IsFirstTaskInGroup = (i == job.m_GroupTaskOffset);
			args.m_IsLastTaskInGroup = (i == job.m_GroupTaskEnd - 1);
			job.m_TaskFunction(args);
		}

		job.m_Context->m_Counter.fetch_sub(1);
		return true;
	}
	return false;
}