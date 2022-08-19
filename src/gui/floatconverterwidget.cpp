#include "floatconverterwidget.h"
#include "qmessagebox.h"
#include "src/exercisegenerator.h"
#include "ui_floatconverterwidget.h"

FloatConverterWidget::FloatConverterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FloatConverterWidget)
{
    ui->setupUi(this);
    connectSignals();
    clearLabels();
    ui->CustomLabel->setVisible(false);
    ui->customLineEdit->setVisible(false);
    ui->exerciseLabel->setVisible(false);
    ui->exerciseDisplayLabel->setVisible(false);
    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
}

FloatConverterWidget::~FloatConverterWidget()
{
    delete ui;
}

void FloatConverterWidget::connectSignals()
{
    QObject::connect(ui->startPushButton,    &QPushButton::clicked,
                     this,                   &FloatConverterWidget::startConversion);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &FloatConverterWidget::nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &FloatConverterWidget::resultRequested);
    QObject::connect(ui->buttonGroup,        &QButtonGroup::buttonClicked,
                     this,                   &FloatConverterWidget::buttonGroupClicked);
    QObject::connect(ui->difficultyComboBox, &QComboBox::currentIndexChanged,
                     this,                   &FloatConverterWidget::difficultyChanged);
}

void FloatConverterWidget::clearLabels()
{
    const QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (const auto &label : labels)
            label->setText(QString());
    if (ui->difficultyComboBox->currentIndex() == 3) {// custom
        ui->difficultyLabel->setText("Difficulty");
    } else {
        ui->difficultyLabel->setText("Difficulty");

    }

}

void FloatConverterWidget::buttonGroupClicked(QAbstractButton *button)
{
    if (button == ui->decToHexButton)
        m_direction = Direction::DecToHex;
    else
        m_direction = Direction::HexToDec;
}

void FloatConverterWidget::difficultyChanged(int index)
{
    DEB << "Index: " << index;
    if (index == 3) { // Custom
        ui->exerciseLabel->setVisible(false);
        ui->exerciseDisplayLabel->setVisible(false);
        ui->CustomLabel->setVisible(true);
        ui->customLineEdit->setVisible(true);
        ui->CustomLabel->setText("Enter a decimal number");
    } else {
        ui->CustomLabel->setVisible(false);
        ui->customLineEdit->setVisible(false);
        ui->exerciseLabel->setVisible(true);
        ui->exerciseDisplayLabel->setVisible(true);
    }
}

void FloatConverterWidget::startConversion()
{
    float number_in;
    QString string_in;
    switch (ui->difficultyComboBox->currentIndex()) {
    case 3: // custom
        clearLabels();
        m_step = 0;
        bool ok;
        string_in = ui->customLineEdit->text();
        if (string_in.contains(QLatin1Char(','))) {
            string_in.replace(',','.');
            ui->customLineEdit->setText(string_in);
        }

        number_in = string_in.toFloat(&ok);
        if (!ok) {
            QMessageBox mb(this);
            mb.setText("Unable to interpret input. Plase enter a base 10 decimal number.");
            mb.exec();
            ui->customLineEdit->setText(QString());
            return;
        }
        m_number = FloatRepresentation::fromFloat(number_in);

        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        break;
    default:
        clearLabels();
        m_step = 0;
        int diff_index = ui->difficultyComboBox->currentIndex();
        difficultyChanged(diff_index);
        m_number = ExerciseGenerator::generateFloatingPointExerciseNew(Difficulty(diff_index));

        ui->exerciseLabel->setText("Convert the following number:");
        ui->exerciseDisplayLabel->setText(m_number.toString(FloatRepresentation::Decimal));
        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        break;
    }
    DEB << "Input Number: " << m_number.toFloat();
}

void FloatConverterWidget::nextStepRequested()
{
    switch (m_direction) {
    case Direction::DecToHex:
        switch(m_step) {
        case 0:
            qDebug() << "Step 1 - Convert the decimal base 10 number to its absolute base 2 representation";
            //decimalToBinary();
            m_step++;
            break;
        //case 1:
        //    qDebug() << "Step 2 - Determine the bias";
        //    determineBias();
        //    m_step++;
        //    break;
        //case 2:
        //    qDebug() << "Step 3 - Pad the mantissa";
        //    padMantissa();
        //    m_step++;
        //    break;
        //case 3:
        //    qDebug() << "Step 4 - Determine the biased exponent";
        //    determineExponent();
        //    m_step++;
        //    break;
        //case 4:
        //    qDebug() << "Step 5 - Determine the sign bit";
        //    determineSignBit();
        //    m_step++;
        //    break;
        //case 5:
        //    qDebug() << "Step 6 - Concatenate the elements";
        //    concatenate();
        //    m_step++;
        //    break;
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
        }
    case Direction::HexToDec:

        break;
    default:
        break;
    }
}

void FloatConverterWidget::resultRequested()
{

}


