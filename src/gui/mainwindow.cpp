#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "dectoieee.h"
#include "ieeetodec.h"
#include "bitbandwidget.h"
#include "intconverterwidget.h"
#include "floatconverterwidget.h"
#include "src/exercisegenerator.h"
#include "src/tools.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectSignals();

    //auto decToHex = new DecToIeee(this);
    //ui->tabWidget->addTab(decToHex, "Dec->Hex");

    //auto hexToDec = new IeeeToDec(this);
    //ui->tabWidget->addTab(hexToDec, "Hex->Dec");

    auto intConverterWidget = new IntConverterWidget(this);
    ui->tabWidget->addTab(intConverterWidget, "Integer conversions");

    auto floatConverterWidget = new FloatConverterWidget(this);
    ui->tabWidget->addTab(floatConverterWidget, "Floating Point Conversion");
    ui->tabWidget->setCurrentWidget(floatConverterWidget);

    auto bitBandWidget = new BitBandWidget(this);
    ui->tabWidget->addTab(bitBandWidget, "Bit Banding");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectSignals()
{
    QObject::connect(ui->aboutPushButton,   &QPushButton::clicked,
                     this,                  &MainWindow::on_aboutClicked);
}

void MainWindow::on_aboutClicked()
{
    QMessageBox message_box(this);
    QString text = QMessageBox::tr(

                "<h3><center>About</center></h3>"
                "<br>"
                "&#169; 2022 Felix Turowsky"
                "<br>"

                "<p>This program is intended as a tool for computer science students who want to practice the conversion of decimal base 10 numbers to their IEEE-754 single precision"
                "floating point representation. It is exclusively designed for educational purposes. If you encounter any bugs, please do report them on <a href=\"http://%1/\">Github</a> </p>"

                "<p>This program is free software: you can redistribute it and/or modify "
                "it under the terms of the GNU General Public License as published by "
                "the Free Software Foundation, either version 3 of the License, or "
                "(at your option) any later version.</p>"

                "<p>This program is distributed in the hope that it will be useful, "
                "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                "GNU General Public License for more details.</p> "

                "<p>You should have received a copy of the GNU General Public License "
                "along with this program.  If not, "
                "please click <a href=\"https://%2\">here</a>.</p>"

                "<p>This program uses <a href=\"http://%3/\">Qt</a> version %4."
                ).arg(
                QStringLiteral("github.com/fiffty-50/ieee754-practice"),
                QStringLiteral("gnu.org/licenses/"),
                QStringLiteral("qt.io"),
                QT_VERSION_STR);
    message_box.setText(text);
    message_box.exec();
}

