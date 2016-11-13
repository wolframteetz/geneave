#include "serial.h"
#include "ui_serial.h"
#include <qdebug>
#include <QtSerialPort/QSerialPort>
#include <QFileDialog>
#include <QStandardItem>
#include <QStandardItemModel>

Serial::Serial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);

    serial = new QSerialPort(this);
    serial->setPortName(ui->comboBoxSerialPort->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setDataBits(QSerialPort::Data8);
    serialPortReader = new SerialPortReader(serial, ui->textBrowserStatus);
    serial->open(QIODevice::ReadWrite);

}

Serial::~Serial()
{
    serial->close();
    delete ui;
}

void Serial::on_pushButtonExecute_clicked()
{
    ui->pushButtonExecute->setEnabled(false);
    ui->textBrowserStatus->setText("Weave all rows.");
    for (int i=0; i<ui->spinBoxTimesExecute->value(); i++) {
        ui->textBrowserStatus->append(QString("Run ").append(QString::number(i+1)).append(" of ").append(QString::number(ui->spinBoxTimesExecute->value())));
        QString s;
        for (int y=0; y<ui->tableWidgetPattern->rowCount(); y++)
        {
            s.clear();
            for (int x=0; x< ui->tableWidgetPattern->columnCount(); x++)
            {

                int v = ui->tableWidgetPattern->item(y,x)->text().toInt();
                s.append(ui->tableWidgetAmounts->item(v,x)->text());
            }

            ui->textBrowserStatus->append(QString("<span style=\"color:#008800;\">").append(s).append(QString("</span>")));

            if (serial->isOpen())
            {
                QByteArray dayArray;
                dayArray.append(s);
                dayArray.append("X");
                serial->write(dayArray);
                serial->waitForBytesWritten(-1);
                ui->textBrowserStatus->append(QString("Written."));
            } else {
                ui->textBrowserStatus->append(QString("Serial port is not open.").append(ui->comboBoxSerialPort->currentText()).append(QString(". Not written.")));
            }
        }
    }
    ui->textBrowserStatus->append(QString("Command finished."));
    ui->pushButtonExecute->setEnabled(true);
}

void Serial::on_pushButtonWeaveSelectedRows_clicked()
{
    ui->pushButtonWeaveSelectedRows->setEnabled(false);
    ui->textBrowserStatus->setText("Weave selected rows.");

    QModelIndexList selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.isEmpty()) selection = ui->tableWidgetPattern->selectionModel()->selectedIndexes();

    for (int i=0; i<ui->spinBoxTimesExecute->value(); i++) {
        ui->textBrowserStatus->append(QString("Run ").append(QString::number(i+1)).append(" of ").append(QString::number(ui->spinBoxTimesExecute->value())));
        QString s;
        // Multiple rows can be selected
        for(int i=0; i< selection.count(); i++)
        {
            int y = selection.at(i).row();
            s.clear();
            for (int x=0; x< ui->tableWidgetPattern->columnCount(); x++)
            {

                int v = ui->tableWidgetPattern->item(y,x)->text().toInt();
                s.append(ui->tableWidgetAmounts->item(v,x)->text());
            }

            ui->textBrowserStatus->append(QString("<span style=\"color:#008800;\">").append(s).append(QString("</span>")));

            if (serial->isOpen())
            {
                QByteArray dayArray;
                dayArray.append(s);
                dayArray.append("X");
                serial->write(dayArray);
                serial->waitForBytesWritten(-1);
                ui->textBrowserStatus->append(QString("Written."));
            } else {
                ui->textBrowserStatus->append(QString("Serial port is not open.").append(ui->comboBoxSerialPort->currentText()).append(QString(". Not written.")));
            }
        }
    }
    ui->textBrowserStatus->append(QString("Command finished."));
    ui->pushButtonWeaveSelectedRows->setEnabled(true);
}

void Serial::on_pushButtonLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open CSV Pattern"), "", tr("CSV Files (*.csv)"));

    if (fileName.length()>0) {
        ui->labelName->setText(fileName);
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {

            int lineindex = 0;                     // file line counter
            QTextStream in(&file);                 // read to text stream

            // Clear all rows
            while (ui->tableWidgetPattern->rowCount()>0) ui->tableWidgetPattern->removeRow(0);
            while (!in.atEnd()) {

                // read one line from textstream(separated by "\n")
                QString fileLine = in.readLine();

                // parse the read line into separate pieces(tokens) with "," as the delimiter
                QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);

                ui->tableWidgetPattern->insertRow(ui->tableWidgetPattern->rowCount());
                // load parsed data to model accordingly
                for (int j = 0; j < lineToken.size(); j++) {
                    QString value = lineToken.at(j);
                    QTableWidgetItem *item = new QTableWidgetItem(value);
                    ui->tableWidgetPattern->setItem(lineindex, j, item);
                }

                lineindex++;
            }
            file.close();
        }
    } else {
        qDebug() << "No filename specified.";
    }

}

