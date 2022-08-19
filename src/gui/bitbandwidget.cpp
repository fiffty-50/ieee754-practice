#include "bitbandwidget.h"
#include "qmessagebox.h"
#include "src/nibble.h"
#include "ui_bitbandwidget.h"
#include "src/exercisegenerator.h"

BitBandWidget::BitBandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitBandWidget)
{
    ui->setupUi(this);
    connectSignals();
    clearLabels();

    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
    ui->customLabel->setVisible(false);
    ui->customLineEdit->setVisible(false);
    ui->bitPosSpinBox->setVisible(false);
}

BitBandWidget::~BitBandWidget()
{
    delete ui;
}

void BitBandWidget::clearLabels()
{
    for (const auto label : this->findChildren<QLabel*>()) {
        label->setText(QString());
        label->setAlignment(Qt::AlignLeft);
    }
    ui->difficultyLabel->setText("Difficulty");
}

bool BitBandWidget::checkUserInput()
{
    bool ok;
    int user_addr = ui->customLineEdit->text().toInt(&ok, 16);

    if(!ok) {
        badUserInput(InputError::NotRecognized);
        return false;
    }

    switch (m_direction) {
    case FromBBA:
        if ((user_addr < 0x22000000) | (user_addr > 0x23ffffff)) {
            badUserInput(InputError::OutOfRangeBBA);
            return false;
        }
        break;
    case FromBitPos:
        if ((user_addr < 0x20000000) | (user_addr > 0x200fffff)) {
            badUserInput(InputError::OutOfRangeBB);
            return false;
        }
        break;
    default:
        return false;
        break;
    }
    return true;
}

void BitBandWidget::badUserInput(InputError error_code)
{
    QString error_msg;
    switch (error_code) {
    case InputError::NotRecognized:
        error_msg = "<p>Input could not be converted to a number. "
                    "Please enter a hexadecimal values.</p>";
        break;
    case InputError::OutOfRangeBBA:
        error_msg = "<p>Input out of range. Input  "
                    "has to be in the range</p> <tt>0x22000000 - 0x23ffffff</tt>";
        break;
    case InputError::OutOfRangeBB:
        error_msg = "<p>Input out of range. Input"
                    "has to be in the range</p> <tt>0x20000000 - 0x200fffff</tt>";
        break;
    default:
        break;
    }

    QMessageBox mb(this);
    mb.setIcon(QMessageBox::Warning);
    mb.setText(QString("Input Error: %1").arg(error_msg));
    mb.exec();
}

void BitBandWidget::buttonGroupClicked(QAbstractButton *button)
{
    if (button == ui->bitposButton)
        m_direction = Direction::FromBitPos;
    else
        m_direction = Direction::FromBBA;
    difficultyChanged(ui->difficultyComboBox->currentIndex());
}

void BitBandWidget::difficultyChanged(int index)
{
    if (index == 3) { // Custom
        ui->customLabel->setVisible(true);
        ui->customLineEdit->setVisible(true);
        switch(m_direction) {
        case Direction::FromBBA:
            ui->customLabel->setText("Enter a BBA (0x...)");
            ui->bitPosSpinBox->setVisible(false);
            break;
        case Direction::FromBitPos:
            ui->customLabel->setText("Enter a Byte Address (0x...) and Bit Position.");
            ui->bitPosSpinBox->setVisible(true);
            break;
        default:
            break;
        }

    } else {
        ui->customLabel->setVisible(false);
        ui->customLineEdit->setVisible(false);
        ui->bitPosSpinBox->setVisible(false);
    }
    clearLabels();
}

// Signals and Slots

void BitBandWidget::connectSignals()
{
    QObject::connect(ui->startPushButton,    &QPushButton::clicked,
                     this,                   &BitBandWidget::startButtonClicked);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &BitBandWidget::nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &BitBandWidget::resultRequested);
    QObject::connect(ui->buttonGroup,        &QButtonGroup::buttonClicked,
                     this,                   &BitBandWidget::buttonGroupClicked);
    QObject::connect(ui->difficultyComboBox, &QComboBox::currentIndexChanged,
                     this,                   &BitBandWidget::difficultyChanged);
}

