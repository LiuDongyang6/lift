#ifndef LABELBUTTON_H
#define LABELBUTTON_H
#include"QPushButton.h"
#include"QLabel.h"

class loft;

class OutButton:public QObject
{
    Q_OBJECT
public:
    OutButton(QPushButton* button,bool isUp,int floor,int unit);
    virtual ~OutButton(){delete(button_);}
    QPushButton* button_;
    bool isUp_;
    int floor_;
    int unit_;
signals:
    void new_request(bool isUp,int floor,int unit);
	void cancel_request(bool isUp, int floor, int unit);
private slots:
    void btn_pressed();
	void btn_released();
private:
	bool asOpenDoor_ = false;
};

class InButton :public QObject
{
	Q_OBJECT
public:
	InButton(QPushButton* button, int floor, int unit, loft* loft);
	virtual ~InButton() { delete(button_); }
	QPushButton* button_;
	int floor_;
	int unit_;
	loft* loft_;
signals:
	void new_request(int floor, int unit);
private slots:
	//if button has been effectively clicked(the first branch of btn_clicked)
	void btn_clicked(bool checked);
};

class OpenButton :public QObject
{
	Q_OBJECT
public:
	OpenButton(QPushButton* button, int unit, loft* loft);
	virtual ~OpenButton() { delete(button_); }
	QPushButton* button_;
	int unit_;
	loft* loft_;
private slots:
	void btn_pressed();
	void btn_released();
private:
	//if button has been effectively clicked(the first branch of btn_clicked)
	bool effective_ = false;
};

class LabelButton
{
public:
    LabelButton() = default;
    LabelButton(OutButton* up,OutButton* down,InButton* in):
        up_(up),down_(down),in_(in){}
    OutButton* up_;
    OutButton* down_ ;
    InButton* in_;
    ~LabelButton()
    {
        delete(up_);
        delete(down_);
        delete(in_);
    }
};

#endif // LABELBUTTON_H
