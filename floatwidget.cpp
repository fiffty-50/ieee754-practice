#include "floatwidget.h"
#include "ui_floatwidget.h"
#include <math.h>
#include <QMessageBox>
#include <QDebug>

FloatWidget::FloatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatWidget)
{
    ui->setupUi(this);
    connectSignals();
    clearLabels();
    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
}

FloatWidget::~FloatWidget()
{
    delete ui;
}

void FloatWidget::clearLabels()
{
    ui->binaryDisplayLabel->setText(QString());
    ui->biasDisplayLabel->setText(QString());
    ui->exponentDisplayLabel->setText(QString());
    ui->mantissaDisplayLabel->setText(QString());
    ui->biasDisplayLabel->setText(QString());
    ui->signBitDisplayLabel->setText(QString());
    ui->concatDisplayLabel->setText(QString());
    ui->hexDisplayLabel->setText(QString());
}

FloatWidget::DecimalNumberBaseTwo FloatWidget::generateDecimalNumber(Difficulty difficulty)
{
    QBitArray integer;
    QBitArray decimals;
    switch (difficulty) {
    case Difficulty::easy:
        integer = randomBitArray(6);
        decimals = randomBitArray(3);
        break;
    case Difficulty::moderate:
        integer = randomBitArray(10);
        decimals = randomBitArray(4);
        break;
    case Difficulty::hard:
        integer = randomBitArray(16);
        decimals = randomBitArray(5);
        break;
    }

    bool sign = randomBit();
    qDebug() << "Generated Number: " << integer << "." << decimals << " with sign bit: " << sign;
    return DecimalNumberBaseTwo(sign, integer, decimals);
}

double FloatWidget::toDouble(const DecimalNumberBaseTwo &dec_number)
{
    double out = 0;

    qDebug() << "Converting integer array: " << dec_number.integer;
    // Convert integer part
    int j = 0;
    for (int i = dec_number.integer.size() -1; i >= 0; i--) {
        if (dec_number.integer[i])
            out += pow(2, j);
        j++;
    }

    qDebug() << "Converting decimal array: " << dec_number.decimals;
    // convert decimal part
    j = 1;
    for (int i = 0; i < dec_number.decimals.size(); i ++) {
        if (dec_number.decimals[i])
            out += pow(2, -j);
        j++;
    }

    if (dec_number.sign)
        out = out * -1;

    return out;
}

QBitArray FloatWidget::randomBitArray(int size)
{
//    qDebug() << "Generating Array of size: " << size;
    QBitArray array(size);
    for (int i = 0; i < size; i++)
        array[i] = randomBit();

    // do not return an array consisting of only zeroes
    while (!array.count(true)) {
        for (int i = 0; i < size; i++)
            array[i] = randomBit();
    }
//    qDebug() << array;
    return array;
}

// Signals and Slots

void FloatWidget::connectSignals()
{
    QObject::connect(ui->generatePushButton, &QPushButton::clicked,
                     this,                   &FloatWidget::on_generateExample);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &FloatWidget::on_nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &FloatWidget::on_resultRequested);
}

void FloatWidget::on_generateExample()
{
    m_step = 0;
    Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());
    qDebug() << "Difficulty: " << static_cast<int>(difficulty);
    DecimalNumberBaseTwo number = generateDecimalNumber(difficulty);

    auto value = toDouble(number);
    qDebug() << "To Double: " << value;


    ui->doubleSpinBox->setValue(value);
    m_decimalNumberBaseTwo = number;
    ui->nextPushButton->setEnabled(true);
    ui->resultPushButton->setEnabled(true);
    clearLabels();
}

// Step 1 - Convert the decimal base 10 number to its absolute base 2 representation
void FloatWidget::decimalToBinary()
{
    if (m_decimalNumberBaseTwo.integer.size() == 0) {
        ui->binaryLabel->setText("The binary representation of the absolute value of 0 is 0");
        return;
    }

    QString binary_string;

    QBitArray* integer = &m_decimalNumberBaseTwo.integer;
    for (int i = 0; i < integer->size(); i++) {
        binary_string.append(QString::number(integer->at(i)));
    }

    binary_string.append(QLatin1Char('.'));

    QBitArray *decimals = &m_decimalNumberBaseTwo.decimals;
    for (int i = 0; i < decimals->size(); i++) {
        binary_string.append(QString::number(decimals->at(i)));
    }

    // Remove leading zeroes
    binary_string.remove(QRegularExpression("^[0]*"));

    const QString out = QString("The binary representation of the absolute value of %1 is: %2").arg(ui->doubleSpinBox->cleanText(), binary_string);
    ui->binaryDisplayLabel->setText(out);
    m_binary_string = binary_string;
}

