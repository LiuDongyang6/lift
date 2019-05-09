#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QVBoxLayout"
#include<vector>
#include"dispatcher.h"
#include"LabelButton.h"
#include"loft.h"
#include"LoftThread.h"
#include<iostream>
#include"qprogressbar.h"

std::vector<std::vector<LabelButton*>> dispatcher::LBs;
std::vector<double> dispatcher::loft_x;
std::vector<double> dispatcher::loft_y;
std::vector<LoftThread*> dispatcher::loft_threads_;
std::vector<QProgressBar*> dispatcher::bars_;
std::vector<OpenButton*> dispatcher::opens_;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton* u_example = ui->up;
    QPushButton* d_example = ui->down;
	QPushButton* o_example = ui->open;
	QPushButton* c_example = ui->close;
	layout
	contents.
    dispatcher* disp = dispatcher::getInstance();
    dispatcher::LBs.resize(5);
	dispatcher::loft_x.resize(5);
	dispatcher::loft_y.resize(20);
	dispatcher::loft_threads_.resize(5);
	dispatcher::bars_.resize(5);
	dispatcher::opens_.resize(5);
    for(int i=0;i!=5;++i)
    {
        dispatcher::LBs[i].resize(20);
    }
	for (int unit = 1; unit != 6; ++unit)
	{
		//loft
		loft* lft = new loft(unit);
		LoftThread* lt = new LoftThread(lft, this);
		dispatcher::loft_threads_[unit - 1] = lt;
		for (int floor = 1; floor != 21; ++floor)
		{
			//out buttons
			auto up = new QPushButton(u_example->icon(), "");
			up->setGeometry(1800.0 / 6.0 * unit, 800.0 / 21.0 * (21.0 - floor), 20, 20);
			up->setIconSize(QSize(20, 20));
			up->setFlat(true);
			up->setCheckable(true);
			auto down = new QPushButton(d_example->icon(), "");
			down->setGeometry(1800.0 / 6.0 * unit + 20.0, 800.0 / 21.0 * (21.0 - floor), 20, 20);
			down->setIconSize(QSize(20, 20));
			down->setFlat(true);
			down->setCheckable(true);
			auto in = new QPushButton(QString(std::string("f").append(std::to_string(floor)).c_str()));
			in->setGeometry(1800.0 / 6.0 * unit - 30.0, 800.0 / 21.0 * (21.0 - floor), 30, 20);
			in->setCheckable(true);
			this->layout()->addWidget(up);
			this->layout()->addWidget(down);
			this->layout()->addWidget(in);

			auto out_up = new OutButton(up, true, floor, unit);
			auto out_down = new OutButton(down, false, floor, unit);
			auto in_btn = new InButton(in, floor, unit, lft);
			dispatcher::LBs[unit - 1][floor - 1] = new LabelButton(out_up, out_down, in_btn);
			connect(out_up, SIGNAL(new_request(bool, int, int)), disp, SLOT(new_request(bool, int, int)));
			connect(out_down, SIGNAL(new_request(bool, int, int)), disp, SLOT(new_request(bool, int, int)));
			connect(out_up, SIGNAL(cancel_request(bool, int, int)), disp, SLOT(cancel_request(bool, int, int)));
			connect(out_down, SIGNAL(cancel_request(bool, int, int)), disp, SLOT(cancel_request(bool, int, int)));
			//loft_position
			dispatcher::loft_x[unit - 1] = 1800.0 / 6.0 * unit + 40;
			dispatcher::loft_y[floor - 1] = 800.0 / 21.0 * (21.0 - floor);
		}
		//label_loft
		QProgressBar* label_loft = new QProgressBar();
		label_loft->setTextVisible(false);
		label_loft->setValue(100);
		label_loft->setGeometry(1800.0 / 6.0 * unit + 40.0, 800.0 / 21.0 * (21.0 - 1.0), 150, 20);
		dispatcher::bars_[unit - 1] = label_loft;
		this->layout()->addWidget(label_loft);
		//open and close bar
		auto open = new QPushButton(o_example->icon(), "");
		open->setGeometry(1800.0 / 6.0 * unit, 880, 40, 40);
		open->setIconSize(QSize(40, 40));
		open->setFlat(true);
		open->setCheckable(true);
		auto openBtn = new OpenButton(open, unit, lft);
		disp->opens_[unit - 1] = openBtn;
		auto close = new QPushButton(c_example->icon(), "");
		close->setGeometry(1800.0 / 6.0 * unit + 40, 880, 40, 40);
		close->setIconSize(QSize(40, 40));
		close->setFlat(true);
		close->setCheckable(true);
		this->layout()->addWidget(open);
		this->layout()->addWidget(close);
	}
    delete(ui->centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


