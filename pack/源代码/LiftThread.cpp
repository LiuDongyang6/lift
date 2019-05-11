#include"LiftThread.h"
#include"dispatcher.h"
#include"qlabel.h"
#include"qprogressbar.h"
void LiftThread::run()
{
	auto instance_ = dispatcher::getInstance();
	while (!stop_)
	{
		mutex.lock();
		if (!instance_->idle[lift_->unit - 1])
		{
			mayOpen_ = false;
			//不闲
			if (lift_->picking)
			{
				if (lift_->floor == lift_->target)
				{
					emit updateArrow(lift_->unit, status);
					//door open
					DoorProcess();
					//等待直到门关
					lift_->picking = false;
					status += 2;
				}
				else if (lift_->stopOrNot(lift_->floor))
				{
					DoorProcess(false);
				}
				else if (lift_->floor > lift_->target)
				{
					emit updateArrow(lift_->unit, 1);
					lift_->floor--;
				}
				else if (lift_->floor < lift_->target)
				{
					emit updateArrow(lift_->unit, 0);
					lift_->floor++;
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
						if (*it == lift_->floor)
						{
							b = true;//可以开门拉外边的人
							break;
						}
					}
					instance_->mutex[status].unlock();
					if (lift_->stopOrNot(lift_->floor))
					{
						b = true;
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
						lift_->floor += status == 0 ? 1 : -1;
					}
					break;
				}
				case 2:
				case 3:
				{
					int low = lift_->floor - 1;
					int high = 20;
					if (status == 3)
					{
						high = lift_->floor;
						low = 0;
					}
					if (lift_->noInnerClick(low, high))
					{
						if (lift_->noInnerClick(0, 20))
						{
							if (!instance_->waiting[status % 2].empty())
							{
								status = status % 2;
								lift_->target = instance_->waiting[status].front();
								lift_->picking = true;
							}
							else if (!instance_->waiting[1 - status % 2].empty())
							{
								status = 1 - status % 2;
								lift_->target = instance_->waiting[status].front();
								lift_->picking = true;
							}
							else
							{
								status = -1;
								instance_->idle_mutex.lock();
								instance_->idle[lift_->unit - 1] = true;
								instance_->idle_mutex.unlock();
								mayOpen_ = true;
							}
						}
						else//if (lift_->noInnerClick(0, 20))
						{
							status = 1 - status % 2;
						}
					}
					else//if (lift_->noInnerClick(low, high))
					{
						status -= 2;
					}
					break;
				}
				default:
					break;
				}
				emit updateArrow(lift_->unit, status == -1 ? -1 : status % 2);
			}
		}
		//电梯空闲
		//电梯空闲状态下mayOpen永远为true
		else
		{
			if (stayOpen == true)
			{
				instance_->idle[lift_->unit - 1] = false;
				DoorProcess();
				instance_->idle[lift_->unit - 1] = true;
			}
			int dis = 1000;
			int nearest = -1;
			for (int i = 0; i != 20; ++i)
			{
				if (lift_->clicked[i] && std::abs(i + 1 - lift_->floor) < dis)
				{
					nearest = i;
					dis = std::abs(i + 1 - lift_->floor);
				}
			}
			if (nearest != -1)
			{
				instance_->idle[lift_->unit - 1] = false;
				if (nearest + 1 >= lift_->floor)
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
		//instance_->lift_labels_[lift_->unit-1]->setGeometry(instance_->lift_x[lift_->unit - 1], instance_->lift_y[lift_->floor - 1], 30.0, 20.0);
		emit updatePos(lift_->unit, lift_->floor);
		msleep(300);
	}
}

void LiftThread::DoorProcess(bool emitOut)
{
	emit cancelInner(lift_->unit, lift_->floor);
	lift_->clicked[lift_->floor] = false;
	if (emitOut)
	{
		emit openDoor(lift_->floor, status % 2 == 0);
	}
	mayOpen_ = true;
	int value = dispatcher::bars_[lift_->unit - 1]->value();
opendoor:
	while (value >= 0)
	{
		emit barUpdate(lift_->unit, --value);
		msleep(10);
	}
	sleep(2);
	while (value<100)
	{
		if (stayOpen)
			goto opendoor;
		else
			emit barUpdate(lift_->unit, ++value);
		msleep(10);
	}
	for (int i = 0; i != 20; ++i)
	{
		if (stayOpen)
			goto opendoor;
		msleep(10);
	}
	//再来一次，避免开门中又有人按本楼层
	emit cancelInner(lift_->unit, lift_->floor);
	lift_->clicked[lift_->floor] = false;
}