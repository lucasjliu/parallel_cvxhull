//
//  UnitTest.h
//
//	@brief: a unit test class for a certain type of functor
//
//  @interface: addCase(...): add testcase and ground truth
//              run(): run testcases and print result, time cost
//
//  by Jiahuan.Liu
//	jiahaun.liu@outlook.com
//
//  03/04/2017
//
#ifndef _UNITTEST_H
#define _UNITTEST_H

#include <chrono>
#include <functional>
#include <vector>
#include <assert.h>

#include "Exception.h"
#include "Logger.h"

class Timer
{
	using clock_t = std::chrono::high_resolution_clock;
	using time_t= std::chrono::time_point<clock_t>;
public:
	Timer();
	void start();
	int stop();
	void pause();
	void resume();
private:
	time_t _beg;
	time_t _pause;
	bool _started;
	bool _paused;
};

template<typename Func, typename Ret>
class UnitTest
{
	using ret_type = Ret;
	using Test = std::function<ret_type()>;
public:
	struct Exception: public ::Exception
	{
		Exception(const std::string& strerr) : ::Exception(strerr) {}
		Exception(const std::string& strerr, int ierr)
		: ::Exception(strerr, ierr) {}
		~Exception() noexcept {}
	};
public:
	explicit UnitTest(Func&& func);
	explicit UnitTest(Func func);

	void run();

	template <typename ...Args>
	void addCase(ret_type&& truth, Args&&... args);

	template <typename ...Args>
	void addCase(const ret_type& truth, const Args&... args);
private:
	std::vector<Test> _tests;
	std::vector<ret_type> _truths;
	Timer _timer;
	Func _func;
};

class UnitTestFactory
{
public:
	/*template<typename Ret, typename Func>
	static UnitTest<Func, Ret> create(Func& func)
	{
		return UnitTest<Func, Ret>(std::forward<Func>(func));
	}*/

	template<typename Ret, typename Func>
	static UnitTest<Func, Ret> create(Func func)
	{
		return UnitTest<Func, Ret>(func);
	}
};

template<typename Func, typename Ret>
UnitTest<Func, Ret>::UnitTest(Func&& func) :_func(std::forward<Func>(func)) 
{}
template<typename Func, typename Ret>
UnitTest<Func, Ret>::UnitTest(Func func) :_func(func) 
{}

template<typename Func, typename Ret>
void UnitTest<Func, Ret>::run()
{
	assert(_tests.size() == _truths.size());
	for (int i = 0; i < _tests.size(); ++i)
	{
		_timer.start();
		ret_type result = _tests[i]();
		int cost = _timer.stop();
		if (!(result == _truths[i]) && _truths[i] != ret_type())
		{
			char err_msg[40] = {0};
			sprintf(err_msg, "Case# %d Fails: %d ms", i, cost);
			throw Exception(err_msg);
		}
		else
		{
			printf("Case# %d OK: %d ms\n", i, cost);
			fflush(stdout);
		}
	}
}

template<typename Func, typename Ret>
template <typename ...Args>
void UnitTest<Func, Ret>::addCase(ret_type&& truth, Args&&... args)
{
	Test test = std::bind(_func, _timer, std::forward<Args>(args)...);
	_tests.push_back(std::move(test));
	_truths.push_back(std::forward<ret_type>(truth));
}

template<typename Func, typename Ret>
template <typename ...Args>
void UnitTest<Func, Ret>::addCase(const ret_type& truth, const Args&... args)
{
	Test test = std::bind(_func, _timer, args...);
	_tests.push_back(std::move(test));
	_truths.push_back(truth);
}

#endif