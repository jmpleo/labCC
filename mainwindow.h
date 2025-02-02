#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>
#include <qvalidator.h>
#include "cipher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_r1PolyGenButton_clicked();

    void on_r2PolyGenButton_clicked();

    void on_r1SeedGenButton_clicked();

    void on_r2SeedGenButton_clicked();

    void on_encryptButton_clicked();

    void on_decipherButton_clicked();

    void on_plainTextEdit_textChanged();

    void on_toolTextEdit_textChanged();

    void on_toolHexEdit_textChanged();

    void on_r1SeedLineEdit_textChanged(const QString &arg1);

    void on_r2SeedLineEdit_textChanged(const QString &arg1);

    void on_cipherTextBrowser_textChanged();

    void on_possibleGammaButton_clicked();

    void on_r1PolyLineEdit_textChanged(const QString &arg1);

    void on_r2PolyLineEdit_textChanged(const QString &arg1);

    void on_decR1SeedLineEdit_textChanged(const QString &arg1);

    void on_decR2SeedLineEdit_textChanged(const QString &arg1);

private:
    static std::string toPrettyPoly(uint64_t bitPoly, int bitLen);
    static std::string hexGenerate(int bitLen);
    static std::string toHex(const std::string &plainText);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
