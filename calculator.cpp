#include "calculator.h"
#include "ui_calculator.h"

double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];

    for (int i = 0; i < 10; i++) {
        QString buttName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(buttName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Minus, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Plus, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->Equal, SIGNAL(released()), this, SLOT(EqualButtonPressed()));

    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));

    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearPressed()));

    connect(ui->Correct, SIGNAL(released()), this, SLOT(CorrectPressed()));

    connect(ui->Dot, SIGNAL(released()), this, SLOT(DotPressed()));
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();
    QString displayVal = ui->Display->text();
    if ((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0)) {
        ui->Display->setText(butVal);
    } else {
        QString newVal = displayVal + butVal;
        double dblNewVal = newVal.toDouble();
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed() {
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    if (QString::compare(butVal, "/", Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    } else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0){
        multTrigger = true;
    } else if (QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    } else {
        subTrigger = true;
    }

    ui->Display->setText("");
}

void Calculator::EqualButtonPressed() {
    double solution = 0.0;

    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if (addTrigger) {
        solution = calcVal + dblDisplayVal;
    } else if(subTrigger) {
        solution = calcVal - dblDisplayVal;
    } else if (multTrigger) {
        solution = calcVal * dblDisplayVal;
    } else if (divTrigger) {
        solution = calcVal / dblDisplayVal;
    }

    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeNumberSign() {
    QString displayVal = ui->Display->text();

    QRegExp reg("[-]?[0-9.]*");

    if (reg.exactMatch(displayVal)) {
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;

        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::ClearPressed() {
    ui->Display->setText("0.0");
    calcVal = 0.0;
}

void Calculator::CorrectPressed() {
    QString text = ui->Display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0.0";
    }
    ui->Display->setText(text);
}

void Calculator::DotPressed() {
    if (!ui->Display->text().contains('.')) {
        ui->Display->setText(ui->Display->text() + tr("."));
    }
}
