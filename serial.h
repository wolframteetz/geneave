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

    void updateFullPattern();
    void on_iconEditorColumns_imageChanged();
    void on_iconEditorPattern_imageChanged();
    void on_iconEditorRows_imageChanged();
    void on_pushButton_clicked();

    void iconEditorBaseParametersChanged();
    void on_spinBoxThreadles_valueChanged(int arg1);
    void on_spinBoxShafts_valueChanged(int arg1);
    void on_spinBoxWarpthreads_valueChanged(int arg1);
    void on_spinBoxPicks_valueChanged(int arg1);

private:
    Ui::Serial *ui;  
    QSerialPort *serial;
    SerialPortReader *serialPortReader;
    void executeRows(bool zeroAll=false);
    QImage *iconImageColumns;
    QImage *iconImageRows;
    QImage *iconImagePattern;
    QImage *iconImageFullPattern;
    int threadles;
    int shafts;
    int warpthreads;
    int picks;

};

#endif // SERIAL_H
