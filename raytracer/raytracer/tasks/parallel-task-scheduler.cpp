#include "tasks/parallel-task-scheduler.h"
#include <thread>

using namespace tasks;

namespace
{
    class ParallelTaskScheduler : public tasks::schedulers::_private_::TaskSchedulerImplementation
    {
    public:
		//Useful: http://thispointer.com/c11-start-thread-by-member-function-with-arguments/
        void perform(std::vector<std::shared_ptr<Task>> tasks) const
        {
			std::vector<std::thread> threads;


            for (auto task : tasks)
            {
				threads.push_back(std::thread(&Task::perform, task));
            }

			for (auto& t : threads) {
				// Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
				t.join();
			}



        }
    };
}

TaskScheduler tasks::schedulers::parallel()
{
    return TaskScheduler(std::make_shared<ParallelTaskScheduler>());
}
