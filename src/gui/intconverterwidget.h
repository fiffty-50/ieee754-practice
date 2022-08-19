#ifndef INTCONVERTERWIDGET_H
#define INTCONVERTERWIDGET_H

#include "qlabel.h"
#include <QWidget>

namespace Ui {
class IntConverterWidget;
}

class IntConverterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntConverterWidget(QWidget *parent = nullptr);
    ~IntConverterWidget();

private:
    enum InputFormat {Binary, Decimal, Hexadecimal};
    QStringList m_conversions;
    Ui::IntConverterWidget *ui;
    QMap<int, QLabel*> m_LABEL_MAP;



    void conversionRequested();

    QStringList convertNumber(InputFormat format);

    void clearDisplayLabels();
};

#endif // INTCONVERTERWIDGET_H


