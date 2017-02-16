/**
  @file Тест пула потоков
*/

#if defined(_MSC_VER)
#include "stdafx.h"
#endif
#include <time.h>
#include <stdio.h>
#include "mkcalcpool.hpp"

#define MILLISEC     1000           ///< количество миллисекунд в секунде
#define MICROSEC     1000000        ///< количество микросекунд в секунде
#define NANOSEC      1000000000     ///< количество наносекунд в секунде
#define NANOMILLISEC 1000000        ///< количество наносекунд в миллисекунде
#define NANOMICROSEC 1000        ///< количество наносекунд в микросекунде

unsigned long DiffTimeMs(timespec *time1, timespec *time0)
{
	///< Результат операции
	unsigned long result = 0;
	///< Разница в секундах и наносекундах
	unsigned long sec = 0, nsec = 0;

	if (time1 && time0)
	{
		sec = time1->tv_sec - time0->tv_sec;
		if (time1->tv_nsec > time0->tv_nsec)
			nsec = time1->tv_nsec - time0->tv_nsec;
		else
		{
			nsec = NANOSEC - time0->tv_nsec + time1->tv_nsec;
			sec -= 1;
		}
		result = sec * MILLISEC;
		result += (nsec / NANOMILLISEC);
	}

	return(result);
}

timespec *t_begin = NULL, *t_end = NULL;

