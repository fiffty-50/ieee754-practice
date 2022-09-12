#include "bitbandwidget.h"
#include "qbuttongroup.h"
#include "qmessagebox.h"
#include "src/tools.h"
#include "ui_bitbandwidget.h"
#include "src/exercisegenerator.h"

BitBandWidget::BitBandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitBandWidget)
{
    ui->setupUi(this);
    clearLabels();

    ui->nextPushButton->setEnabled(false);
    ui->resultPushButton->setEnabled(false);
    ui->customLabel->setVisible(false);
    ui->customLineEdit->setVisible(false);
    ui->bitPosSpinBox->setVisible(false);

    auto directionButtons = new QButtonGroup(this);
    directionButtons->addButton(ui->fromBbaCheckBox);
    directionButtons->addButton(ui->fromBitPosCheckBox);
    ui_directionButtonGroup = directionButtons;

    auto addressingButtons = new QButtonGroup(this);
    addressingButtons->addButton(ui->byteAddressedCheckBox);
    addressingButtons->addButton(ui->wordAddressedCheckBox);
    ui_addressingButtonGroup = addressingButtons;
    connectSignals();

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

    BitBand b;

    switch (m_direction) {
    case FromBBA:
        b = BitBand(user_addr);
        if(b.addressSpace() == BitBand::AddressSpace::OutOfRange)
            badUserInput(InputError::OutOfRange);
        break;
    case FromBitPos:
        b = BitBand(user_addr, ui->bitPosSpinBox->value());
        if(b.addressSpace() == BitBand::AddressSpace::OutOfRange)
            badUserInput(InputError::OutOfRange);
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
    case InputError::OutOfRange:
        error_msg = "<p>Input out of range.</p>";
        break;
    default:
        break;
    }

    QMessageBox mb(this);
    mb.setIcon(QMessageBox::Warning);
    mb.setText(QString("Input Error: %1").arg(error_msg));
    mb.exec();
}

void BitBandWidget::directionChanged(QAbstractButton *button)
{
    if (button == ui->fromBitPosCheckBox)
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
            ui->customLabel->setText("Enter a Base Address (0x...) and Bit Position.");
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

void BitBandWidget::addressingTypeChanged(QAbstractButton *button)
{
    if(button == ui->byteAddressedCheckBox)
        m_addressing = BitBand::AddressingType::Byte;
    else
        m_addressing = BitBand::AddressingType::Word;
}

// Signals and Slots

void BitBandWidget::connectSignals()
{
    QObject::connect(ui->startPushButton,       &QPushButton::clicked,
                     this,                      &BitBandWidget::startButtonClicked);
    QObject::connect(ui->nextPushButton,        &QPushButton::clicked,
                     this,                      &BitBandWidget::nextStepRequested);
    QObject::connect(ui->resultPushButton,      &QPushButton::clicked,
                     this,                      &BitBandWidget::resultRequested);
    QObject::connect(ui->difficultyComboBox,    &QComboBox::currentIndexChanged,
                     this,                      &BitBandWidget::difficultyChanged);
    QObject::connect(ui_directionButtonGroup,   &QButtonGroup::buttonClicked,
                     this,                      &BitBandWidget::directionChanged);
    QObject::connect(ui_addressingButtonGroup,  &QButtonGroup::buttonClicked,
                     this,                      &BitBandWidget::addressingTypeChanged);
}

void BitBandWidget::startButtonClicked()
{
    if (ui->difficultyComboBox->currentIndex() == 3) {
        if (!checkUserInput())
            return;
        bool ok;
        switch (m_direction) {
        case FromBBA:
            m_bitBand = BitBand(ui->customLineEdit->text().toInt(&ok, 16), m_addressing);
            break;
        case FromBitPos:
            m_bitBand = BitBand(ui->customLineEdit->text().toInt(&ok, 16), ui->bitPosSpinBox->value(), m_addressing);
            break;
        default:
            break;
        }
    } else {
        Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());

        switch (m_direction) {
        case FromBBA:
            m_bitBand = ExerciseGenerator::generateBitBandExercise(difficulty, m_addressing, ExerciseGenerator::FromAliasAddress);
            break;
        case FromBitPos:
            m_bitBand = ExerciseGenerator::generateBitBandExercise(difficulty, m_addressing, ExerciseGenerator::FromBaseAddress);
            break;
        default:
            break;
        }
    }

    m_step = 0;
    ui->nextPushButton->setEnabled(true);
    ui->resultPushButton->setEnabled(true);
    clearLabels();
    DEB << "Bit Band:" << m_bitBand;

    switch (m_direction) {
    case Direction::FromBBA:
        ui->step0Label->setText("Given the following:");
        ui->step0DisplayLabel->setText("Bit-Band Alias Address:  <b><tt>0x" + Tools::toHex(m_bitBand.BitBand::aliasAddress())
                                         + "</b></tt>");
        ui->step1Label->setText("Calculate the Bit Position");
        break;
    case Direction::FromBitPos:
        ui->step0Label->setText("Given the following:");
        ui->step0DisplayLabel->setText(QString("Base Address X = <b><tt>0x%1</tt></b>, Bit Number n = <b> %2 </b>")
                                         .arg(Tools::toHex(m_bitBand.baseAddress()))
                                         .arg(m_bitBand.bitPos()));
        ui->step1Label->setText("Calculate the BBA");
        break;
    default:
        break;
    }
}

