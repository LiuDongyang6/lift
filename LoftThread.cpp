#include"LoftThread.h"
#include"dispatcher.h"
#include"qlabel.h"
#include"qprogressbar.h"
void LoftThread::run()
{
	auto instance_ = dispatcher::getInstance();
	while (true)
	{
		mutex.lock();
		if (!instance_->idle[loft_->unit - 1])
		{
			mayOpen_ = false;
			//不闲
			if (loft_->picking)
			{
				if (loft_->floor == loft_->target)
				{
					//door open
					DoorProcess();
					//等待直到门关
					loft_->picking = false;
					status += 2;
				}
				else if (loft_->floor > loft_->target)
				{
					loft_->floor--;
				}
				else if (loft_->floor < loft_->target)
				{
					loft_->floor++;
				}
			}
			//unpicking & have inner clicked
			else
			{
				switch (status)
				{
				case 0:
				case 1:
				{
					mayOpen_ = false;
					//b为真则停
					bool b = false;
					instance_->mutex[status].lock();
					for (auto it = instance_->waiting[status].begin(); it != instance_->waiting[status].end(); ++it)
					{
						if (*it == loft_->floor)
						{
							b = true;//可以开门拉外边的人
							break;
						}
					}
					instance_->mutex[status].unlock();
					if (loft_->stopOrNot(loft_->floor))
					{
						b = true;
						emit cancelInner(loft_->unit, loft_->floor);
					}
					if (b)
					{
						//dooropen
						DoorProcess();
						//等待直到门关
						status += 2;
					}
					else
					{
						loft_->floor += status == 0 ? 1 : -1;
					}
					break;
				}
				case 2:
				case 3:
				{
					int low = loft_->floor - 1;
					int high = 20;
					if (status == 3)
					{
						high = loft_->floor;
						low = 0;
					}
					if (loft_->noInnerClick(low, high))
					{
						if (loft_->noInnerClick(0, 20))
						{
							if (!instance_->waiting[status % 2].empty())
							{
								status = status % 2;
								loft_->target = instance_->waiting[status].front();
								loft_->picking = true;
							}
							else if (!instance_->waiting[1 - status % 2].empty())
							{
								status = 1 - status % 2;
								loft_->target = instance_->waiting[status].front();
								loft_->picking = true;
							}
							else
							{
								status = -1;
								instance_->idle_mutex.lock();
								instance_->idle[loft_->unit - 1] = true;
								instance_->idle_mutex.unlock();
								mayOpen_ = true;
							}
						}
						else//if (loft_->noInnerClick(0, 20))
						{
							status = 1 - status % 2;
						}
					}
					else//if (loft_->noInnerClick(low, high))
					{
						status -= 2;
					}
					break;
				}
				default:
					break;
				}
			}
		}
		//电梯空闲
		//电梯空闲状态下mayOpen永远为true
		else
		{
			if (stayOpen == true)
			{
				instance_->idle[loft_->unit - 1] = false;
				DoorProcess();
				instance_->idle[loft_->unit - 1] = true;
			}
			int dis = 1000;
			int nearest = -1;
			for (int i = 0; i != 20; ++i)
			{
				if (loft_->clicked[i] && std::abs(i + 1 - loft_->floor) < dis)
				{
					nearest = i;
					dis = std::abs(i + 1 - loft_->floor);
				}
			}
			if (nearest != -1)
			{
				instance_->idle[loft_->unit - 1] = false;
				if (nearest + 1 >= loft_->floor)
				{
					status = 0;
				}
				else
				{
					status = 1;
				}
			}
		}
		mutex.unlock();
		//instance_->loft_labels_[loft_->unit-1]->setGeometry(instance_->loft_x[loft_->unit - 1], instance_->loft_y[loft_->floor - 1], 30.0, 20.0);
		emit updatePos(loft_->unit, loft_->floor);
		msleep(300);
	}
}

void LoftThread::DoorProcess()
{
	emit openDoor(loft_->floor, status % 2 == 0);
	mayOpen_ = true;
	int value = dispatcher::bars_[loft_->unit - 1]->value();
opendoor:
	while (value >= 0)
	{
		emit barUpdate(loft_->unit, --value);
		msleep(10);
	}
	sleep(2);
	while (value<100)
	{
		if (stayOpen)
			goto opendoor;
		else
			emit barUpdate(loft_->unit, ++value);
		msleep(10);
	}
	for (int i = 0; i != 20; ++i)
	{
		if (stayOpen)
			goto opendoor;
		msleep(10);
	}
}