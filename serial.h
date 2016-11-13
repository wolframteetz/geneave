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

    void on_spinBoxServos_valueChanged(int arg1);

    void on_spinBoxServos_editingFinished();

    void on_toolButtonAddRowServo_clicked();

    void on_toolButtonRemoveRowServo_clicked();

    void on_pushButtonWeaveAndForward_clicked();

    void on_pushButtonWeaveAndBackward_clicked();

    void on_pushButtonMoveToZero_clicked();

private:
    Ui::Serial *ui;  
    QSerialPort *serial;
    SerialPortReader *serialPortReader;
    void executeRows(bool zeroAll=false);
};

#endif // SERIAL_H