///////////////////////////////////////////////////////////////////////////////
/// Расчетная функция потока
/// \param calc_func_data_t *data - указатель на структуру данных для расчета
/// \return нет
void *test_func(calc_func_data_t *data)
{
	if(data)
	{
		const int work_cycles = 20000;
		unsigned long id = (unsigned long)data->parameter;
		timespec start = { 0 }, end = { 0 };
        
#if defined(WIN32) || defined(WIN64)
		timespec_get(&start, TIME_UTC);
#else
		clock_gettime(CLOCK_MONOTONIC, &start);
#endif

		if(t_begin)
			t_begin[id - 1] = start;

		printf("test func %ld: mode = %d\n",
		 id, data->mode);

		for( int j = 0; j < work_cycles && NOT_BREAK(); j++ )
		{  // ... якобы вычисления
			for( unsigned long i = 0; i < (work_cycles * id) && NOT_BREAK(); i++ )
			{
			 	i = ( i + 1 ) - 1;
			 	TST_POINT();
			}
			TST_POINT();
		}
		SET_FINISHED(data->finished);
#if defined(WIN32) || defined(WIN64)
		timespec_get(&end, TIME_UTC);
#else
		clock_gettime(CLOCK_MONOTONIC, &end);
#endif
		printf("test func %ld finished (%ld ms)\n", id, DiffTimeMs(&end, &start));

		if (t_end)
		{
#if defined(WIN32) || defined(WIN64)
			timespec_get(&t_end[id - 1], TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &t_end[id - 1]);
#endif
		}
	}
	return(NULL);
}
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// Стандартная точка входа приложения
int main(int argc, char *argv[])
{
	int result = EOK;
	int threads = 1;
	int stack = 0;
	bool thr_key = false, stk_key = false;
	const char *UseMsg = "Usage: mkcalctest [-t <threads>] [-s <stacksize>]\n"
					 "-t <threads> - number of threads (1)\n"
					 "-s <stacksize> - thread stack size, in bytes (0, system stack size)\n";

	mkCalcPool *testpool = new mkCalcPool;
	if(!testpool)
	{
		printf("Error %d, exiting...\n", errno);
		return(errno);
	}
	
	for(int i = 1; i < argc; i++)
	{
		char *arg = argv[i];
		if(arg)
		{
			if(arg[0] == '-')
			{
				switch(arg[1])
				{
					case 't':
						if(arg[2])
						{
							arg += 2;
							threads = atoi(arg);
						}
						else
							thr_key = true;
					break;
					case 's':
						if(arg[2])
						{
							arg += 2;
							stack = atoi(arg);
						}
						else
							stk_key = true;
					break;
					case 'h':
						printf("%s", UseMsg);
						return(result);
					break;
				}
			}
			else if(thr_key)
			{
				threads = atoi(arg);
				thr_key = false;
			}
			else if(stk_key)
			{
				stack = atoi(arg);
				stk_key = false;
			}
		}
	}

	result = testpool->Init(threads, stack);
	if(result == EOK)
	{
		printf("Thread list ptr = %p\n", testpool->getItems());
		printf("Thread list stack size = %ld\n", testpool->getStackSize());

		t_begin = (timespec *)malloc(threads * sizeof(timespec));
		t_end = (timespec *)malloc(threads * sizeof(timespec));
		if(t_begin)
		{
			for(int i = 0; i < threads; i++)
				t_begin[i] = {0};
		}
		if(t_end)
		{
			for(int i = 0; i < threads; i++)
				t_end[i] = {0};
		}

		printf("Run %d threads\n", threads);
		for(unsigned long i = 0; i < (unsigned long)threads; i++)
		{
			result = testpool->Inject(test_func, (void *)(i + 1));
			if(result != EOK)
			{
				printf("Inject error %d\n", result);
				break;
			}
		}
		if(result == EOK)
		{
			printf("Threads count = %ld\n", testpool->getCount());
			printf("Last thread position = %ld\n", testpool->getLast());
			
			printf("Test mode %d (wait for one)\n", CRM_WAIT_ONE);
			timespec start = { 0 }, end = { 0 };
#if defined(WIN32) || defined(WIN64)
			timespec_get(&start, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &start);
#endif
			result = testpool->Start(CRM_WAIT_ONE);
			if(result != EOK)
			{
				printf("Pool start error %d\n", result);
				return(result);
			}
			printf("Test mode %d threads start time %ld ms\n", CRM_WAIT_ONE, DiffTimeMs(testpool->get_pool_start_end(), testpool->get_pool_start_begin()));
#if defined(WIN32) || defined(WIN64)
			timespec_get(&end, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &end);
#endif
			printf("Test mode %d finished (%ld ms)\n", CRM_WAIT_ONE, DiffTimeMs(&end, &start));
			if(t_begin)
			{
				for(int i = 0; i < threads; i++)
				{
					printf("thread %d started after %ld ms\n", i+1, DiffTimeMs(&t_begin[i], &start));
					t_begin[i] = {0};
				}
			}
			if(t_end)
			{
				for(int i = 0; i < threads; i++)
				{
					if(t_end[i].tv_sec > 0 || t_end[i].tv_nsec > 0)
						printf("thread %d stopped %ld ms ago\n", i+1, DiffTimeMs(&end, &t_end[i]));
					t_end[i] = {0};
				}
			}

			printf("Test mode %d (wait for all)\n", CRM_WAIT_ALL);
#if defined(WIN32) || defined(WIN64)
			timespec_get(&start, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &start);
#endif
			result = testpool->Start(CRM_WAIT_ALL);
			if(result != EOK)
			{
				printf("Pool start error %d\n", result);
				return(result);
			}
			printf("Test mode %d threads start time %ld ms\n", CRM_WAIT_ALL, DiffTimeMs(testpool->get_pool_start_end(), testpool->get_pool_start_begin()));
#if defined(WIN32) || defined(WIN64)
			timespec_get(&end, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &end);
#endif
			printf("Test mode %d finished (%ld ms)\n", CRM_WAIT_ALL, DiffTimeMs(&end, &start));
			if (t_begin)
			{
				for (int i = 0; i < threads; i++)
				{
					printf("thread %d started after %ld ms\n", i + 1, DiffTimeMs(&t_begin[i], &start));
					t_begin[i] = { 0 };
				}
			}
			if (t_end)
			{
				for (int i = 0; i < threads; i++)
				{
					if (t_end[i].tv_sec > 0 || t_end[i].tv_nsec > 0)
						printf("thread %d stopped %ld ms ago\n", i + 1, DiffTimeMs(&end, &t_end[i]));
					t_end[i] = { 0 };
				}
			}

			printf("Test mode %d  (run sequentaly)\n", CRM_SEQENCE);
#if defined(WIN32) || defined(WIN64)
			timespec_get(&start, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &start);
#endif
			result = testpool->Start(CRM_SEQENCE);
			if(result != EOK)
			{
				printf("Pool start error %d\n", result);
				return(result);
			}
#if defined(WIN32) || defined(WIN64)
			timespec_get(&end, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &end);
#endif
			printf("Test mode %d finished (%ld ms)\n", CRM_SEQENCE, DiffTimeMs(&end, &start));
			if (t_begin)
			{
				for (int i = 0; i < threads; i++)
				{
					printf("thread %d started after %ld ms\n", i + 1, DiffTimeMs(&t_begin[i], &start));
					t_begin[i] = { 0 };
				}
			}
			if (t_end)
			{
				for (int i = 0; i < threads; i++)
				{
					if (t_end[i].tv_sec > 0 || t_end[i].tv_nsec > 0)
						printf("thread %d stopped %ld ms ago\n", i + 1, DiffTimeMs(&end, &t_end[i]));
					t_end[i] = { 0 };
				}
			}
		}
		free(t_begin);
		free(t_end);
		t_begin = NULL;
		t_end = NULL;
	}
	else
		printf("Initialization error %d\n", result);
	delete testpool;
	return(result);
}
//////////////////////////////////////////////////////////////////////////////

