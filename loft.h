#ifndef LOFT_H
#define LOFT_H
#include"qobject.h"
class loft:public QObject
{
    Q_OBJECT
public:
	loft(int Unit) :unit(Unit) {};
    int floor = 1;
	int unit;
	bool clicked[20] = { 0 };
    //return true and set clicked[floor-1] to false if given floor is clicked inside the loft
    bool stopOrNot(int floor);
	bool noInnerClick(int low = 0,int high = 20);
	//if the loft is on the way to pick someone
	bool picking = false;
	int target;
private slots:
    void new_click(int floor);
};

#endif // LOFT_H
