#include "ieeetodec.h"
#include "ui_ieeetodec.h"
#include "src/exercisegenerator.h"
#include <QDebug>

IeeeToDec::IeeeToDec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ieeetodec)
{
    ui->setupUi(this);
    connectSignals();
    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
    clearLabels();
}

IeeeToDec::~IeeeToDec()
{
    delete ui;
}

void IeeeToDec::connectSignals()
{
    QObject::connect(ui->generatePushButton, &QPushButton::clicked,
                     this,                   &IeeeToDec::on_generateExample);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &IeeeToDec::on_nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &IeeeToDec::on_resultRequested);
}

void IeeeToDec::clearLabels()
{
    ui->hexDisplayLabel->setText(QString());
    ui->binaryDisplayLabel->setText(QString());
    ui->biasDisplayLabel->setText(QString());
    ui->extractMantissaDisplayLabel->setText(QString());
    ui->deNormMantissaDisplayLabel->setText(QString());
    ui->signDisplayLabel->setText(QString());
    ui->decimalDisplayLabel->setText(QString());
}

void IeeeToDec::on_generateExample()
{
    clearLabels();
    m_step = 0;
    Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());
    m_number = ExerciseGenerator::generateDecimalNumber(difficulty);

    const QString text = QLatin1String("<b>") + m_number.toString(Number::Format::Hexadecimal) + QLatin1String("</b>");
    ui->hexDisplayLabel->setText(text);
    ui->nextPushButton->setEnabled(true);
    ui->resultPushButton->setEnabled(true);
}

void IeeeToDec::on_nextStepRequested()
{
    switch (m_step) {
    case 0:
        qDebug() << "Step 1 - Convert the hexadecimal number to its base 2 representation";
        hexToBinary();
        m_step++;
        break;
    case 1:
        qDebug() << "Step 2 - Determine the bias";
        determineBias();
        m_step++;
        break;
    case 2:
        qDebug() << "Step 3 - Extract the mantissa";
        extractMantissa();
        m_step++;
        break;
    case 3:
        qDebug() << "Step 4 - De-normalise the mantissa";
        deNormMantissa();
        m_step++;
        break;
    case 4:
        qDebug() << "Step 6 - Check the sign bit";
        applySign();
        m_step++;
        break;
    case 5:
        qDebug() << "Step 7 - Convert to decimal";
        convertToDecimal();
        m_step++;
        break;
    //case 6:
    //    qDebug() << "Step 7 - Break up the string into nibbles";
    //    stringToNibbles();
    //    m_step++;
    //    break;
    //case 7:
    //    qDebug() << "Step 8 - Convert to Hexadecimal";
    //    stringToHex();
    //    ui->nextPushButton->setEnabled(false);
    //    ui->resultPushButton->setEnabled(false);
    //    break;
    default:
        break;
    }
}

void IeeeToDec::on_resultRequested()
{
    for (int i = 0; i < 6 ; i++)
        on_nextStepRequested();
}

// Steps / Conversion Logic

void IeeeToDec::hexToBinary()
{
    ui->binaryDisplayLabel->setText(m_number.toNibbles(true));
}

void IeeeToDec::determineBias()
{
    const QString bias_binary = m_number.biasedExponent(Number::Format::Binary);
    const QString bias_decimal = m_number.biasedExponent(Number::Format::Decimal);
    const QString shift = QString::number(m_number.getBias());

    const auto text = QString("%1 = %2 -> %2 - 127 = <b><font color='red'>%3</font></b>").arg(bias_binary, bias_decimal, shift);
    ui->biasDisplayLabel->setText(text);
}

void IeeeToDec::extractMantissa()
{
    const QString mantissa = QString("<font color='red'>1.</font><font color='orange'>%1</font>").arg(m_number.mantissa());
    ui->extractMantissaDisplayLabel->setText(mantissa);
}

void IeeeToDec::deNormMantissa()
{
    QString mantissa = m_number.mantissa();
    mantissa.prepend(QLatin1String("1"));
    int dec_pos = m_number.getBias() + 1;
    mantissa.insert(dec_pos, QLatin1Char('.'));
    ui->deNormMantissaDisplayLabel->setText(mantissa);
}

void IeeeToDec::applySign()
{
    QString text;
    if (m_number.getSign())
        text = QString("The sign bit is <b><font color='blue'>1</font></b> so the number is <b>negative</b>");
    else
        text = QString("The sign bit is <b><font color='blue'>0</font></b> so the number is <b>positive</b>");
    ui->signDisplayLabel->setText(text);
}

void IeeeToDec::convertToDecimal()
{
    const auto text = QLatin1String("<b>") + m_number.toString(Number::Format::Decimal);
    ui->decimalDisplayLabel->setText(text);
}
