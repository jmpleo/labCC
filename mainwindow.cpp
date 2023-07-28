#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cipher.h"

#include <QDebug>
#include <algorithm>
#include <complex>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <qchar.h>
#include <qdebug.h>
#include <qglobal.h>
#include <qobjectdefs.h>
#include <qregexp.h>
#include <qstringliteral.h>
#include <qstringview.h>
#include <qvalidator.h>
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

    QRegExp hexRegExp("^0x[0-9a-fA-F]*$");
    ui->r1PolyLineEdit->setValidator(new QRegExpValidator(hexRegExp, this));
    ui->r2PolyLineEdit->setValidator(new QRegExpValidator(hexRegExp, this));
    ui->r1SeedLineEdit->setValidator(new QRegExpValidator(hexRegExp, this));
    ui->r2SeedLineEdit->setValidator(new QRegExpValidator(hexRegExp, this));

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
    ui->r1PolyLineEdit->setText(QString::fromStdString(
                hexGenerate(ui->r1LenghtSpinBox->value())));
}


void MainWindow::on_r2PolyGenButton_clicked()
{
    ui->r2PolyLineEdit->setText(QString::fromStdString(
                hexGenerate(ui->r2LenghtSpinBox->value())));
}


void MainWindow::on_r1SeedGenButton_clicked()
{
    ui->r1SeedLineEdit->setText(QString::fromStdString(
                hexGenerate(ui->r1LenghtSpinBox->value())));
}


void MainWindow::on_r2SeedGenButton_clicked()
{
    ui->r2SeedLineEdit->setText(QString::fromStdString(
                hexGenerate(ui->r2LenghtSpinBox->value())));
}


