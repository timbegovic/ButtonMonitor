#ifndef GPIO_H
#define GPIO_H

#include <QObject>

class gpio : public QObject
{
    Q_OBJECT
public:
    explicit gpio(QObject *parent = nullptr); // constructor
    ~gpio(); // destructor for clean-up
    enum getMode : uint8_t;
    void set(int pin, bool value);
    void set(unsigned int pattern);

    bool get(int pin, getMode mode = getMode::raw);

    enum getMode : uint8_t
    {
        raw = 0,
        falling = 1,
        rising = 2
    };

signals:

public slots:

private:
    int m_handle;
    QList<bool> m_oldStates;
};

#endif // GPIO_H
