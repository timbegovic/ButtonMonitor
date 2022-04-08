#include "config.h"
#include "widget.h"
#include <QGridLayout>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Widget s &QTimer::timeouttructure
    setWindowTitle("ButtonMonitor");
    resize(300, 100);
    QGridLayout* grid = new QGridLayout(this);
    setLayout(grid);

    // Labels for input pins / buttons
    int column = 0;
    for (auto pin : BUTTONS) {
        // pin numbers
        QLabel* label = new QLabel("GPIO " + QString::number(pin), this);
        label->setAlignment(Qt::AlignCenter);
        grid->addWidget(label, 0, column); // links oben (1. Zeile / 1. - 3. Spalte)

        // pin states
        QLabel* state = new QLabel("0", this);
        state->setAlignment(Qt::AlignCenter);
        grid->addWidget(state, 1, column++); // links oben (2. Zeile / 1. - 3. Spalte)
        m_input_display.push_back(state);
    }

    // initialize hardware
    m_gpio = new gpio();

    // Counter
    m_cntLabel = new QLabel(this);
    m_cntLabel->setAlignment(Qt::AlignCenter);
    grid->addWidget(m_cntLabel, 3, 1);
    updateCount();

    m_timer = new QTimer(this);
    // Connect QTimer (Signal) with GUI function (Slot) "update display"
    connect(m_timer, &QTimer::timeout, this, &Widget::updateButtonState);
    connect(m_timer, &QTimer::timeout, this, &Widget::updateCount);
    m_timer->start(T_UPDATE);
}

Widget::~Widget()
{

}

void Widget::updateButtonState()
{
    int n = 0;
    for (auto pin : BUTTONS){
        int state = !m_gpio->get(pin);
        m_input_display[n++]->setText(QString::number(state));
    }
}

void Widget::updateCount()
{
    if(m_gpio->get(BUTTONS[0], gpio::getMode::falling)) // getMode: raw | rising | falling
        m_cnt--;

    else if(m_gpio->get(BUTTONS[2], gpio::getMode::falling))
        m_cnt++;

    else if(m_gpio->get(BUTTONS[1], gpio::getMode::falling))
        m_cnt = 0;

    m_cntLabel->setText(QString("Counter: ") + QString::number(m_cnt));
}
