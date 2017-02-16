/**
  @file Реализация интерфейс для запуска расчетных потоков
  
*/

#include "mkcalcpool.hpp"

/// Признак остановки всех потоков
calc_atomic_flag_t calc_stop(false);

#ifdef _MK_PARALLEL_
	#if defined(WIN32) || defined(WIN64)
		HANDLE run_event = NULL;
	#else
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
		/// Барьер для запуска POSIX-потоков
		pthread_barrier_t *run_barrier = NULL;
		/// Барьер для остановки POSIX-потоков
		pthread_barrier_t *fin_barrier = NULL;
		#endif
	#endif
#endif


/// Установка атомарного флага
/// \param calc_atomic_flag_t *ptr - указатель на атомарную переменную
/// \param bool value - значение (true или false)
void SET_ATOM_FLAG(calc_atomic_flag_t *ptr, bool value)
{
#ifdef _MK_PARALLEL_
	if(ptr)
		ptr->store(value);
#else
	if(ptr)
		*ptr = value;
#endif
}

/// Проверка атомарного флага
/// \param calc_atomic_flag_t *ptr - указатель на атомарную переменную
/// \return значение (true или false)
bool GET_ATOM_FLAG(calc_atomic_flag_t *ptr)
{
#ifdef _MK_PARALLEL_
	if(ptr)
		return(ptr->load());
#else
	if(ptr)
		return(*ptr);
#endif
	return(false);
}

/// Проверка флага остановки в условии цикла расчета и немедленный выход из потока
/// \param нет
/// \return true, если цикл может быть продолжен
/// \return false, если цикл должен быть прерван
bool NOT_BREAK(void)
{
	bool is_stop = GET_ATOM_FLAG(&calc_stop);
#ifdef _MK_PARALLEL_
	if(is_stop)
	#if defined(WIN32) || defined(WIN64)
//		_endthread();
		ExitThread(0);
	#else
		pthread_exit(NULL);
	#endif
#endif
	return(!is_stop);
}

/// Конструктор класса calc_item_t
/// \param calc_func_t f - указатель на расчетную функцию
/// \param void *p - параметр функции
/// \param calc_atomic_flag_t *flg - указатель на флаг завершения расчета
calc_item_t::calc_item_t(calc_func_t f, void *p, calc_atomic_flag_t *flg)
{
	init(f, p, flg);
}

/// Деструктор класса calc_item_t
calc_item_t::~calc_item_t()
{
	clear();
}

/// Инициализация объекта класса calc_item_t
/// \param calc_func_t f - указатель на расчетную функцию
/// \param void *p - параметр функции
/// \param calc_atomic_flag_t *flg - указатель на флаг завершения расчета
/// \return нет
void calc_item_t::init(calc_func_t f, void *p, calc_atomic_flag_t *flg)
{
	tid = (calc_thread_t)-1;
	func = f;
	func_data = {0};
	if(f != NULL)
	{
		func_data.parameter = p;
		func_data.mode = CRM_INIT;
		func_data.finished = flg;
	};
}

/// Закрытие объекта класса calc_item_t
/// \param нет
/// \return нет
void calc_item_t::clear(void)
{
	tid = (calc_thread_t)-1;
	func = NULL;
	func_data = {0};
}

/// Установить режим запуска
/// \param calc_run_mode_t m - режим запуска
/// \return нет
void calc_item_t::set_mode(calc_run_mode_t m)
{
	if(func != NULL)
		func_data.mode = m;
}

/// Посмотреть текущий режим запуска
/// \param нет
/// \return режим запуска
calc_run_mode_t calc_item_t::get_mode(void)
{
	return(func_data.mode);
}

/// Установить указатель на флаг завершения
/// \param calc_atomic_flag_t *flg - режим запуска
/// \return нет
void calc_item_t::set_flag_ptr(calc_atomic_flag_t *flg)
{
	if(func != NULL)
		func_data.finished = flg;
}

/// Посмотреть текущий указатель на флаг завершения
/// \param нет
/// \return режим запуска
calc_atomic_flag_t *calc_item_t::get_flag_ptr(void)
{
	return(func_data.finished);
}

/// Установить идентификатор потока
/// \param calc_thread_t id - идентификатор потока
/// \return нет
void calc_item_t::set_tid(calc_thread_t id)
{
	tid = id;
}

/// Получить текущий идентификатор потока
/// \param нет
/// \return идентификатор потока
calc_thread_t calc_item_t::get_tid(void)
{
	return(tid);
}

/// Получить указатель на функцию потока
/// \param нет
/// \return указатель на функцию потока
calc_func_t calc_item_t::get_func(void)
{
	return(func);
}

