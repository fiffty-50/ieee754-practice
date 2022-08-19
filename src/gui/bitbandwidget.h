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
    QButtonGroup *ui_directionButtonGroup  = nullptr;
    QButtonGroup *ui_addressingButtonGroup = nullptr;
    void connectSignals();
    void clearLabels();
    enum class InputError { NotRecognized, OutOfRange };
    bool checkUserInput();
    void badUserInput(InputError error_code);

    enum Direction{FromBBA, FromBitPos};
    Direction m_direction = Direction::FromBBA;
    BitBand m_bitBand;
    int m_step = 0;
    BitBand::AddressingType m_addressing = BitBand::AddressingType::Byte;

private slots:
    void directionChanged(QAbstractButton *button);
    void addressingTypeChanged(QAbstractButton *button);
    void difficultyChanged(int index);
    void startButtonClicked();
    void nextStepRequested();
    void resultRequested();
};

#endif // BITBANDWIDGET_H
