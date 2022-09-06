#include "floatconverterwidget.h"
#include "qmessagebox.h"
#include "src/exercisegenerator.h"
#include "src/tools.h"
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
                     this,                   &FloatConverterWidget::directionChanged);
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

void FloatConverterWidget::directionChanged(QAbstractButton *button)
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
        DEB << __PRETTY_FUNCTION__ << "NOT CURRENTLY WORKING";
        return;
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
//        m_number = FloatRepresentation::fromFloat(number_in);

        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        break;
    default:
        clearLabels();
        m_step = 0;
        int diff_index = ui->difficultyComboBox->currentIndex();
        difficultyChanged(diff_index);
        m_number = ExerciseGenerator::generateFloatingPointExercise(Difficulty(diff_index));

        ui->exerciseLabel->setText("Convert the following number:");
        auto text = QString("<b><tt>%1</b></tt>");
        switch (m_direction){
        case DecToHex:
            text = text.arg(m_number.toString(Number::Format::Decimal));
            break;
        case HexToDec:
            text = text.arg(m_number.toString(Number::Format::Hexadecimal));
            break;
        default:
            break;
        }
        ui->exerciseDisplayLabel->setText(text);
        ui->nextPushButton->setEnabled(true);
        ui->resultPushButton->setEnabled(true);
        break;
    }
}

void FloatConverterWidget::nextStepRequested()
{
    // allocate some temps to use in switch
    int int_temp_1, int_temp_2, int_temp_3;
    QString string_temp_1, string_temp_2, string_temp_3, string_temp_4;

    switch (m_direction) {
    case Direction::DecToHex:
        switch(m_step) {
        case 0:
            // Convert the decimal base 10 number to its absolute base 2 representation
            string_temp_1 = m_number.absoluteBinary();
            ui->step0Label->setText("Convert to Binary");
            ui->step0DisplayLabel->setText(QString("<tt>%1").arg(string_temp_1));
            m_step++;
            break;
        case 1:
            // Determine the bias";
            string_temp_1 = m_number.absoluteBinary();
            string_temp_2 = m_number.normalisedBinary();
            string_temp_3 = QString::number(m_number.getBias());

            ui->step1Label->setText("Determine the bias");
            ui->step1DisplayLabel->setText(QString("<tt>%1 = %2 x 2^<b><font color='red'>%3</font></b>")
                                           .arg(string_temp_1, string_temp_2, string_temp_3 ));
            m_step++;
            break;
        case 2:
            // Determine the mantissa";
            string_temp_1 = m_number.mantissa(true);
            ui->step2Label->setText("Determine the mantissa");
            ui->step2DisplayLabel->setText(QString("<tt><b><font color='orange'>%1</font></bb>")
                                           .arg(string_temp_1));
            m_step++;
            break;
        case 3:
            // Determine the biased exponent;
            string_temp_1 = QString::number(m_number.getBias());
            string_temp_2 = QString::number(m_number.getBias() + 127);

            ui->step3Label->setText("Determine the exponent");
            ui->step3DisplayLabel->setText(QString("<tt>127 + <b><font color='red'>%1</font></b> = %2")
                                           .arg(string_temp_1,string_temp_2));
            m_step++;
            break;
        case 4:
            // Convert to binary
            string_temp_1 = m_number.biasedExponent();
            string_temp_1.insert(1, ' ');
            string_temp_1.insert(5, ' ');
            ui->step4Label->setText("Convert to binary");
            ui->step4DisplayLabel->setText(QString("<tt><b><font color='green'>%1</font></tt></b>")
                                           .arg(string_temp_1));
            m_step++;
            break;
        case 5:
            // Determine sign bit
            int_temp_1 = m_number.getSign();
            if (int_temp_1)
                string_temp_1 = "The number is negative, so the sign bit is: <font color='blue'><b>1</b></font>";
            else
                string_temp_1 = "The number is positive, so the sign bit is: <font color='blue'><b>0</b></font>";

            ui->step5Label->setText("Determine the sign bit");
            ui->step5DisplayLabel->setText(string_temp_1);
            m_step++;
            break;
        case 6:
            // Concatenate
            ui->step6Label->setText("Concatenate the elements");
            ui->step6DisplayLabel->setText(m_number.toString(Number::Format::StyledNibbles));
            m_step++;
            break;
        case 7:
            // Convert to Hex
            ui->step7Label->setText("Convert to hexadecimal");
            ui->step7DisplayLabel->setText(QString("<tt><bb>0x%1</tt></bb>")
                                           .arg(m_number.toString(Number::Format::Hexadecimal)));
            m_step++;
            break;
        }
    case Direction::HexToDec:

        break;
    default:
        break;
    }
}

void FloatConverterWidget::resultRequested()
{
    for (int i = 0; i < 10; i++)
        nextStepRequested();
}