/// Получить указатель на данные для функции потока
/// \param нет
/// \return указатель на данные для функции потока
calc_func_data_t *calc_item_t::get_func_data(void)
{
	return(&func_data);
}

unsigned int mkCalcPool::in_use = 0;

/// Конструктор класса mkCalcPool по умолчанию
mkCalcPool::mkCalcPool()
{
	count = 0;
	items = NULL;
	stacksize = 0;
	last = 0;
	handles = NULL;
#ifdef _MK_PARALLEL_
	pool_start_begin = {0};
	pool_start_end = {0};
#endif
	in_use++;
}

/// Конструктор класса mkCalcPool
/// \param size_t cnt - число потоков
/// \param size_t stk - объем стека на поток
mkCalcPool::mkCalcPool(size_t cnt, size_t stk)
{
	count = 0;
	stacksize = 0;
	last = 0;
	items = NULL;
	handles = NULL;
#ifdef _MK_PARALLEL_
	pool_start_begin = {0};
	pool_start_end = {0};
#endif
	in_use++;
	Init(cnt, stk);
}

/// Деструктор класса mkCalcPool
mkCalcPool::~mkCalcPool()
{
	Free();
	in_use--;
}
	
/// Инициализация объекта
/// \param size_t cnt - число потоков
/// \param size_t stk - объем стека на поток (по умолчанию 0 - размер стека устанавливается системой)
/// \return EOK в cлучае успеха
/// \return ENOMEM при ошибке выделения памяти
/// \return EINVAL при неверных параметрах
/// \return EBUSY при попытке работы со вторым экземпляром объекта
int mkCalcPool::Init(size_t cnt, size_t stk)
{
	int result = EINVAL;

	if(in_use > 1)
	{
		result = EBUSY;
	}
	else
	{
		count = cnt;
		stacksize = stk;
		last = 0;
		items = (calc_item_t *)malloc(sizeof(calc_item_t) * count);
		if(items)
		{
			for(size_t i = 0; i < count; i++)
			{
				items[i].init();
			}
			result = EOK;
		}
		else
			result = ENOMEM;
		handles = (calc_thread_t *)malloc(sizeof(calc_thread_t) * count);
		if(handles)
		{
			for(size_t i = 0; i < count; i++)
			{
				handles[i] = (calc_thread_t)NULL;
			}
		}
		else
			result = ENOMEM;
	}
	return(result);
}

/// Добавление функции потока в указанное место
/// \param void *(*f)(calc_func_data_t*) - указатель на расчетную функцию
/// \param void *p - параметр функции
/// \param int ind - порядковый номер в списке потоков (-1 добавляет ункцию в первую свободную ячейку списка)
/// \return EOK в cлучае успеха
/// \return ENOENT, если список потоков не инициализирован
/// \return ESRCH, если порядковый номер указан неверно
/// \return EAGAIN, если свободное место в списке исчерпано
/// \return EBUSY при попытке работы со вторым экземпляром объекта
int mkCalcPool::Inject(void *(*f)(calc_func_data_t*), void *p, int ind)
{
	int result = ENOENT;
	
	if(in_use > 1)
	{
		result = EBUSY;
	}
	else
	{
		if(items)
		{
			result = ESRCH;
			if(ind == -1)
			{
				if(last < count)
				{
					items[last].init((calc_func_t)f, p);
					last++;
					result = EOK;
				}
				else
					result = EAGAIN;
			}
			else
			{
				if(ind >= 0 && (size_t)ind < count)
				{
					items[ind].init((calc_func_t)f, p);
					if((size_t)ind >= last)
						last = ind + 1;
					result = EOK;
				}
			}
		}
	}
	return(result);
}

/// Признак запуска всех потоков
calc_atomic_flag_t calc_start(false);

