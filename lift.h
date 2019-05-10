#ifndef LOFT_H
#define LOFT_H
#include"qobject.h"
class lift:public QObject
{
    Q_OBJECT
public:
	lift(int Unit) :unit(Unit) {};
    int floor = 1;
	int unit;
	bool clicked[20] = { 0 };
    //return true and set clicked[floor-1] to false if given floor is clicked inside the lift
    bool stopOrNot(int floor);
	bool noInnerClick(int low = 0,int high = 20);
	//if the lift is on the way to pick someone
	bool picking = false;
	int target;
private slots:
    void new_click(int floor);
};

#endif // LOFT_H
