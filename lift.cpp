#include "lift.h"
#include"dispatcher.h"
#include"LabelButton.h"
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
        return true;
    }
    else
        return false;
}
void lift::alerted()
{
	for (int i = 0; i != dispatcher::LBs[unit - 1].size(); ++i)
	{
		dispatcher::LBs[unit - 1][i]->in_->button_->setChecked(true);
		clicked[i] = true;
	}
}
