#ifndef LOFT_THREAD_H
#define LOFT_THREAD_H
#include"qthread.h"
#include"loft.h"
#include"qmutex.h"
class LoftThread :public QThread
{
	Q_OBJECT
public:
	LoftThread(loft* lft, QObject *parent = NULL) :
		loft_(lft), QThread(parent) {}
	~LoftThread() { delete(loft_); }
	void run();
	loft* loft_;
	//1 down;0 up;-1 idle;2 break after open & up;3 break after open & down
	int status = -1;
	QMutex mutex;
	//���ذ�ť��Ч����ʱ��Ϊtrue
	//���ذ�ť�ɿ���Ϊfalse
	volatile int stayOpen = 0;
	bool mayOpen() { return mayOpen_; }
signals:
	void openDoor(int floor, bool isUp);
	void updatePos(int unit, int floor);
	void cancelInner(int unit, int floor);
	void barUpdate(int unit, int value);
	
private:
	void DoorProcess();
	volatile bool mayOpen_ = true;
};
#endif // !LOFT_THREAD_H
