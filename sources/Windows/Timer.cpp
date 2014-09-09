#include "stdafx.h"

#pragma comment(lib,"winmm")

namespace Common{
	void c_timer::start()
	{
		SMART_ASSERT(_timer != NULL).Fatal();
		SMART_ASSERT(_tid == 0).Fatal();

		_timer->update_timer(0,0,0);
		_time_value[0] = _time_value[1] = _time_value[2] = 0;

		_tid = ::timeSetEvent(1000, 0, _timer_proc, DWORD_PTR(this), TIME_PERIODIC | TIME_CALLBACK_FUNCTION);
		SMART_ASSERT(_tid != 0).Warning();
		if (_tid == 0 && _notifier)
			_notifier->msgerr("����ϵͳ��ʱ��ʧ��");
	}

	void c_timer::stop(bool bsetzero)
	{
		SMART_ASSERT(_tid != 0).Warning();
		::timeKillEvent(_tid);
		_tid = 0;
		if (bsetzero)
			_timer->update_timer(0,0,0);
	}

	void c_timer::set_notifier(i_notifier* not)
	{
		_notifier = not;
	}

	void c_timer::set_timer(i_timer* tim)
	{
		_timer = tim;
	}

	void __stdcall c_timer::_timer_proc(UINT, UINT, DWORD_PTR dwUser, DWORD_PTR, DWORD_PTR)
	{
		return reinterpret_cast<c_timer*>(dwUser)->timer_proc();
	}

	void c_timer::timer_proc()
	{
		if (++_time_value[0] == 60){
			_time_value[0] = 0;
			if (++_time_value[1] == 60){
				_time_value[1] = 0;
				if (++_time_value[2] == 24){
					_time_value[2] = 0;
				}
			}
		}
		if (_timer) _timer->update_timer(
			_time_value[2], _time_value[1], _time_value[0]);
	}

	c_timer::~c_timer()
	{
		if (_tid != 0){
			stop();
		}
	}
}