void MainWindow::on_encryptButton_clicked()
{
    Cipher cip(
        std::stoull(ui->r1PolyLineEdit->text().toStdString(), nullptr, 16),
        std::stoull(ui->r2PolyLineEdit->text().toStdString(), nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = std::stoull(ui->r1SeedLineEdit->text().toStdString(), nullptr, 16);
    uint64_t k2 = std::stoull(ui->r2SeedLineEdit->text().toStdString(), nullptr, 16);

    Bytes bytes = Cipher::fromHex(
            ui->hexPlainTextBrowser->toPlainText().toStdString());

    ui->cipherTextBrowser->setText(
            QString::fromStdString(
                Cipher::toHex( cip.Enc(bytes, k1, k2) )));
}



void MainWindow::on_decipherButton_clicked()
{
    Cipher cip(
        std::stoull(ui->r1PolyLineEdit->text().toStdString(), nullptr, 16),
        std::stoull(ui->r2PolyLineEdit->text().toStdString(), nullptr, 16),
        ui->r1LenghtSpinBox->value(),
        ui->r2LenghtSpinBox->value()
    );

    uint64_t k1 = std::stoull(ui->decR1SeedLineEdit->text().toStdString(), nullptr, 16);
    uint64_t k2 = std::stoull(ui->decR2SeedLineEdit->text().toStdString(), nullptr, 16);

    Bytes bytes = Cipher::fromHex(
            ui->cipherTextEdit->toPlainText().toStdString());

    bytes = cip.Dec(bytes, k1, k2);

    ui->hexPlainTextBrowser_2->setPlainText(
            QString::fromStdString(Cipher::toHex( bytes )));

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
    if ((arg1.size() - 2) * 4 > ui->r1LenghtSpinBox->value()) {
        ui->r1SeedLineEdit->setText(
                "0x" + arg1.right((ui->r1LenghtSpinBox->value() + 3) / 4));
    }

    ui->decR1SeedLineEdit->setText(ui->r1SeedLineEdit->text());
}


void MainWindow::on_r2SeedLineEdit_textChanged(const QString &arg1)
{
    if ((arg1.size() - 2) * 4 > ui->r2LenghtSpinBox->value()) {
        ui->r2SeedLineEdit->setText(
                "0x" + arg1.right((ui->r2LenghtSpinBox->value() + 3) / 4));
    }

    ui->decR2SeedLineEdit->setText(ui->r2SeedLineEdit->text());
}


void MainWindow::on_cipherTextBrowser_textChanged()
{
    ui->cipherTextEdit->setText(
            ui->cipherTextBrowser->toPlainText());
}


void MainWindow::on_possibleGammaButton_clicked()
{
    int i, k = (ui->lcmLenghtSpinBox->value() + 9) / 10;

    ui->possibleGammaBrowser->clear();

    Bytes cipher = Cipher::fromHex(
            ui->analysisCipherTextEdit->toPlainText().toStdString());

    Bytes plain = Cipher::fromHex(
            ui->analysisPlainTextEdit->toPlainText().toStdString());

    if (plain.size() < k || cipher.size() < k) {
        return;
    }

    auto allPermG = Cipher::computeG();
    std::vector<std::vector<uint64_t>> possibleGammas(k);

    for (i = 0; i < k; ++i) {
        for (uint64_t gammas = 0; gammas < allPermG.size(); ++gammas) {
            if (allPermG[gammas][ plain[i] ] == cipher[i]) {
                possibleGammas[i].push_back(gammas);
            }
        }
    }

    //i = 0;
    //std::string hexText = Cipher::toHex(
    //        Bytes(plain.begin(), plain.begin() + k)).substr(2);
    for (auto &forSymbol : possibleGammas) {
        std::stringstream ss;
        for (auto &gammas : forSymbol) {
            ss << "0x" << std::hex << std::setfill('0') << std::setw(2)
               << gammas << '\t';
        }
        //ss << "для " << "0x" << hexText.substr(i, 2);
        //ss << " (" << plain[i++] << ")\n";
        ss << '\n';
        ui->possibleGammaBrowser->insertPlainText(QString::fromStdString(ss.str()));
    }
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
    ss << "0x" << std::hex << uDist(RND);
    return ss.str();
}

std::string MainWindow::toPrettyPoly(uint64_t bitPoly, int bitLen)
{
    std::stringstream prettyPoly;
    char x = 'x', pow = '^';
    int deg = bitLen;

    prettyPoly << x << pow << deg;
    while (--deg, deg) {
        if ((bitPoly >>deg) & 1) {
            prettyPoly << " + " << x << pow << deg;
        }
    }
    if ((bitPoly & 1) == 1) {
        prettyPoly << " + 1";
    }
    return prettyPoly.str();
}

void MainWindow::on_r1PolyLineEdit_textChanged(const QString &arg1)
{
    if ((arg1.size() - 2) * 4 > ui->r1LenghtSpinBox->value()) {
        ui->r1PolyLineEdit->setText(
                "0x" + arg1.right((ui->r1LenghtSpinBox->value() + 3) / 4));
    }

    if (ui->r1PolyLineEdit->hasAcceptableInput()) {
        ui->poly1TextBrowser->setText(QString::fromStdString(
            toPrettyPoly(std::stoull(ui->r1PolyLineEdit->text().toStdString(), nullptr, 16),
                ui->r1LenghtSpinBox->value())));
    }
}


void MainWindow::on_r2PolyLineEdit_textChanged(const QString &arg1)
{
    if ((arg1.size() - 2) * 4 > ui->r2LenghtSpinBox->value()) {
        ui->r2PolyLineEdit->setText(
                "0x" + arg1.right((ui->r2LenghtSpinBox->value() + 3) / 4));
    }

    if (ui->r2PolyLineEdit->hasAcceptableInput()) {
        ui->poly2TextBrowser->setText(QString::fromStdString(
            toPrettyPoly(std::stoull(ui->r2PolyLineEdit->text().toStdString(), nullptr, 16),
                ui->r2LenghtSpinBox->value())));
    }
}

void MainWindow::on_decR1SeedLineEdit_textChanged(const QString &arg1)
{
    if ((arg1.size() - 2) * 4 > ui->r1LenghtSpinBox->value()) {
        ui->decR2SeedLineEdit->setText(
                "0x" + arg1.right((ui->r1LenghtSpinBox->value() + 3) / 4));
    }
}


void MainWindow::on_decR2SeedLineEdit_textChanged(const QString &arg1)
{
    if ((arg1.size() - 2) * 4 > ui->r2LenghtSpinBox->value()) {
        ui->decR2SeedLineEdit->setText(
                "0x" + arg1.right((ui->r2LenghtSpinBox->value() + 3) / 4));
    }
}

