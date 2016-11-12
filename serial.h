#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include "serialportreader.h"

namespace Ui {
class Serial;
}

class Serial : public QWidget
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    ~Serial();

private slots:
    void on_pushButtonExecute_clicked();

    void on_pushButtonLoad_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonWeaveSelectedRows_clicked();

    void on_toolButtonAddRowPattern_clicked();

    void on_toolButtonRemoveRowPattern_clicked();

    void on_pushButtonSaveAs_clicked();

    void on_comboBoxSerialPort_currentTextChanged(const QString &arg1);

private:
    Ui::Serial *ui;  
    QSerialPort *serial;
    SerialPortReader *serialPortReader;
};

#endif // SERIAL_H
