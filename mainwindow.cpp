#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cipher.h"

#include <QDebug>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <qdebug.h>
#include <qobjectdefs.h>
#include <qstringliteral.h>
#include <qstringview.h>
#include <random>
#include <sstream>
#include <string>

static std::random_device RND;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_r1PolyGenButton_clicked()
{
    ui->r1PolyLineEdit->setText(QString::fromStdString(hexGenerate(ui->r1LenghtSpinBox->value())));
}


void MainWindow::on_r2PolyGenButton_clicked()
{
    ui->r2PolyLineEdit->setText(QString::fromStdString(hexGenerate(ui->r2LenghtSpinBox->value())));
}


void MainWindow::on_r1SeedGenButton_clicked()
{
    ui->r1SeedLineEdit->setText(QString::fromStdString(hexGenerate(ui->r1LenghtSpinBox->value())));
}


void MainWindow::on_r2SeedGenButton_clicked()
{
    ui->r2SeedLineEdit->setText(QString::fromStdString(hexGenerate(ui->r2LenghtSpinBox->value())));
}


void MainWindow::on_encryptButton_clicked()
{
    Cipher cipher(
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = ui->r1SeedLineEdit->text().toInt(nullptr, 16);
    uint64_t k2 = ui->r2SeedLineEdit->text().toInt(nullptr, 16);

    QByteArray bytes = ui->hexPlainTextBrowser->toPlainText().toUtf8();

    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');

    for (size_t i = 0; i < bytes.size(); ++i) {
        //cipher.EncByte(text[i], k1, k2);
        //ss << std::setw(2) << static_cast<int>(bytes[i]);
    }
    ss << bytes.data();
    ui->ciperTextBrowser->setText(QString::fromStdString(ss.str()));
}


void MainWindow::on_decipherButton_clicked()
{
    Cipher cipher(
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = ui->r1SeedLineEdit->text().toInt(nullptr, 16);
    uint64_t k2 = ui->r2SeedLineEdit->text().toInt(nullptr, 16);

    QString hexText = ui->cipherTextEdit->toPlainText().toLower();

    //uint8_t* text = reinterpret_cast<uint8_t*>(bytes.data());

    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');

    //for (size_t i = 0; i < bytes.size(); ++i) {
    //    cipher.EncByte(text[i], k1, k2);
    //    ss << std::setw(2) << static_cast<int>(text[i]);
  //  }
//
    //ui->ciperTextBrowser->setText(QString::fromStdString(ss.str()));
}


void MainWindow::on_decryptButton_clicked()
{

}


void MainWindow::on_plainTextEdit_textChanged()
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');

    std::string plainText = ui->plainTextEdit->toPlainText().toStdString();

    for (std::string::const_iterator it = plainText.begin(); it != plainText.end(); ++it) {
        ss << std::setw(2) << static_cast<int>(*it);
    }

    ui->hexPlainTextBrowser->setPlainText(QString::fromStdString(ss.str()));
}


std::string MainWindow::hexGenerate(int bitLen)
{
    std::stringstream ss;
    std::uniform_int_distribution<uint64_t> uDist(0, (1ull << bitLen));
    ss << "0x" << std::hex << uDist(RND);
    return ss.str();
}