// Step 2 - Determine the bias
void FloatWidget::determineBias()
{
    // create a copy - the member variable will get modified
    QString unbiased_binary_string = m_binary_string;

    // get the decimal separator position
    int original_decimal_position = m_binary_string.indexOf(QLatin1Char('.'));
    // get the position of the first '1'
    int leftmost_one = m_binary_string.indexOf(QLatin1Char('1'));
    // determine how many positions the decimal point needs to be shifted
    m_shift = original_decimal_position - (leftmost_one + 1);
    // edit the binary string
    m_binary_string.remove(0, leftmost_one);
    m_binary_string.remove(QLatin1Char('.'));
    m_binary_string.insert(1, QLatin1Char('.'));


    QString out = QString("%1 = %2 x 2^%3").arg(unbiased_binary_string, m_binary_string, QString::number(m_shift));
    ui->biasDisplayLabel->setText(out);
}

// Step 3 - Pad the mantissa
void FloatWidget::padMantissa()
{
    m_mantissa = m_binary_string;
    m_mantissa.remove(0,2);
    while (m_mantissa.length() != 23)
        m_mantissa.append(QLatin1Char('0'));
    ui->mantissaDisplayLabel->setText(QString("Remove the leading one and pad to 23 digits: <font color='orange'>%1</font>").arg(m_mantissa));
}

// Step 4 - Determine the biased exponent
void FloatWidget::determineExponent()
{
    qDebug() << "shift:" << m_shift;
    int biased_exponent = 127 + m_shift;
    qDebug() << "biased exponent: " << biased_exponent;
    m_biased_exponent_string = (QString::number(biased_exponent, 2));

    ui->exponentDisplayLabel->setText(QString("127 + %1 = %2, which is <font color='green'>%3</font> in binary representation").arg(QString::number(m_shift),QString::number(biased_exponent), m_biased_exponent_string));
}
// Step 5 - Determine the sign bit
void FloatWidget::determineSignBit()
{
    bool sign = m_decimalNumberBaseTwo.sign;
    QString sign_string;
    sign ? sign_string = "positive" : sign_string = "negative";
    QString out = QString("%1 is a %2 number, so the sign bit is <font color='blue'> %3 </font>").arg(ui->doubleSpinBox->cleanText(), sign_string, QString::number(sign));

    ui->signBitDisplayLabel->setText(out);
}

// Step 6 - concatenate
void FloatWidget::concatenate()
{
    m_Z32 = QString("%1%2%3").arg(QString::number(m_decimalNumberBaseTwo.sign),
                                  m_biased_exponent_string,
                                  m_mantissa);

    QString out = QString("<font color='blue'>%1</font><font color='green'>%2</font><font color='orange'>%3</font>")
            .arg(QString::number(m_decimalNumberBaseTwo.sign),
                 m_biased_exponent_string,
                 m_mantissa);
    ui->concatDisplayLabel->setText(out);

}

// Step 7 - convert Z32 to 0x
void FloatWidget::toHex()
{
    bool ok;
    auto value = m_Z32.toULong(&ok, 2);  //convert Bit String to int
    qDebug() << "Conversion successful:" << ok;

    QString hex_string = QString::number(value, 16);  //The new base is 16

    qDebug() << m_Z32 << "to int: " << value << "to hex:" << hex_string;
    ui->hexDisplayLabel->setText("0x" + hex_string);
}

void FloatWidget::on_nextStepRequested()
{
    switch (m_step) {
    case 0:
        qDebug() << "Step 1 - Convert the decimal base 10 number to its absolute base 2 representation";
        decimalToBinary();
        m_step++;
        break;
    case 1:
        qDebug() << "Step 2 - Determine the bias";
        determineBias();
        m_step++;
        break;
    case 2:
        qDebug() << "Step 3 - Pad the mantissa";
        padMantissa();
        m_step++;
        break;
    case 3:
        qDebug() << "Step 4 - Determine the biased exponent";
        determineExponent();
        m_step++;
        break;
    case 4:
        qDebug() << "Step 5 - Determine the sign bit";
        determineSignBit();
        m_step++;
        break;
    case 5:
        qDebug() << "Step 6 - Concatenate the elements";
        concatenate();
        m_step++;
        break;
    case 6:
        qDebug() << "Step 7 - Convert to Hexadecimal";
        toHex();
        ui->nextPushButton->setEnabled(false);
        ui->resultPushButton->setEnabled(false);
        break;
    default:
        break;

    }
}

void FloatWidget::on_resultRequested()
{
    for (int i = 0; i < 8 ; i++)
        on_nextStepRequested();
}



