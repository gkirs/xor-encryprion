#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QApplication>
#include <QFileDialog>
#include <QDirIterator>
#include <QByteArray>
#include <QIODevice>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QRegularExpression>
#include <QValidator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString inputStartDir, outputDir,
            inputMask, key;
    bool deleteInput = false, searchSubfold=false,
         workWithTimer = false, deleteOutput = true,
         stopEncoding = false, successfullyEncode;
    int msecTimer;
    int changeCount, skipCount;
    QStringList fileList;
    QTimer *timer;

    void start();
    void encodeFiles(QString dirIter,QStringList mask);

    QByteArray xorEncode(QByteArray array, QString key);

private slots:

    void on_pushButton_inputFilse_clicked();
    void on_lineEdit_inputMask_textChanged(const QString &arg1);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_pushButton_outputFiles_clicked();
    void on_pushButton_start_clicked();
    void on_lineEdit_input_textChanged(const QString &arg1);
    void on_lineEdit_output_textChanged(const QString &arg1);
    void on_lineEdit_key_textChanged(const QString &arg1);
    void on_timeEdit_timer_timeChanged(const QTime &time);
    void on_radioButton_timer_toggled(bool checked);
    void on_pushButton_stop_clicked();
    void on_radioButton_delOut_toggled(bool checked);
    void start_with_timer();
};

#endif // MAINWINDOW_H
