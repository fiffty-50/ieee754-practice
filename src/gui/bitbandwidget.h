#ifndef BITBANDWIDGET_H
#define BITBANDWIDGET_H

#include "src/bitband.h"
#include <QWidget>
#include <QAbstractButton>

namespace Ui {
class BitBandWidget;
}

class BitBandWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BitBandWidget(QWidget *parent = nullptr);
    ~BitBandWidget();

private:
    Ui::BitBandWidget *ui;
    void connectSignals();
    void clearLabels();
    enum class InputError { NotRecognized, OutOfRangeBB, OutOfRangeBBA };
    bool checkUserInput();
    void badUserInput(InputError error_code);

    enum Direction{FromBBA, FromBitPos};
    Direction m_direction = Direction::FromBBA;
    BitBand m_bitBand = BitBand(0);
    int m_step = 0;

private slots:
    void buttonGroupClicked(QAbstractButton *button);
    void difficultyChanged(int index);
    void startButtonClicked();
    void nextStepRequested();
    void resultRequested();
};

#endif // BITBANDWIDGET_H
