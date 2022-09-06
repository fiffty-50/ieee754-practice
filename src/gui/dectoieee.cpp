#include "dectoieee.h"
#include "ui_dectoieee.h"
#include "src/exercisegenerator.h"
#include <QMessageBox>
#include <QDebug>

DecToIeee::DecToIeee(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecToIeee)
{
    ui->setupUi(this);
    connectSignals();
    clearLabels();
    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
    ui->decimalNumberCustomLabel->setVisible(false);
    ui->lineEdit->setVisible(false);
}

DecToIeee::~DecToIeee()
{
    delete ui;
}

void DecToIeee::clearLabels()
{
    const QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (const auto &label : labels)
        if(label->objectName().contains(QLatin1String("Display")))
            label->setText(QString());
}

// Signals and Slots

void DecToIeee::connectSignals()
{
    QObject::connect(ui->startPushButton,    &QPushButton::clicked,
                     this,                   &DecToIeee::on_StartConversion);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &DecToIeee::on_nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &DecToIeee::on_resultRequested);
    QObject::connect(ui->difficultyComboBox, &QComboBox::currentIndexChanged,
                     this,                   &DecToIeee::on_difficultyChanged);
}

void DecToIeee::on_StartConversion()
{
    float number_in;
    switch (ui->difficultyComboBox->currentIndex()) {
    case 3: // custom
        bool ok;
        number_in = ui->lineEdit->text().toFloat(&ok);
        if (!ok) {
            QMessageBox mb(this);
            mb.setText("Unable to interpret input. Plase enter a base 10 decimal number.");
            mb.exec();
            ui->lineEdit->setText(QString());
            return;
        }
        DEB << "Number: " << number_in;
        m_number = Number::fromFloat(number_in);
        m_step = 0;
        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        clearLabels();
        break;
    default:
        m_step = 0;
        Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());
        m_number = ExerciseGenerator::generateFloatingPointExercise(difficulty);

        DEB << __PRETTY_FUNCTION__ << "Number not showing on label..?";
        ui->decimalLabel->setText(m_number.toString(Number::Format::Decimal));
        ui->decimalDisplayLabel->setVisible(true);
        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        clearLabels();
        break;
    }

}

void DecToIeee::on_nextStepRequested()
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
        qDebug() << "Step 7 - Break up the string into nibbles";
        stringToNibbles();
        m_step++;
        break;
    case 7:
        qDebug() << "Step 8 - Convert to Hexadecimal";
        stringToHex();
        ui->nextPushButton->setEnabled(false);
        ui->resultPushButton->setEnabled(false);
        break;
    default:
        break;
    }
}

void DecToIeee::on_resultRequested()
{
    for (int i = 0; i < 8 ; i++)
        on_nextStepRequested();
}

void DecToIeee::on_difficultyChanged(int index)
{
    if (index == 3) { // Custom
        ui->decimalLabel->setVisible(false);
        ui->decimalDisplayLabel->setVisible(false);
        ui->decimalNumberCustomLabel->setVisible(true);
        ui->lineEdit->setVisible(true);
        clearLabels();
    } else { // generate Example
        ui->decimalLabel->setVisible(true);
        ui->decimalDisplayLabel->setVisible(true);
        ui->decimalNumberCustomLabel->setVisible(false);
        ui->lineEdit->setVisible(false);
        clearLabels();
    }

}

// Conversion Logic
// Step 1 - Convert the decimal base 10 number to its absolute base 2 representation
void DecToIeee::decimalToBinary()
{
    ui->binaryDisplayLabel->setText(m_number.absoluteBinary());
}

// Step 2 - Determine the bias
void DecToIeee::determineBias()
{
    QString absolute_binary_string = m_number.absoluteBinary();
    QString normalised_binary_string = m_number.normalisedBinary();
    int shift = m_number.getBias();


    QString out = QString("%1 = %2 x 2^<b><font color='red'>%3</font></b>").arg(absolute_binary_string, normalised_binary_string, QString::number(shift));
    ui->biasDisplayLabel->setText(out);
}

// Step 3 - Pad the mantissa
void DecToIeee::padMantissa()
{
    QString mantissa = m_number.mantissa();
    ui->mantissaDisplayLabel->setText(QString("<font color='orange'>%1</font>").arg(mantissa));
}

// Step 4 - Determine the biased exponent
void DecToIeee::determineExponent()
{
    QString biased_exponent = m_number.biasedExponent();
    int shift = m_number.getBias();

    ui->exponentDisplayLabel->setText(QString("127 + <b><font color='red'>%1</font></b> = %2 -> to binary: <font color='green'>%3</font>").arg(QString::number(shift),QString::number(127 + shift), biased_exponent));
}
// Step 5 - Determine the sign bit
void DecToIeee::determineSignBit()
{
    bool sign = m_number.getSign();
    QString sign_string;
    sign ? sign_string = "negative" : sign_string = "positive";
    QString out = QString("%1 is a <b>%2</b> number, so the sign bit is <b><font color='blue'>%3</font></b>").arg(ui->decimalDisplayLabel->text(), sign_string, QString::number(sign));

    ui->signBitDisplayLabel->setText(out);
}

// Step 6 - concatenate
void DecToIeee::concatenate()
{
    QString sign = QString::number(m_number.getSign());
    QString exponent = m_number.biasedExponent();
    QString mantissa = m_number.mantissa();

    QString out = QString("<b><font color='blue'>%1 </font><font color='green'>%2 </font><font color='orange'>%3</font></b>")
            .arg(sign,
                 exponent,
                 mantissa);
    ui->concatDisplayLabel->setText(out);

}

// Step 7 - split the bits into nibbles
void DecToIeee::stringToNibbles()
{
    ui->nibblesDisplayLabel->setText(m_number.toString(Number::Format::Nibbles));
}

// Step 8 - convert Z32 to 0x
void DecToIeee::stringToHex()
{
    ui->hexDisplayLabel->setText("<b>0x" + m_number.toString(Number::Format::Hexadecimal) + "</b>");
}