void BitBandWidget::nextStepRequested()
{
    QString s_temp_1, s_temp_2, s_temp_3, s_temp_4;
    int i_temp_1, i_temp_2, i_temp_3;

    switch (m_direction) {
    case Direction::FromBBA:
        switch (m_step) {
        case 0:
            s_temp_1 = Tools::toHex(m_bitBand.bitPosOffset());
            ui->step1DisplayLabel->setText(QString("<tt>BITPOS = (BBA & 0x%1) >> 2").arg(s_temp_1));
            m_step++;
            break;
        case 1:
            m_step++;
            i_temp_1 = m_bitBand.BitBand::aliasAddress();
            i_temp_2 = i_temp_1 & m_bitBand.bitPosOffset();
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = Tools::toNibbles(m_bitBand.bitPosOffset());
            s_temp_3 = Tools::toNibbles(i_temp_2);
            s_temp_4 = Tools::toHex(m_bitBand.bitPosOffset());

            ui->step2Label->setText(QString("<tt>1: BBA & 0x%1").arg(s_temp_4));
            ui->step2DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step2DisplayLabel->setText(QString("<tt>%1<br>"
                                                       "& %2<hr>"
                                                       "= %3<br>")
                                               .arg(s_temp_1, s_temp_2, s_temp_3));
            break;
        case 2:
            i_temp_1 = (m_bitBand.aliasAddress() & m_bitBand.bitPosOffset()) >> 2;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            s_temp_2 = QString::number(i_temp_1);
            ui->step3Label->setText("<tt>2: >> 2");
            ui->step3DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step3DisplayLabel->setText(QString("<tt><hr>= %1</tt><br>The Bit Position (Base 10) is: <b>%2</b>.")
                                           .arg(s_temp_1, s_temp_2));
            m_step++;
            break;
        case 3:
            DEB << "HERE!";
            s_temp_1 = Tools::toHex(m_bitBand.baseAddressOffset());
            ui->step4Label->setText("Calculate the Base Address");
            ui->step4DisplayLabel->setText(QString("<tt>BASEADR = ((BBA & %1) ) | 0x2000 0000").arg(s_temp_1));
            m_step++;
            break;
        case 4:
            i_temp_1 = m_bitBand.BitBand::aliasAddress();
            i_temp_2 = m_bitBand.baseAddressOffset();
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
            i_temp_1 = (m_bitBand.BitBand::aliasAddress() & m_bitBand.baseAddressOffset()) >> 5;
            s_temp_1 = Tools::toNibbles(i_temp_1);
            ui->step6Label->setText("<tt>2: >> 5");
            ui->step6DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step6DisplayLabel->setText(QString("<hr><tt> = %1").arg(s_temp_1));
            m_step++;
            break;
        case 6:
            i_temp_1 = 0x20000000;
            i_temp_2 = m_bitBand.baseAddress();
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
            i_temp_1 = m_bitBand.aliasStartAddress();
            i_temp_2 = m_bitBand.addressSpaceStartAddress();
            s_temp_1 = Tools::toHex(i_temp_1);
            s_temp_2 = Tools::toHex(i_temp_2);
            ui->step1DisplayLabel->setText(QString("<tt>BBA = 0x%1 + (X − 0x%2) × 32 + n × 4")
                                           .arg(s_temp_1, s_temp_2));
            m_step++;
            break;
        case 1:
            i_temp_1 = m_bitBand.addressSpaceStartAddress();
            i_temp_2 = m_bitBand.baseAddress();
            s_temp_1 = Tools::toHex(i_temp_1);
            s_temp_2 = Tools::toNibbles(i_temp_1);
            s_temp_3 = Tools::toNibbles(i_temp_2);
            s_temp_4 = Tools::toNibbles(i_temp_2 - i_temp_1);

            ui->step2Label->setText(QString("1: <tt>(X − 0x%1)").arg(s_temp_1));
            ui->step2DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step2DisplayLabel->setText(
                        QString("<tt>%1<br>"
                                "- %2<hr>"
                                "= %3<br></tt>")
                        .arg(s_temp_2, s_temp_3, s_temp_4));
            m_step++;
            break;
        case 2:            
            s_temp_1 = Tools::toNibbles((m_bitBand.baseAddress() - m_bitBand.addressSpaceStartAddress()) << 5);
            ui->step3DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step3Label->setText("2: << 5 to multiply by 32");
            ui->step3DisplayLabel->setText(QString("<tt>&lt;&lt; 5<hr>"
                                                   "= %1</tt>")
                                           .arg(s_temp_1));
            m_step++;
            break;
        case 3:
            i_temp_1 = (m_bitBand.baseAddress() - m_bitBand.addressSpaceStartAddress()) << 5;
            s_temp_1 = Tools::toHex(i_temp_1);
            ui->step4Label->setText("Convert back to Hexadecimal");
            ui->step4DisplayLabel->setAlignment(Qt::AlignRight);
            ui->step4DisplayLabel->setText(QString("<tt><hr> = <b>0x%1")
                                           .arg(s_temp_1));
            m_step++;
            break;
        case 4:
            i_temp_1   = m_bitBand.aliasStartAddress();
            s_temp_1   = Tools::toHex(i_temp_1);
            s_temp_2   = Tools::toHex(((m_bitBand.baseAddress() - m_bitBand.addressSpaceStartAddress()) << 5) + i_temp_1);
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
            s_temp_3 = Tools::toHex(m_bitBand.BitBand::aliasAddress());
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
