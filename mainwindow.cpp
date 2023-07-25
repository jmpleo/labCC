#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cipher.h"

#include <QDebug>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <qchar.h>
#include <qdebug.h>
#include <qglobal.h>
#include <qobjectdefs.h>
#include <qstringliteral.h>
#include <qstringview.h>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

using cipher::Cipher;
using cipher::Bytes;
using cipher::Byte;

static std::random_device RND;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_r1PolyGenButton_clicked();
    on_r2PolyGenButton_clicked();
    on_r1SeedGenButton_clicked();
    on_r2SeedGenButton_clicked();
    ui->plainTextEdit->insertPlainText("Secret Message");
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
    Cipher cip(
        ui->r1PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = ui->r1SeedLineEdit->text().toUInt(nullptr, 16);
    uint64_t k2 = ui->r2SeedLineEdit->text().toUInt(nullptr, 16);

    Bytes bytes = Cipher::fromHex(
            ui->hexPlainTextBrowser->toPlainText().toStdString());

    ui->cipherTextBrowser->setText(
            QString::fromStdString(
                Cipher::toHex( cip.Enc(bytes, k1, k2) )));
}



void MainWindow::on_decipherButton_clicked()
{
    Cipher cip(
        ui->r1PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r2PolyLineEdit->text().toUInt(nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = ui->decR1SeedLineEdit->text().toUInt(nullptr, 16);
    uint64_t k2 = ui->decR2SeedLineEdit->text().toUInt(nullptr, 16);

    Bytes bytes = Cipher::fromHex(
            ui->cipherTextEdit->toPlainText().toStdString());

    bytes = cip.Dec(bytes, k1, k2);

    ui->hexPlainTextBrowser_2->setPlainText(
            QString::fromStdString(
                Cipher::toHex( bytes )));

    ui->plainTextBrowser->setPlainText(
            QString::fromStdString(std::string(bytes.begin(), bytes.end())));

}


void MainWindow::on_plainTextEdit_textChanged()
{
    ui->hexPlainTextBrowser->setPlainText(
            QString::fromStdString(
                toHex(ui->plainTextEdit->toPlainText().toStdString())));
}

void MainWindow::on_r1SeedLineEdit_textChanged(const QString &arg1)
{
    ui->decR1SeedLineEdit->setText(arg1);
}


void MainWindow::on_r2SeedLineEdit_textChanged(const QString &arg1)
{
    ui->decR2SeedLineEdit->setText(arg1);
}


void MainWindow::on_cipherTextBrowser_textChanged()
{
    ui->cipherTextEdit->setText(ui->cipherTextBrowser->toPlainText());
}


void MainWindow::on_possibleGammaButton_clicked()
{
    int k = (ui->lcmLenghtSpinBox->value() / 10)     // 43 / 10 = 4
        + static_cast<int>(ui->lcmLenghtSpinBox->value() % 10 != 0); // 43 % 10 != 0 -> 4+1

    QByteArray cipherBytes = QByteArray::fromHex(ui->analysisCipherTextEdit->toPlainText().mid(2).toUtf8());
    QByteArray plainBytes = QByteArray::fromHex(ui->analysisCipherTextEdit->toPlainText().mid(2).toUtf8());

    auto allPermG = Cipher::computeG();
    std::vector<std::vector<uint64_t>> possibleGammas(k);

    for (int i = 0; i < k; ++i) {
        for (uint64_t gammas = 0; gammas < allPermG.size(); ++gammas) {
            if (allPermG[gammas][ plainBytes[i] ] == cipherBytes[i]) {
                possibleGammas[i].push_back(gammas);
            }
        }
    }

    QString s;
    for (auto &forSymbol : possibleGammas) {
        for (auto &gammas : forSymbol) {
            std::stringstream ss;
            ss << "0x" << std::hex << std::setfill('0');
            ss << std::setw(2) << gammas;
            s.append(QString::fromStdString(ss.str()));
            s.push_back('\t');
        }
        s.push_back('\n');
    }
    ui->possibleGammaBrowser->setText(s);
}

std::string MainWindow::toHex(const std::string &plainText)
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');

    for (auto ch : plainText) {
        ss << std::setw(2)
           << static_cast <unsigned int> (
                static_cast<unsigned char> (ch));
    }
    return ss.str();
}

std::string MainWindow::hexGenerate(int bitLen)
{
    std::stringstream ss;
    std::uniform_int_distribution<uint64_t> uDist(0, (1ull << bitLen)-1);
    ss << "0x" << std::hex; //<< std::setfill('0');
    //ss << std::setw(bitLen / 4) << uDist(RND);
    ss << uDist(RND);
    return ss.str();
}




/*
std::string MainWindow::toHex(Cipher::Bytes other)
{
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0');

    for (auto ch : other) {
        BIN(std::cout << "0b", ch) << " : "
         << std::hex << static_cast<unsigned int> ( ch ) << std::endl;

        ss << std::setw(2)
           << static_cast<unsigned int> ( ch );
    }
    return ss.str();
}
Cipher::Bytes MainWindow::fromHex(std::string hexStr)
{
    std::stringstream ss;
    Cipher::Bytes bytes;
    for (int i = 2; i < hexStr.size(); i += 2) {
        uint8_t byte = std::stoi(hexStr.substr(i, 2), nullptr, 16);
        BIN(std::cout << "0b", byte) << " : " << hexStr.substr(i, 2) << std::endl;
        bytes.push_back(byte);
    }
    return bytes;
}

*/