void BitBandWidget::startButtonClicked()
{
    if (ui->difficultyComboBox->currentIndex() == 3) {
        if (!checkUserInput())
            return;
        bool ok;
        switch (m_direction) {
        case FromBBA:
            m_bitBand = BitBand(ui->customLineEdit->text().toInt(&ok, 16));
            break;
        case FromBitPos:
            m_bitBand = BitBand(ui->customLineEdit->text().toInt(&ok, 16), ui->bitPosSpinBox->value());
            break;
        default:
            break;
        }
    } else {
        Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());
        m_bitBand = ExerciseGenerator::generateBitBandExercise(difficulty);
    }

    DEB << "Bit Band: " << m_bitBand.bbAddress() << m_bitBand.bitPos() << m_bitBand.bbAliasAddress();

    m_step = 0;
    ui->nextPushButton->setEnabled(true);
    ui->resultPushButton->setEnabled(true);
    clearLabels();

    switch (m_direction) {
    case Direction::FromBBA:
        ui->step0Label->setText("Given the following:");
        ui->step0DisplayLabel->setText("Bit-Band Alias Address:  <b><tt>0x" + Tools::toHex(m_bitBand.bbAliasAddress())
                                         + "</b></tt>");
        break;
    case Direction::FromBitPos:
        ui->step0Label->setText("Given the following:");
        ui->step0DisplayLabel->setText(QString("Byte Address X = <b><tt>0x%1</tt></b>, Bit Number n = <b> %2 </b>")
                                         .arg(Tools::toHex(m_bitBand.bbAddress()))
                                         .arg(m_bitBand.bitPos()));
        break;
    default:
        break;
    }
}