#ifdef _MK_PARALLEL_
/// Функция-обертка, выполняемая в контексте потока. Обеспечивает одновременный запуск расчетных функций по взведению
/// флага старта.
/// В последовательном режиме не задействуетсяю Если не определен флаг _MK_PARALLEL_, исключается из компиляции
/// \param void *thread_data - указатель на данные для расчетной функции
#if defined(WIN32) || defined(WIN64)
//void thread_func_wrapper(void *thread_data)
DWORD WINAPI thread_func_wrapper(LPVOID thread_data)
#else
void *thread_func_wrapper(void *thread_data)
#endif
{
	calc_item_t *func_item = (calc_item_t *)thread_data;

	if(func_item && func_item->get_func())
	{

#if !defined(WIN32) && !defined(WIN64)
		sigset_t set, oset;

		sigfillset(&set);
		pthread_sigmask(SIG_BLOCK, &set, &oset);
		sigemptyset(&set);
		sigaddset(&set, SIGSEGV);
		pthread_sigmask( SIG_UNBLOCK, &set, &oset );
#endif

		calc_func_t func = func_item->get_func();
		calc_func_data_t *func_data = func_item->get_func_data();
#if defined(WIN32) || defined(WIN64)
		if(run_event)
			WaitForSingleObject(run_event, INFINITE);
		if(func)
			func(func_data);
#else

	#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
		if(run_barrier != NULL)
		{
			pthread_barrier_wait(run_barrier);
			if(func)
				func(func_data);
		}
		else
		{
			while(true)
			{
				if(GET_ATOM_FLAG(&calc_start))
				{
					if(func)
					func(func_data);
					break;
				}
				TST_POINT();
				sched_yield();
			}
			SET_FINISHED(func_data->finished);
		}
	#else
		while(true)
		{
			if(GET_ATOM_FLAG(&calc_start))
			{
				if(func)
					func(func_data);
				break;
			}
			TST_POINT();
			sched_yield();
		}
		SET_FINISHED(func_data->finished);
	#endif
#endif
	#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
		if(fin_barrier != NULL)
			pthread_barrier_wait(fin_barrier);
	#endif
	}
#if defined(WIN32) || defined(WIN64)
//	_endthread();
	ExitThread(0);
#else
	pthread_exit(NULL);
#endif
}

/// Запуск потока
/// \param calc_item_t *item - указатель на описание потока
/// \return EOK в cлучае успеха
/// \return EINVAL, если список указатель на описание потока не инициализирован
/// \return признак ошибки запуска потока, возвращаемый функцией системы
int mkCalcPool::run(calc_item_t *item)
{
	int result = EINVAL;

	if(item)
	{
#if defined(WIN32) || defined(WIN64)
//		calc_thread_t Tid = (calc_thread_t)_beginthread(thread_func_wrapper, stacksize, item);
		calc_thread_t Tid = CreateThread(NULL, stacksize, thread_func_wrapper, item, CREATE_SUSPENDED, NULL);
		if(Tid == (calc_thread_t)NULL)
			result = GetLastError();//errno;
		else
		{
			item->set_tid(Tid);
			result = EOK;
		}
#else
		pthread_attr_t attr, *attr_p = NULL;
		calc_thread_t Tid = (calc_thread_t)-1;
		result = EOK;
		
		if(stacksize > 0)
		{
			pthread_attr_init(&attr);
			result = pthread_attr_setstacksize(&attr, stacksize);
			attr_p = &attr;
		}
		if(result == EOK)
		{
			result = pthread_create(&Tid, attr_p, thread_func_wrapper, item);
			if(result == EOK)
				item->set_tid(Tid);
		}
#endif		
	}
	
	return(result);
}

/// Ожидание в течение заданного числа миллисекунд
/// \param int Ms - число миллисекунд
void mkCalcPool::wait(int Ms)
{
#if defined(WIN32) || defined(WIN64)
	Sleep(Ms);
#else
	struct timespec req = {0};
	req.tv_sec = Ms / 1000;
	req.tv_nsec=(Ms + (Ms % 1000))*1000000;
	nanosleep(&req, NULL);
#endif
}

#endif

