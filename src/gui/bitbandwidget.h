#ifndef BITBANDWIDGET_H
#define BITBANDWIDGET_H

#include "qlabel.h"
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

    enum Direction{FromBBA, FromBitPos};
    Direction m_direction = Direction::FromBBA;
    BitBand m_bitBand = BitBand(0);
    int m_step = 0;

    const QMap<int, QPair<QLabel*, QLabel*>> *m_label_map;


private slots:
    void on_buttonGroupClicked(QAbstractButton *button);
    void on_generateExample();
    void on_nextStepRequested();
    void on_resultRequested();
};

#endif // BITBANDWIDGET_H