void BitBandWidget::nextStepRequested()
{
    QString s_temp_1, s_temp_2, s_temp_3;
    int i_temp_1, i_temp_2, i_temp_3;

    switch (m_direction) {
    case Direction::FromBBA:
        switch (m_step) {
        case 0:
            ui->step1Label->setText("Calculate the Bit Position");
            ui->step1DisplayLabel->setText("BITPOS = (BBA & 0x1C) >> 2");
            m_step++;
            break;
        case 1:
            m_step++;
            i_temp_1 = m_bitBand.bbAliasAddress();
            i_temp_2 = i_temp_1 & 0x1c;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = Tools::toNibbles(i_temp_2);

            ui->step2Label->setText("<tt>1: BBA & 0x1C");
            ui->step2DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step2DisplayLabel->setText(QString("<tt>%1<br>"
                                                       "& 0001 1010<hr>"
                                                       "= %2<br>")
                                               .arg(s_temp_1, s_temp_2));
            break;
        case 2:
            i_temp_1 = (m_bitBand.bbAliasAddress() & 0x1c) >> 2;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = QString::number(i_temp_1);
            ui->step3Label->setText("<tt>2: >> 2");
            ui->step3DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step3DisplayLabel->setText(QString("<tt><hr>= %1</tt><br>The Bit Position (Base 10) is: <b>%2</b>.")
                                           .arg(s_temp_1, s_temp_2));
            m_step++;
            break;
        case 3:
            ui->step4Label->setText("Calculate the Byte Address");
            ui->step4DisplayLabel->setText("<tt>BYTEADR = ((BBA & 0x1FFF FE0) >> 5) | 0x2000 0000");
            m_step++;
            break;
        case 4:
            i_temp_1 = m_bitBand.bbAliasAddress();
            i_temp_2 = 0x1FFFFE0;
            i_temp_3 = i_temp_1 & i_temp_2 ;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = Tools::toNibbles(i_temp_2);
            s_temp_3 = Tools::toNibbles(i_temp_3);

            ui->step5Label->setText(QString("<tt>1: BBA & 0x%1")
                                    .arg(Tools::toHex(i_temp_2)));
            ui->step5DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step5DisplayLabel->setText(QString("<tt>%1<br>"
                                                     "& %2<hr>"
                                                     "= %3")
                                             .arg(s_temp_1, s_temp_2, s_temp_3)
                                                  );
            m_step++;
            break;
        case 5:
            i_temp_1 = (m_bitBand.bbAliasAddress() & 0x1FFFFE0) >> 5;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            ui->step6Label->setText("<tt>2: >> 5");
            ui->step6DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step6DisplayLabel->setText(QString("<hr><tt> = %1").arg(s_temp_1));
            m_step++;
            break;
        case 6:
            i_temp_1 = 0x20000000;
            i_temp_2 = m_bitBand.bbAddress();
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = Tools::toNibbles(i_temp_2);
            s_temp_3 = Tools::toHex(i_temp_2);

            ui->step7Label->setText(QString("<tt>3: | 0x%1").arg(Tools::toHex(i_temp_1)));

            ui->step7DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step7DisplayLabel->setText(QString("<tt> | %1<hr>"
                                                       "= %2<br><br>"
                                                       "</tt>The byte address is <b>0x%3")
                                               .arg(s_temp_1, s_temp_2, s_temp_3));
            ui->nextPushButton->setEnabled(false);
            ui->resultPushButton->setEnabled(false);
            m_step++;
            break;
        default:
            break;
        }
        break;
    case Direction::FromBitPos:
        switch (m_step) {
        case 0:
            ui->step1Label->setText("Calculate the BBA");
            ui->step1DisplayLabel->setText("<tt>BBA = 0x2200 0000 + (X − 0x2000 0000) × 32 + n × 4");
            m_step++;
            break;
        case 1:

            s_temp_1   = Tools::toNibbles(m_bitBand.bbAddress(), Tools::AlignRight);
            s_temp_2   = Tools::toNibbles(0x20000000, Tools::AlignRight);
            s_temp_3 = Tools::toNibbles(m_bitBand.bbAddress() - 0x20000000, Tools::AlignRight);

            ui->step2Label->setText("1: <tt>(X − 0x2000 0000)");
            ui->step2DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step2DisplayLabel->setText(
                        QString("<tt>%1<br>"
                                "- %2<hr>"
                                "= %3<br></tt>")
                        .arg(s_temp_1, s_temp_2, s_temp_3));
            m_step++;
            break;
        case 2:
            s_temp_1 = Tools::toNibbles((m_bitBand.bbAddress() - 0x20000000) << 5, Tools::AlignRight);
            ui->step3DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step3Label->setText("2: << 5 to multiply by 32");
            ui->step3DisplayLabel->setText(QString("<tt>&lt;&lt; 5<hr>"
                                                   "= %1</tt>")
                                           .arg(s_temp_1));
            m_step++;
            break;
        case 3:
            i_temp_1 = (m_bitBand.bbAddress() - 0x20000000) << 5;
            s_temp_1 = Tools::toHex(i_temp_1);
            ui->step4Label->setText("Convert back to Hexadecimal");
            ui->step4DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step4DisplayLabel->setText(QString("<tt><hr> = <b>0x%2")
                                           .arg(s_temp_1));
            m_step++;
            break;
        case 4:
            i_temp_1   = 0x22000000;
            s_temp_1   = Tools::toHex(i_temp_1);
            s_temp_2   = Tools::toHex(((m_bitBand.bbAddress() - 0x20000000) << 5) + i_temp_1);
            ui->step5Label->setText(QString("Add <tt>0x%1").arg(s_temp_1));
            ui->step5DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step5DisplayLabel->setText(QString("<tt>"
                                                   "+ 0x%1<hr>"
                                                   "= <b>0x%2</tt>")
                                           .arg(s_temp_1, s_temp_2));
            m_step++;
            break;
        case 5:
            i_temp_1 = m_bitBand.bitPos();
            s_temp_1 = QString("<tt>%1 x 4").arg(i_temp_1);
            s_temp_2 = Tools::toHex(4 * i_temp_1);
            s_temp_3 = Tools::toHex(m_bitBand.bbAliasAddress());
            ui->step6Label->setText(QString("Add %1").arg(s_temp_1));
            ui->step6DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step6DisplayLabel->setText(QString("<tt>+ 0x%1<hr>BBA = <b>0x%2")
                                           .arg(s_temp_2, s_temp_3));


        default:
            break;
        }
        break;
    default:
        break;
    }

}

void BitBandWidget::resultRequested()
{
    for (int i = 0; i < 8; i++)
        nextStepRequested();
}
