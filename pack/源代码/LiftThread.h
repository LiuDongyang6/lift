#ifndef LOFT_THREAD_H
#define LOFT_THREAD_H
#include"qthread.h"
#include"lift.h"
#include"qmutex.h"
class LiftThread :public QThread
{
	Q_OBJECT
public:
	LiftThread(lift* lft, QObject *parent = NULL) :
		lift_(lft), QThread(parent) {}
	~LiftThread() { delete(lift_); }
	void run();
	lift* lift_;
	//1 down;0 up;-1 idle;2 break after open & up;3 break after open & down
	int status = -1;
	QMutex mutex;
	//开关按钮有效按下时置为true
	//开关按钮松开后为false
	volatile int stayOpen = 0;
	bool mayOpen() { return mayOpen_; }
	bool stop_ = false;
signals:
	void openDoor(int floor, bool isUp);
	void updatePos(int unit, int floor);
	void cancelInner(int unit, int floor);
	void barUpdate(int unit, int value);
	//-1still,0up,1down
	void updateArrow(int unit, int arrow);
	
private:
	//emitOut:是否向总控类发送取消外部请求的signal
	void DoorProcess(bool emitOut = true);
	volatile bool mayOpen_ = true;
};
#endif // !LOFT_THREAD_H
