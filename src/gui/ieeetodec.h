#ifndef IEEETODEC_H
#define IEEETODEC_H

#include <QWidget>
#include "src/number.h"

namespace Ui {
class ieeetodec;
}

class IeeeToDec : public QWidget
{
    Q_OBJECT

public:
    explicit IeeeToDec(QWidget *parent = nullptr);
    ~IeeeToDec();

private:
    Ui::ieeetodec *ui;

    Number m_number;

    void connectSignals();
    void clearLabels();

    //steps
    int m_step = 0;
    void hexToBinary();
    void determineBias();
    void extractMantissa();
    void deNormMantissa();
    void convertToDecimal();
    void applySign();

    const static inline QString POS = QStringLiteral("positive");
    const static inline QString NEG = QStringLiteral("negative");

private slots:
    void on_generateExample();
    void on_nextStepRequested();
    void on_resultRequested();
};

#endif // IEEETODEC_H
