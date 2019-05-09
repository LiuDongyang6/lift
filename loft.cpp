#include "loft.h"

void loft::new_click(int floor)
{
    clicked[floor-1] = true;
}

bool loft::noInnerClick(int low,int high)
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
bool loft::stopOrNot(int floor)
{
    if(clicked[floor])
    {
        clicked[floor] = false;
        return true;
    }
    else
        return false;
}
