#include "intconverterwidget.h"
#include "qmessagebox.h"
#include "src/debug.h"
#include "ui_intconverterwidget.h"

IntConverterWidget::IntConverterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntConverterWidget)
{
    ui->setupUi(this);
    clearDisplayLabels();
    QObject::connect(ui->pushButton,        &QPushButton::clicked,
                     this,                  &IntConverterWidget::conversionRequested);
    m_LABEL_MAP = {
        {0, ui->binaryDisplayLabel},
        {1, ui->decimalDisplayLabel},
        {2, ui->hexDisplayLabel},
    };
}

IntConverterWidget::~IntConverterWidget()
{
    delete ui;
}

void IntConverterWidget::conversionRequested()
{
    switch (ui->comboBox->currentIndex()) {
    case 0: // Input is binary
        m_conversions = convertNumber(Binary);
        break;
    case 1: // Input is decimal
        m_conversions = convertNumber(Decimal);
        break;
    case 2: // Input is hexadecimal
        m_conversions = convertNumber(Hexadecimal);
        break;
    default:
        // interpret as decimal
        m_conversions = convertNumber(Decimal);
        break;
    }

    if (!m_conversions.isEmpty())
        for (int i = 0; i < m_conversions.length(); i++)
            m_LABEL_MAP[i]->setText(m_conversions[i]);
    else {
        QMessageBox mb(this);
        mb.setText("Conversion unsuccessful. Check Input Format.");
        mb.exec();
        clearDisplayLabels();
    }
}

QStringList IntConverterWidget::convertNumber(InputFormat format)
{
    bool ok;
    int base;
    switch (format) {
    case InputFormat::Binary:
        base = 2;
        break;
    case InputFormat::Decimal:
        base = 10;
        break;
    case InputFormat::Hexadecimal:
        base = 16;
        break;
    default:
        base = 10;
        break;
    }

    long long number = ui->lineEdit->text().toLongLong(&ok, base);
    if (!ok)
        return {};

    // Basic Conversions
    // Split Binary into nibbles
    QString binary_string = QString::number(number, 2);
    QString nibbles;
    if (binary_string.length() > 4) {
        //int steps = binary_string.length() / 4;
        //int leftovers = binary_string.length() % 4;
        //for (int i = 0; i < steps; i++)
        //    nibbles.append(binary_string.mid(4 * i, 4) + QLatin1Char(' '));
        //DEB << "Leftover:" << leftovers;
        //nibbles.append(binary_string.last(leftovers) + QLatin1Char(' '));
        int steps = binary_string.length() / 4;
        int leftovers = binary_string.length() % 4;
        for (int i = 0; i < steps; i++) {
            nibbles.prepend(binary_string.right(4) + QLatin1Char(' '));
            binary_string.chop(4);
        }

        DEB << "Leftover:" << leftovers;
        nibbles.prepend(binary_string + QLatin1Char(' '));
    } else
        nibbles = binary_string;

    QStringList conversions = {
        QString("<tt>%1").arg(nibbles),
        QString("<tt>%1").arg(QLocale().toString(number)),
        QString("<tt>0x%1").arg(QString::number(number, 16).toUpper()),
    };

    return conversions;
}

void IntConverterWidget::clearDisplayLabels()
{
    const QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (const auto &label : labels)
        if(label->objectName().contains(QLatin1String("Display")))
            label->setText(QString());
}
