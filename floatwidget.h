#ifndef FLOATWIDGET_H
#define FLOATWIDGET_H

#include <QWidget>
#include <QBitArray>
#include <QRandomGenerator>

namespace Ui {
class FloatWidget;
}

class FloatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FloatWidget(QWidget *parent = nullptr);
    ~FloatWidget();

private slots:
    void on_generateExample();
    void on_nextStepRequested();
    void on_resultRequested();

private:
    Ui::FloatWidget *ui;
    void connectSignals();
    void clearLabels();

    enum class Difficulty {easy, moderate, hard};
    /*!
     * \brief Represents a decimal number in base 2, e.g. 1011.011
     */
    struct DecimalNumberBaseTwo {
        bool sign = false;
        QBitArray integer = QBitArray();
        QBitArray decimals = QBitArray();

        DecimalNumberBaseTwo() = default;
        DecimalNumberBaseTwo(bool sign_, const QBitArray &integer_, const QBitArray &decimals_)
            : sign(sign_), integer(integer_), decimals(decimals_) {}

    };
    DecimalNumberBaseTwo m_decimalNumberBaseTwo;

    DecimalNumberBaseTwo generateDecimalNumber(Difficulty difficulty);

    double toDouble(const DecimalNumberBaseTwo &dec_number);

    /*!
     * \brief return a randomised bit
     */
    inline bool randomBit() {return QRandomGenerator::global()->bounded(2);}
    /*!
     * \brief returns a randomised QBitArray of specified size
     */
    QBitArray randomBitArray(int size);

    // Steps
    int m_step;
    int m_shift;
    void decimalToBinary();
    QString m_binary_string;
    QString m_biased_exponent_string;
    QString m_mantissa;
    QString m_Z32;
    void determineBias();
    void padMantissa();
    void determineExponent();
    void determineSignBit();
    void concatenate();
    void toHex();



};

#endif // FLOATWIDGET_H
