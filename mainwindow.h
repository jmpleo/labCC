#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>

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

    void on_decryptButton_clicked();

    void on_plainTextEdit_textChanged();

private:
    std::string hexGenerate(int bitLen);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
