#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("XOR Шифрование");

    ui->timeEdit_timer->setVisible(false);
    ui->pushButton_stop->setDisabled(true);

    ui->lineEdit_key->setMaxLength(16);//максимальное значение ключа 16 символов
    // только шестнадцатиричные значения:
    //QRegularExpression re("[0-9A-F]");
    QValidator *val = new QRegularExpressionValidator(QRegularExpression("[0-9A-F]{0,16}"),this);
    ui->lineEdit_key->setValidator(val);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// слоты
void MainWindow::on_pushButton_start_clicked()
{
    // обновляем переменную для остановки шифрования
    stopEncoding = false;

    // одноразовый запуск
    inputMask = ui->lineEdit_inputMask->text();

    if(workWithTimer == false){
        this->start();
    }

    // запуск с таймером
    if(workWithTimer == true){

        // отключаем на все время работы таймера
        ui->pushButton_start->setDisabled(true);
        ui->timeEdit_timer->setDisabled(true);
        ui->pushButton_stop->setDisabled(false);
        ui->checkBox->setDisabled(true);
        ui->checkBox_2->setDisabled(true);
        ui->pushButton_inputFilse->setDisabled(true);
        ui->pushButton_outputFiles->setDisabled(true);
        ui->lineEdit_input->setDisabled(true);
        ui->lineEdit_inputMask->setDisabled(true);
        ui->lineEdit_key->setDisabled(true);
        ui->lineEdit_output->setDisabled(true);
        ui->radioButton_single->setDisabled(true);
        ui->radioButton_timer->setDisabled(true);
        ui->radioButton_delOut->setDisabled(true);
        ui->radioButton_modOut->setDisabled(true);

        timer = new QTimer(this);
        timer->setInterval(msecTimer);
        start();
        connect(timer,SIGNAL(timeout()),this,SLOT(start_with_timer()));
    }
}

void MainWindow::on_pushButton_inputFilse_clicked()
{
    inputStartDir = QFileDialog::getExistingDirectory();
    ui->lineEdit_input->setText(inputStartDir);
}

void MainWindow::on_pushButton_outputFiles_clicked()
{
    outputDir = QFileDialog::getExistingDirectory();
    ui->lineEdit_output->setText(outputDir);
}

void MainWindow::on_lineEdit_inputMask_textChanged(const QString &arg1)
{
    inputMask = arg1;
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    deleteInput = checked;
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    searchSubfold = checked;
}

void MainWindow::on_lineEdit_input_textChanged(const QString &arg1)
{
    inputStartDir = arg1;
}

void MainWindow::on_lineEdit_output_textChanged(const QString &arg1)
{
    outputDir = arg1;
}

void MainWindow::on_lineEdit_key_textChanged(const QString &arg1)
{
    key = arg1;
}

void MainWindow::on_timeEdit_timer_timeChanged(const QTime &time)
{
    msecTimer = time.msecsSinceStartOfDay();
}

void MainWindow::on_radioButton_timer_toggled(bool checked)
{
    workWithTimer = checked;
}

void MainWindow::on_radioButton_delOut_toggled(bool checked)
{
    deleteOutput = checked;
}

void MainWindow::on_pushButton_stop_clicked()
{
   // остановка шифрования:
    stopEncoding = true;
    if(workWithTimer == true){

        timer->stop();

        ui->pushButton_start->setDisabled(false);
        ui->timeEdit_timer->setDisabled(false);
        ui->pushButton_stop->setDisabled(true);
        ui->checkBox->setDisabled(false);
        ui->checkBox_2->setDisabled(false);
        ui->pushButton_inputFilse->setDisabled(false);
        ui->pushButton_outputFiles->setDisabled(false);
        ui->lineEdit_input->setDisabled(false);
        ui->lineEdit_inputMask->setDisabled(false);
        ui->lineEdit_key->setDisabled(false);
        ui->lineEdit_output->setDisabled(false);
        ui->radioButton_single->setDisabled(false);
        ui->radioButton_timer->setDisabled(false);
        ui->radioButton_delOut->setDisabled(false);
        ui->radioButton_modOut->setDisabled(false);
    }
}

void MainWindow::start_with_timer(){
    start();
}

