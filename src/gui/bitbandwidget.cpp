#include "bitbandwidget.h"
#include "ui_bitbandwidget.h"
#include "src/exercisegenerator.h"

BitBandWidget::BitBandWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitBandWidget)
{
    ui->setupUi(this);
    connectSignals();

    const auto label_map = QMap<int, QPair<QLabel*, QLabel*>> {
        {1, {ui->stepOneLabel,   ui->stepOneDisplayLabel}},
        {2, {ui->stepTwoLabel,   ui->stepTwoDisplayLabel}},
        {3, {ui->stepThreeLabel, ui->stepThreeDisplayLabel}},
        {4, {ui->stepFourLabel,  ui->stepFourDisplayLabel}},
        {5, {ui->stepFiveLabel,  ui->stepFiveDisplayLabel}},
        {6, {ui->stepSixLabel,   ui->stepSixDisplayLabel}},
        {7, {ui->stepSevenLabel, ui->stepSevenDisplayLabel}},
        {8, {ui->stepEightLabel, ui->stepEightDisplayLabel}},
    };
    m_label_map = &label_map;
    clearLabels();
}

BitBandWidget::~BitBandWidget()
{
    delete ui;
}

void BitBandWidget::clearLabels()
{
    for (auto label_pair : *m_label_map) {
        label_pair.second->setText(QString());
    }
}

void BitBandWidget::on_buttonGroupClicked(QAbstractButton *button)
{
    if (button == ui->bitposButton)
        m_direction = Direction::FromBitPos;
    else
        m_direction = Direction::FromBBA;
}

// Signals and Slots

void BitBandWidget::connectSignals()
{
    QObject::connect(ui->generatePushButton, &QPushButton::clicked,
                     this,                   &BitBandWidget::on_generateExample);
    QObject::connect(ui->nextPushButton,     &QPushButton::clicked,
                     this,                   &BitBandWidget::on_nextStepRequested);
    QObject::connect(ui->resultPushButton,   &QPushButton::clicked,
                     this,                   &BitBandWidget::on_resultRequested);
    QObject::connect(ui->buttonGroup,        &QButtonGroup::buttonClicked,
                     this,                   &BitBandWidget::on_buttonGroupClicked);
}

void BitBandWidget::on_generateExample()
{
    m_step = 0;
    Difficulty difficulty = Difficulty(ui->difficultyComboBox->currentIndex());
    m_bitBand = ExerciseGenerator::generateBitBandExercise(difficulty);

    ui->nextPushButton->setEnabled(true);
    ui->resultPushButton->setEnabled(true);
    clearLabels();

    DEB << "Generated Example:";
    DEB << "BitPos: "  << QString::number(m_bitBand.bitPos(), 10);
    DEB << "BB-Addr: "  << QString::number(m_bitBand.bbAddress(), 16);
    DEB << "BBA-Addr:"  << QString::number(m_bitBand.bbAliasAddress(), 16);
}

void BitBandWidget::on_nextStepRequested()
{
    switch (m_step) {
    case 0:
        qDebug() << "Step 1 - Convert the decimal base 10 number to its absolute base 2 representation";

        m_step++;
        break;
    case 1:
        qDebug() << "Step 2 - Determine the bias";

        m_step++;
        break;
    case 2:
        qDebug() << "Step 3 - Pad the mantissa";

        m_step++;
        break;
    case 3:
        qDebug() << "Step 4 - Determine the biased exponent";

        m_step++;
        break;
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
    default:
        break;
    }
}

void BitBandWidget::on_resultRequested()
{

}
