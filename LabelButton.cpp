#include "LabelButton.h"
#include<iostream>
#include"lift.h"
#include"dispatcher.h"
#include"LiftThread.h"

InButton::InButton(QPushButton* button,int floor,int unit, lift* lift):
    button_(button),floor_(floor),unit_(unit),lift_(lift)
{
	connect(button_, SIGNAL(clicked(bool)), this, SLOT(btn_clicked(bool)));
}

void InButton::btn_clicked(bool checked)
{
	//here is a problem
	//will this func still be called if the button was unchecked?
	if (checked)
	{
		lift_->clicked[floor_] = true;
	}
	else
	{
		this->button_->setChecked(true);
	}
}

OutButton::OutButton(QPushButton* button, bool isUp, int floor, int unit) :
	button_(button), isUp_(isUp), floor_(floor), unit_(unit)
{
	connect(button_, SIGNAL(pressed()), this, SLOT(btn_pressed()));
	connect(button_, SIGNAL(released()), this, SLOT(btn_released()));
}
void OutButton::btn_pressed()
{
	//here is a problem
	//will this func still be called if the button was unchecked?
	auto instance_ = dispatcher::getInstance();
	if (instance_->lift_threads_[unit_ - 1]->lift_->floor == floor_ &&
		instance_->lift_threads_[unit_ - 1]->mayOpen() &&
		instance_->lift_threads_[unit_ - 1]->status == !isUp_)
	{
		//debug
		if (instance_->lift_threads_[unit_ - 1]->status != 0 && instance_->lift_threads_[unit_ - 1]->status != 1)
			std::cout << "label_button_btn_clicked_assert" << std::endl;
		asOpenDoor_ = true;
		instance_->lift_threads_[unit_ - 1]->stayOpen++;
	}
	else
	{
		asOpenDoor_ = false;
		if (!button_->isChecked())
		{
			emit new_request(isUp_, floor_, unit_);
		}
		this->button_->setChecked(true);
		button_->setDown(false);
	}
}

void OutButton::btn_released()
{
	if (asOpenDoor_)
	{
		asOpenDoor_ = false;
		auto instance_ = dispatcher::getInstance();
		//debug
		if (instance_->lift_threads_[unit_ - 1]->status != 0 && instance_->lift_threads_[unit_ - 1]->status != 1)
			std::cout << "label_button_btn_released_assert" << std::endl;
		if (instance_->lift_threads_[unit_ - 1]->lift_->floor == floor_ &&
			instance_->lift_threads_[unit_ - 1]->stayOpen&&
			instance_->lift_threads_[unit_ - 1]->status == !isUp_)
		{
			this->button_->setChecked(false);
			instance_->lift_threads_[unit_ - 1]->stayOpen--;
		}
	}
}

OpenButton::OpenButton(QPushButton* button, int unit, lift* lift):
	button_(button), unit_(unit), lift_(lift)
{
	connect(button_, SIGNAL(pressed()), this, SLOT(btn_pressed()));
	connect(button_, SIGNAL(released()), this, SLOT(btn_released()));
}

void OpenButton::btn_pressed()
{
	auto instance_ = dispatcher::getInstance();
	if (instance_->lift_threads_[unit_ - 1]->mayOpen())
	{
		effective_ = true;
		instance_->lift_threads_[unit_ - 1]->stayOpen++;
	}
	else
	{
		effective_ = false;
		button_->setChecked(false);
	}
}

void OpenButton::btn_released()
{
	if (effective_)
	{
		effective_ = false;
		auto instance_ = dispatcher::getInstance();
		//debug
		if (instance_->lift_threads_[unit_ - 1]->status != 0 && instance_->lift_threads_[unit_ - 1]->status != 1)
			std::cout << "label_button_btn_released_assert" << std::endl;
		//equivalent out btn 
		instance_->lift_threads_[unit_ - 1]->stayOpen--;
	}
	button_->setChecked(false);
}