/// Запуск расчета
/// \param calc_run_mode_t mode режим расчета
/// Если директива _MK_PARALLEL_ не определена, режим расчета принудително будет установлен в последовательный
/// (CRM_SEQENCE)
/// \return EOK в cлучае успеха
/// \return ENOENT, если список потоков не инициализирован
/// \return EBUSY при попытке работы со вторым экземпляром объекта
/// \return признак ошибки запуска потока, возвращаемый функцией pthread_create (если определена директива _MK_PARALLEL_)
int mkCalcPool::Start(calc_run_mode_t mode)
{
	int result = ENOENT;

	if(in_use > 1)
	{
		result = EBUSY;
	}
	else
	{
		SET_ATOM_FLAG(&calc_start, false);
		SET_ATOM_FLAG(&calc_stop, false);
		
		if(items)
		{
#ifdef _MK_PARALLEL_
			calc_run_mode_t current_mode = mode;
	#if defined(WIN32) || defined(WIN64)
			run_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	#else	
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
			if(current_mode != CRM_SEQENCE)
			{
				run_barrier = new pthread_barrier_t;
				fin_barrier = new pthread_barrier_t;
				pthread_barrier_init(run_barrier, NULL, count+1);
				if(current_mode == CRM_WAIT_ONE)
					pthread_barrier_init(fin_barrier, NULL, 2);
				else if(current_mode == CRM_WAIT_ALL)
					pthread_barrier_init(fin_barrier, NULL, count+1);
			}
		#endif
	#endif
#else
			calc_run_mode_t current_mode = CRM_SEQENCE;
#endif
			calc_func_t func = NULL;
			calc_func_data_t *func_data = NULL;
			calc_atomic_flag_t calc_finished(false);
			result = EOK;
#ifdef _MK_PARALLEL_
			pool_start_begin = {0};
			pool_start_end = {0};
#if defined(WIN32) || defined(WIN64)
			timespec_get(&pool_start_begin, TIME_UTC);
#else
			clock_gettime(CLOCK_MONOTONIC, &pool_start_begin);
#endif
#endif
			for(size_t i = 0; i < count; i++)
			{
				items[i].set_mode(current_mode);
				items[i].set_flag_ptr(&calc_finished);
				items[i].set_tid((calc_thread_t)-1);
				handles[i] = (calc_thread_t)NULL;
				func = items[i].get_func();
				func_data = items[i].get_func_data();
				if(current_mode == CRM_SEQENCE)
				{
					if(func)
						func(func_data);
				}
#ifdef _MK_PARALLEL_
				else
				{
					result = run(&items[i]);
					if(result != EOK)
						break;
					handles[i] = items[i].get_tid();
				}
#endif
			}
#ifdef _MK_PARALLEL_
			if(current_mode != CRM_SEQENCE)
			{
				if(result == EOK)
				{
	#if defined(WIN32) || defined(WIN64)
					for(size_t i = 0; i < count; i++)
					{
						if(items[i].get_tid() != (calc_thread_t)-1)
							ResumeThread(items[i].get_tid());
					}
					if(run_event)
						SetEvent(run_event);
					timespec_get(&pool_start_end, TIME_UTC);
	#else
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
					if(run_barrier != NULL)
						pthread_barrier_wait(run_barrier);
					else
						SET_ATOM_FLAG(&calc_start, true);
					clock_gettime(CLOCK_MONOTONIC, &pool_start_end);
		#else
					SET_ATOM_FLAG(&calc_start, true);
					clock_gettime(CLOCK_MONOTONIC, &pool_start_end);
		#endif
	#endif
					if(current_mode == CRM_WAIT_ONE)
					{
	#if defined(WIN32) || defined(WIN64)
						WaitForMultipleObjects(count, handles, FALSE, INFINITE);
	#else
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
						if(fin_barrier != NULL)
							pthread_barrier_wait(fin_barrier);
						else
						{
							while(!GET_ATOM_FLAG(&calc_finished))
							{
								wait(100);
							}
						}
		#else
						while(!GET_ATOM_FLAG(&calc_finished))
						{
							wait(100);
						}
		#endif
	#endif
						SET_ATOM_FLAG(&calc_stop, true);
					}
					else if(current_mode == CRM_WAIT_ALL)
					{
	#if defined(WIN32) || defined(WIN64)
						WaitForMultipleObjects(count, handles, TRUE, INFINITE);
	#else
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
						if(fin_barrier != NULL)
							pthread_barrier_wait(fin_barrier);
		#endif
	#endif
					}
				}
				else
					SET_ATOM_FLAG(&calc_stop, true);
	#if defined(WIN32) || defined(WIN64)
				for(size_t i = 0; i < count; i++)
				{
					if(items[i].get_tid() != (calc_thread_t)-1)
					{
						CloseHandle(items[i].get_tid());
					}
					items[i].set_tid((calc_thread_t)-1);
				}
				CloseHandle(run_event);
				run_event = NULL;
	#else
				for(size_t i = 0; i < count; i++)
				{
					if(items[i].get_tid() != (calc_thread_t)-1)
					{
						pthread_join(items[i].get_tid(), NULL);
						items[i].set_tid((calc_thread_t)-1);
					}
				}
		#if defined (_POSIX_BARRIERS) || defined (PTHREAD_BARRIER_SERIAL_THREAD)
				if(fin_barrier != NULL)
				{
					pthread_barrier_destroy(fin_barrier);
					delete fin_barrier;
					fin_barrier = NULL;
				}
				if(run_barrier != NULL)
				{
					pthread_barrier_destroy(run_barrier);
					delete run_barrier;
					run_barrier = NULL;
				}
		#endif
	#endif
			}
#endif
		}
	}
	return(result);
}
	
/// Очистка ресурсов и закрытие объекта
/// \param нет
/// \return нет
void mkCalcPool::Free(void)
{
	if(in_use > 1)
	{
		return;
	}
	if(handles)
	{
		free(handles);
		handles = NULL;
	}
	if(items)
	{
		for(size_t i = 0; i < count; i++)
		{
			items[i].clear();
		}
		free(items);
		items = NULL;
	}
	count = 0;
	stacksize = 0;
	last = 0;
	pool_start_begin = {0};
	pool_start_end = {0};
}
