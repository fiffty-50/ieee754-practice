#ifndef FLOATCONVERTERWIDGET_H
#define FLOATCONVERTERWIDGET_H

#include <QWidget>
#include "qabstractbutton.h"
#include "src/floatrepresentation.h"

namespace Ui {
class FloatConverterWidget;
}

class FloatConverterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FloatConverterWidget(QWidget *parent = nullptr);
    ~FloatConverterWidget();

private:
    Ui::FloatConverterWidget *ui;
    void connectSignals();
    void clearLabels();

    enum Direction {DecToHex, HexToDec};
    Direction m_direction = DecToHex;
    FloatRepresentation m_number;
    int m_step = 0;

private slots:
    void startConversion();
    void nextStepRequested();
    void resultRequested();
    void buttonGroupClicked(QAbstractButton *button);
    void difficultyChanged(int index);
};

#endif // FLOATCONVERTERWIDGET_H
