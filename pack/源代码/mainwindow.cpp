#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"QVBOXLayout"
#include<vector>
#include"dispatcher.h"
#include"LabelButton.h"
#include"lift.h"
#include"LiftThread.h"
#include<iostream>
#include"qprogressbar.h"
#include"qpalette.h"
#include"qframe.h"
#include"qlcdnumber.h"

std::vector<std::vector<LabelButton*>> dispatcher::LBs;
std::vector<double> dispatcher::lift_x;
std::vector<double> dispatcher::lift_y;
std::vector<LiftThread*> dispatcher::lift_threads_;
std::vector<QProgressBar*> dispatcher::bars_;
std::vector<OpenButton*> dispatcher::opens_;
std::vector<QLCDNumber*> dispatcher::indicators_;
std::vector<QLabel*> dispatcher::arrows_[2];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setFixedSize(1400, 800);
    QPushButton* u_example = ui->up;
    QPushButton* d_example = ui->down;
	QPushButton* o_example = ui->open;
	QPushButton* c_example = ui->close;
	QPushButton* a_example = ui->alert;
    dispatcher* disp = dispatcher::getInstance();
    dispatcher::LBs.resize(5);
	dispatcher::lift_x.resize(5);
	dispatcher::lift_y.resize(20);
	dispatcher::lift_threads_.resize(5);
	dispatcher::bars_.resize(5);
	dispatcher::opens_.resize(5);
	dispatcher::indicators_.resize(5);
	dispatcher::arrows_[0].resize(5);
	dispatcher::arrows_[1].resize(5);
    for(int i=0;i!=5;++i)
    {
        dispatcher::LBs[i].resize(20);
	}

	QFrame* panel = new QFrame();
	panel->setGeometry(QRect(0, 0, 1400, 800));
	panel->setFrameShape(QFrame::Shape::Panel);
	panel->setFrameShadow(QFrame::Shadow::Raised);
	panel->setLineWidth(4);
	for (int unit = 1; unit != 6; ++unit)
	{
		const double x_base = double(panel->geometry().width()) / 5.0 * (unit - 1);
		//lift
		lift* lft = new lift(unit);
		LiftThread* lt = new LiftThread(lft, this);
		dispatcher::lift_threads_[unit - 1] = lt;
		//
		auto frame1 = new QFrame(panel);
		frame1->setGeometry(x_base + 10, 95, 90, panel->geometry().height() - 105);
		frame1->setLineWidth(5);
		frame1->setFrameShape(QFrame::Shape::Panel);
		frame1->setFrameShadow(QFrame::Shadow::Raised);
		//

		for (int floor = 1; floor != 21; ++floor)
		{
			const float y_base = panel->geometry().height() - (double)(panel->geometry().height() - 100) / 20.0 * floor;
			//out buttons
			auto up = new QPushButton(u_example->icon(), "",panel);
			up->setGeometry(x_base + 110, y_base, 20, 20);
			up->setIconSize(QSize(20, 20));
			up->setFlat(true);
			up->setCheckable(true);
			auto down = new QPushButton(d_example->icon(), "",panel);
			down->setGeometry(x_base + 130.0, y_base, 20, 20);
			down->setIconSize(QSize(20, 20));
			down->setFlat(true);
			down->setCheckable(true);
			auto in = new QPushButton(QString(std::string("f").append(std::to_string(floor)).c_str()),panel);
			in->setGeometry(x_base + 60, y_base, 30, 20);
			in->setCheckable(true);
			auto out_up = new OutButton(up, true, floor, unit);
			auto out_down = new OutButton(down, false, floor, unit);
			auto in_btn = new InButton(in, floor, unit, lft);

			dispatcher::LBs[unit - 1][floor - 1] = new LabelButton(out_up, out_down, in_btn);
			connect(out_up, SIGNAL(new_request(bool, int, int)), disp, SLOT(new_request(bool, int, int)));
			connect(out_down, SIGNAL(new_request(bool, int, int)), disp, SLOT(new_request(bool, int, int)));
			connect(out_up, SIGNAL(cancel_request(bool, int, int)), disp, SLOT(cancel_request(bool, int, int)));
			connect(out_down, SIGNAL(cancel_request(bool, int, int)), disp, SLOT(cancel_request(bool, int, int)));
			//lift_position
			dispatcher::lift_x[unit - 1] = x_base + 150;
			dispatcher::lift_y[floor - 1] = y_base;
		}
		//label_lift
		QProgressBar* label_lift = new QProgressBar(panel);
		label_lift->setTextVisible(false);
		label_lift->setValue(100);
		dispatcher::bars_[unit - 1] = label_lift;
		//panel->addWidget(label_lift);
		//open and close bar
		auto open = new QPushButton(o_example->icon(), "",panel);
		open->setGeometry(x_base + 20, panel->geometry().height() / 2 - 20, 30, 30);
		open->setIconSize(QSize(30, 30));
		open->setFlat(true);
		open->setCheckable(true);
		auto openBtn = new OpenButton(open, unit, lft);
		disp->opens_[unit - 1] = openBtn;
		auto close = new QPushButton(c_example->icon(), "",panel);
		close->setGeometry(x_base + 20, panel->geometry().height() / 2 + 20, 30, 30);
		close->setIconSize(QSize(30, 30));
		close->setFlat(true);
		close->setCheckable(false);
		//alert
		auto alert = new QPushButton(a_example->icon(), "", panel);
		alert->setGeometry(x_base + 20, panel->geometry().height() / 2 + 100, 30, 30);
		alert->setIconSize(QSize(30, 30));
		alert->setFlat(true);
		connect(alert, SIGNAL(clicked()), lft, SLOT(alerted()));
		//indicator
		QLCDNumber* indi = new QLCDNumber(panel);
		indi->setLineWidth(5);
		indi->setGeometry(x_base + 160, 15, 80, 60);
		indi->setDigitCount(2);
		//indi->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
		dispatcher::indicators_[unit - 1] = indi;
		//arrows
		QPixmap ua = QPixmap("up_arrow.png").scaled(QSize(30, 60));
		QLabel* up_arrow = new QLabel(panel);
		up_arrow->setGeometry(x_base + 120, 15, 30, 60);
		up_arrow->setPixmap(ua);
		up_arrow->setVisible(false);
		QPixmap da = QPixmap("down_arrow.png").scaled(QSize(30, 60));
		QLabel* down_arrow = new QLabel(panel);
		down_arrow->setGeometry(x_base + 120, 15, 30, 60);
		down_arrow->setPixmap(da);
		down_arrow->setVisible(false);
		dispatcher::arrows_[0][unit - 1] = up_arrow;
		dispatcher::arrows_[1][unit - 1] = down_arrow;
	}
	//QPixmap pixmap = QPixmap("back0.jpg").scaled(this->size());
	//QPalette palette(this->palette());
	//palette.setBrush(QPalette::Background, pixmap);
	//this->setPalette(palette);
    delete(ui->centralWidget);
	this->layout()->addWidget(panel);
}

MainWindow::~MainWindow()
{
	for (int i = 0; i != dispatcher::lift_threads_.size(); ++i)
	{
		dispatcher::lift_threads_[i]->stop_ = true;
	}
	for (int i = 0; i != dispatcher::lift_threads_.size(); ++i)
	{
		dispatcher::lift_threads_[i]->wait();
	}

    delete ui;
}


