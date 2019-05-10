#include "dispatcher.h"
#include"LabelButton.h"
#include"LiftThread.h"
#include"qprogressbar.h"
#include"qlcdnumber.h"
bool dispatcher::inited = false;
dispatcher* dispatcher::instance_ = NULL;
dispatcher* dispatcher::getInstance()
{
    if(!inited)
    {
        inited = true;
        instance_ = new dispatcher();
    }
    return instance_;
}

void dispatcher::new_request(bool isUp,int floor,int unit)
{
	for (int i = 0; i != 5; ++i)
	{
		if (isUp)
			LBs[i][floor - 1]->up_->button_->setChecked(true);
		else
			LBs[i][floor - 1]->down_->button_->setChecked(true);
	}
	idle_mutex.lock();
	int assigned = NearestIdle(floor);
	if (assigned != -1)
	{
		lift_threads_[assigned - 1]->mutex.lock();
		idle[assigned - 1] = false;
		lift_threads_[assigned - 1]->lift_->target = floor;
		lift_threads_[assigned - 1]->lift_->picking = true;
		lift_threads_[assigned - 1]->status = isUp ? 0 : 1;
		lift_threads_[assigned - 1]->mutex.unlock();
	}
	else
	{
		mutex[isUp ? 0 : 1].lock();
		waiting[isUp ? 0 : 1].push_back(floor);
		mutex[isUp ? 0 : 1].unlock();
	}
	idle_mutex.unlock();
}
int dispatcher::NearestIdle(int floor)
{
	int dis = 1000;
	int nearest = -1;
	for (int i = 0; i != 5; ++i)
	{
		if (!idle[i])
			continue;
		if (std::abs(lift_threads_[i]->lift_->floor - floor) < dis)
		{
			nearest = i + 1;
			dis = std::abs(lift_threads_[i]->lift_->floor - floor);
		}
	}
	return nearest;
}
void dispatcher::cancel_request(bool isUp,int floor,int unit)
{
	for (int i = 0; i != 5; ++i)
	{
		if (isUp)
			LBs[i][floor - 1]->up_->button_->setChecked(false);
		else
			LBs[i][floor - 1]->down_->button_->setChecked(false);
	}
	mutex[isUp ? 0 : 1].lock();
	for (auto it = waiting[isUp ? 0 : 1].begin(); it != waiting[isUp ? 0 : 1].end();)
	{
		if (*it == floor)
			it = waiting[isUp ? 0 : 1].erase(it);
		else
			++it;
	}
	mutex[isUp ? 0 : 1].unlock();
}
void dispatcher::init()
{
	for (int i = 0; i != 5; ++i)
	{
		idle[i] = true;
		connect((QObject*)lift_threads_[i], SIGNAL(updatePos(int, int)), this, SLOT(updatePos(int, int)));
		connect((QObject*)lift_threads_[i], SIGNAL(openDoor(int, bool)), this, SLOT(openDoor(int, bool)));
		connect((QObject*)lift_threads_[i], SIGNAL(cancelInner(int, int)), this, SLOT(cancelInner(int, int)));
		connect((QObject*)lift_threads_[i], SIGNAL(barUpdate(int, int)), this, SLOT(barUpdate(int, int)));
		lift_threads_[i]->start();
	}
}
void dispatcher::updatePos(int unit, int floor)
{
	bars_[unit - 1]->setGeometry(lift_x[unit - 1], lift_y[floor - 1], 120.0, 20.0);
	indicators_[unit - 1]->display(floor);
}
void dispatcher::openDoor(int floor, bool isUp)
{
	cancel_request(isUp, floor, 0);
}
void dispatcher::cancelInner(int unit, int floor)
{
	LBs[unit - 1][floor - 1]->in_->button_->setChecked(false);
}
void dispatcher::barUpdate(int unit, int value)
{
	bars_[unit - 1]->setValue(value);
}