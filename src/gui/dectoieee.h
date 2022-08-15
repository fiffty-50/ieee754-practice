#ifndef DECTOIEEE_H
#define DECTOIEEE_H

#include <QWidget>
#include <QBitArray>
#include <QRandomGenerator>
#include "src/number.h"

namespace Ui {
class DecToIeee;
}

/*!
 * \brief Convert a decimal base 10 number to IEEE-754 single precision floating point number
 */
class DecToIeee : public QWidget
{
    Q_OBJECT

public:
    explicit DecToIeee(QWidget *parent = nullptr);
    ~DecToIeee();

private slots:
    void on_generateExample();
    void on_nextStepRequested();
    void on_resultRequested();

private:
    Ui::DecToIeee *ui;
    void connectSignals();
    void clearLabels();

    // Steps
    Number m_number;
    int m_step;
    void decimalToBinary();
    void determineBias();
    void padMantissa();
    void determineExponent();
    void determineSignBit();
    void concatenate();
    void stringToNibbles();
    void stringToHex();

};

#endif // DECTOIEEE_H
