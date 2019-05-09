#ifndef DISPATCHER_H
#define DISPATCHER_H
#include<vector>
#include"QObject.h"
#include"qmutex.h"
#include<list>
class LabelButton;
class QLabel;
class LoftThread;
class QProgressBar;
class OpenButton;

class dispatcher: public QObject
{
    Q_OBJECT
public:
	static dispatcher* getInstance();
	static std::vector<std::vector<LabelButton*>> LBs;
	static std::vector<double> loft_x;
	static std::vector<double> loft_y;
	static std::vector<LoftThread*> loft_threads_;
	static std::vector<QProgressBar*> bars_;
	static std::vector<OpenButton*> opens_;
	std::list<int> waiting[2];
	QMutex mutex[2];
	QMutex idle_mutex;
	bool idle[5];
	void init();
private slots:
    void new_request(bool isUp,int floor,int unit);
    void cancel_request(bool isUp,int floor,int unit);
	void updatePos(int unit, int floor);
	void openDoor(int floor, bool isUp);
	void cancelInner(int unit, int floor);
	void barUpdate(int unit, int value);
private:
    dispatcher(){}
    static bool inited;
    int NearestIdle(int floor);
    static dispatcher* instance_;
};

#endif // DISPATCHER_H
