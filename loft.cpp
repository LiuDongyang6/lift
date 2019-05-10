#include "lift.h"

void lift::new_click(int floor)
{
    clicked[floor-1] = true;
}

bool lift::noInnerClick(int low,int high)
{
	bool b = true;
	for (int i = low; i != high; ++i)
	{
		if (clicked[i])
		{
			b = false;
			break;
		}
	}
	return b;
}
bool lift::stopOrNot(int floor)
{
    if(clicked[floor])
    {
        clicked[floor] = false;
        return true;
    }
    else
        return false;
}