// методы
void MainWindow::start(){

    if(workWithTimer == false){
        ui->pushButton_start->setDisabled(true);
        ui->timeEdit_timer->setDisabled(true);
        ui->pushButton_stop->setDisabled(false);
        ui->checkBox->setDisabled(true);
        ui->checkBox_2->setDisabled(true);
        ui->pushButton_inputFilse->setDisabled(true);
        ui->pushButton_outputFiles->setDisabled(true);
        ui->lineEdit_input->setDisabled(true);
        ui->lineEdit_inputMask->setDisabled(true);
        ui->lineEdit_key->setDisabled(true);
        ui->lineEdit_output->setDisabled(true);
        ui->radioButton_single->setDisabled(true);
        ui->radioButton_timer->setDisabled(true);
        ui->radioButton_delOut->setDisabled(true);
        ui->radioButton_modOut->setDisabled(true);
    }

    // приостанавливаем рабоут таймера на время выполнения
    if(workWithTimer == true){
        timer->stop();
    }

    QStringList mask;
    mask << inputMask;

    changeCount = 0;
    skipCount = 0;

    // кодировка файлов
    this->encodeFiles(inputStartDir,mask);

    // поиск по вложенным папкам, если надо
    if(searchSubfold == true){
        QDirIterator subfoldIter(inputStartDir,(QDir::Dirs | QDir::NoDotAndDotDot), QDirIterator::Subdirectories);
        while (subfoldIter.hasNext()) {
            this->encodeFiles(subfoldIter.next(),mask);
        }
    }

    // выводим информации в статус бар
    std::string str = "Зашифровано файлов: " + std::to_string(changeCount) + "          Пропущено файлов: " + std::to_string(skipCount);
    QString countFiles = QString::fromStdString(str);
    if(successfullyEncode == true){
         ui->statusBar->showMessage("Шифрование успешно завершено.          " + countFiles);
    } else {
        ui->statusBar->showMessage("Шифрование прервано.            " + countFiles);
    }

    if(workWithTimer == false){
        ui->pushButton_start->setDisabled(false);
        ui->timeEdit_timer->setDisabled(false);
        ui->pushButton_stop->setDisabled(true);
        ui->checkBox->setDisabled(false);
        ui->checkBox_2->setDisabled(false);
        ui->pushButton_inputFilse->setDisabled(false);
        ui->pushButton_outputFiles->setDisabled(false);
        ui->lineEdit_input->setDisabled(false);
        ui->lineEdit_inputMask->setDisabled(false);
        ui->lineEdit_key->setDisabled(false);
        ui->lineEdit_output->setDisabled(false);
        ui->radioButton_single->setDisabled(false);
        ui->radioButton_timer->setDisabled(false);
        ui->radioButton_delOut->setDisabled(false);
        ui->radioButton_modOut->setDisabled(false);
    }

}

// шифрование файлов в директории
void MainWindow::encodeFiles(QString inputDir, QStringList mask){

    ui->statusBar->showMessage("Шифрование...");

    // при совпадении директории с директорией выходных файлов ничего не делаем
    if(QString::compare(inputDir,outputDir)==0){
        return;
    }

    QDirIterator dirIter(inputDir,mask,QDir::Files);

    //проход по всем файлам
    while(dirIter.hasNext()){

        QApplication::processEvents();

        if(stopEncoding == true){
            successfullyEncode = false;
            return;
        }

        dirIter.next();

        QFile inputFile(dirIter.filePath());

        // работаем с файлом только если он открывается в режиме ReadWrite
        if(inputFile.open(QIODevice::ReadWrite)){

        QString outFileName = outputDir + "/" +dirIter.fileName();
        QFile outputFile(outFileName);
            if(outputFile.exists()){

                if(deleteOutput == false){
                    int n = 1;
                    while(true){
                        std::string newFileName = outputDir.toStdString() + "/" +dirIter.fileName().toStdString();
                        int index = newFileName.find_last_of(".");

                            std::string insert = " (" + std::to_string(n) + ")";

                            if(index == -1){
                            newFileName = outputDir.toStdString() + "/" +dirIter.fileName().toStdString() + insert;
                        } else {
                            newFileName.insert(index,insert);
                        }

                        outFileName = QString::fromStdString(newFileName);
                        outputFile.setFileName(outFileName);
                        if(outputFile.exists()){
                            n++;
                            continue;
                        } else {
                            break;
                        }
                    }
                }
            }

            outputFile.open(QIODevice::WriteOnly);

            // если размер файла меньше 10мб читаем и пишем полностью, иначе по 5мб блоками
            if(inputFile.size() <= 1024*1024*10){
                QByteArray array = inputFile.readAll();
                QByteArray outputArray = xorEncode(array,this->key);
                outputFile.write(outputArray);
                changeCount++;
            }else {
                char a[1024*1024*5];
                while (!inputFile.atEnd()) {
                    QApplication::processEvents();
                    if(stopEncoding == true){
                        successfullyEncode = false;
                        //break;
                        return ;
                    }
                    int nBlockSize = inputFile.read(a,sizeof(a));
                    QByteArray outputArray = xorEncode(QByteArray::fromRawData(a,nBlockSize),this->key);
                    outputFile.write(outputArray,nBlockSize);
                }
                changeCount++;
            }


            // если нужно удаляем входной файл
            if(this->deleteInput == true){
                inputFile.remove();
            }
                inputFile.close();
                outputFile.close();
        } else{
            skipCount++;
        }
   }

   ui->statusBar->clearMessage();
   successfullyEncode = true;
   // запускаем таймер после работы программы
   if(workWithTimer == true){
       timer->start();
   }
}

// xor шифрование файлов
QByteArray MainWindow::xorEncode(QByteArray array, QString key){

    // если ключ пустой, то просто копируем файл
    if(key.isEmpty()){
        return array;
    }

    // если ключ меньше 16 символов, то заполняем слева нулями
    if(key.length()<16 && !key.isEmpty()){
        while(key.length()<16){
            key.prepend("0");
        }
    }

    QByteArray keyAr = QByteArray::fromHex(key.toLocal8Bit());
    QByteArray tmp;

    for(int i=0;i<array.size();++i){
        tmp[i] = array[i]^keyAr[i % keyAr.size()];
    }

    return tmp;
}