void Serial::on_pushButtonSave_clicked()
{
    QString fileName = ui->labelName->text();

    if (fileName.length()>0) {
        QString textData;
        int rows = ui->tableWidgetPattern->rowCount();
        int columns = ui->tableWidgetPattern->columnCount();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {

                    textData += ui->tableWidgetPattern->item(i,j)->text();
                    textData += ", ";      // for .csv file format
            }
            textData += "\n";             // (optional: for new line segmentation)
        }
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&csvFile);
            out << textData;
            csvFile.close();
        }
    }
}


void Serial::on_toolButtonAddRowPattern_clicked()
{
    QModelIndexList selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.count()>0) {
        ui->tableWidgetPattern->insertRow(selection.at(selection.count()-1).row()+1);
    } else {
        ui->tableWidgetPattern->insertRow(ui->tableWidgetPattern->rowCount());
    }
}

void Serial::on_toolButtonAddRowServo_clicked()
{
    QModelIndexList selection = ui->tableWidgetAmounts->selectionModel()->selectedRows();
    if (selection.count()>0) {
        ui->tableWidgetAmounts->insertRow(selection.at(selection.count()-1).row()+1);
    } else {
        ui->tableWidgetAmounts->insertRow(ui->tableWidgetAmounts->rowCount());
    }
}

void Serial::on_toolButtonRemoveRowPattern_clicked()
{
    QModelIndexList selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    int c=0;
    for(int i=0; i< selection.count(); i++)
    {
        int y = selection.at(i).row();
        ui->tableWidgetPattern->removeRow(y-c);
        c++;
    }
}

void Serial::on_toolButtonRemoveRowServo_clicked()
{
    QModelIndexList selection = ui->tableWidgetAmounts->selectionModel()->selectedRows();
    int c=0;
    for(int i=0; i< selection.count(); i++)
    {
        int y = selection.at(i).row();
        ui->tableWidgetAmounts->removeRow(y-c);
        c++;
    }
}

void Serial::on_pushButtonSaveAs_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save CSV Pattern"), ui->labelName->text(), tr("CSV Files (*.csv)"));

    if (fileName.length()>0) {
        QString textData;
        int rows = ui->tableWidgetPattern->rowCount();
        int columns = ui->tableWidgetPattern->columnCount();

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {

                    textData += ui->tableWidgetPattern->item(i,j)->text();
                    textData += ", ";      // for .csv file format
            }
            textData += "\n";             // (optional: for new line segmentation)
        }
        QFile csvFile(fileName);
        if(csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QTextStream out(&csvFile);
            out << textData;
            csvFile.close();
        }
    }
}

void Serial::on_comboBoxSerialPort_currentTextChanged(const QString &arg1)
{
    serial->close();
    delete serialPortReader;
    qDebug() << "Updated to " << ui->comboBoxSerialPort->currentText();
    serial->setPortName(ui->comboBoxSerialPort->currentText());
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setDataBits(QSerialPort::Data8);
    serialPortReader = new SerialPortReader(serial, ui->textBrowserStatus);
    serial->open(QIODevice::ReadWrite);
}

void Serial::on_spinBoxServos_valueChanged(int _newColumns)
{
    int existingColumns = ui->tableWidgetAmounts->columnCount();
    for (int cx=0; cx<_newColumns-existingColumns; cx++) {
        ui->tableWidgetAmounts->insertColumn(ui->tableWidgetAmounts->columnCount());
        ui->tableWidgetPattern->insertColumn(ui->tableWidgetPattern->columnCount());
    }
    for (int cx=0; cx<existingColumns-_newColumns; cx++) {
        ui->tableWidgetAmounts->removeColumn(ui->tableWidgetAmounts->columnCount()-1);
        ui->tableWidgetPattern->removeColumn(ui->tableWidgetPattern->columnCount()-1);
    }
}

void Serial::on_spinBoxServos_editingFinished()
{
    int _newColumns = ui->spinBoxServos->value();
}


