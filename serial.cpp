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

    iconEditorBaseParametersChanged();
}

Serial::~Serial()
{
    serial->close();
    delete ui;
}

void Serial::on_pushButtonExecute_clicked()
{
/*    ui->pushButtonExecute->setEnabled(false);
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
    ui->pushButtonExecute->setEnabled(true);*/
}

void Serial::on_pushButtonWeaveSelectedRows_clicked()
{
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
    qDebug() << "Updated to " << arg1;
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
}

void Serial::executeRows(bool zeroAll)
{
    ui->pushButtonMoveToZero->setEnabled(false);
    ui->pushButtonWeaveAndForward->setEnabled(false);
    ui->pushButtonWeaveAndBackward->setEnabled(false);

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
                if (!zeroAll) s.append(ui->tableWidgetAmounts->item(v,x)->text());
                else s.append(ui->tableWidgetAmounts->item(0,0)->text());
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
    ui->pushButtonMoveToZero->setEnabled(true);
    ui->pushButtonWeaveAndForward->setEnabled(true);
    ui->pushButtonWeaveAndBackward->setEnabled(true);
}

void Serial::on_pushButtonWeaveAndForward_clicked()
{
    // make sure ANY row is selected
    QModelIndexList selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.isEmpty()) selection = ui->tableWidgetPattern->selectionModel()->selectedIndexes();
    if (selection.isEmpty()) ui->tableWidgetPattern->selectRow(0);
    // forward
    selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.isEmpty()) selection = ui->tableWidgetPattern->selectionModel()->selectedIndexes();
    ui->tableWidgetPattern->selectRow(  (selection.at(selection.count()-1).row()+1)  %  ui->tableWidgetPattern->rowCount() );
    // execute
    ui->textBrowserStatus->setText("Weave selected rows and forward.");
    executeRows();
}

void Serial::on_pushButtonWeaveAndBackward_clicked()
{
    // make sure ANY row is selected
    QModelIndexList selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.isEmpty()) selection = ui->tableWidgetPattern->selectionModel()->selectedIndexes();
    if (selection.isEmpty()) ui->tableWidgetPattern->selectRow(0);
    // backward
    selection = ui->tableWidgetPattern->selectionModel()->selectedRows();
    if (selection.isEmpty()) selection = ui->tableWidgetPattern->selectionModel()->selectedIndexes();
    ui->tableWidgetPattern->selectRow(  (selection.at(selection.count()-1).row()+ui->tableWidgetPattern->rowCount()-1)  %  ui->tableWidgetPattern->rowCount());
    // execute
    ui->textBrowserStatus->setText("Weave selected rows and backward.");
    executeRows();
}

void Serial::on_pushButtonMoveToZero_clicked()
{
    ui->textBrowserStatus->setText("Zero all.");
    executeRows(true);
}

int Serial::help_getWeaveX(int lifty)
{
    int weavex=0;
    for (int liftx = 0; liftx <  ui->iconEditorRows->image.width(); ++liftx) {
        QColor color = QColor::fromRgba(ui->iconEditorRows->image.pixel(liftx, lifty)); // check xy
        if (color.red() < 255) {
            weavex=liftx; break; // found active weave column
        }
    }
    return weavex;
}

void Serial::updateFullPattern()
{
    for (int lifty = 0; lifty <  ui->iconEditorRows->image.height(); ++lifty) {
        int weavex=help_getWeaveX(lifty);
        // reset all
        for (int tiex = 0; tiex <  ui->iconEditorColumns->image.width(); ++tiex) {
            ui->iconEditorFullPattern->image.setPixel(tiex, lifty, qRgba(255, 255, 255, 255));
        }
        // set
        for (int weavey = 0; weavey <  ui->iconEditorPattern->image.height(); ++weavey) {
            QColor color = QColor::fromRgba(ui->iconEditorPattern->image.pixel(weavex, weavey));
            if (color.red() < 255) { // active shaft
                for (int tiex = 0; tiex <  ui->iconEditorColumns->image.width(); ++tiex) {
                    QColor colorTie = QColor::fromRgba(ui->iconEditorColumns->image.pixel(tiex, weavey)); // check xy
                    if (colorTie.red() < 255) {
                        ui->iconEditorFullPattern->image.setPixelColor(tiex, lifty, colorTie); // shaft raised, set color of tie up
                    }
                }
            }
        }
    }
    ui->iconEditorFullPattern->update();
}

void Serial::on_iconEditorColumns_imageChanged()
{
    updateFullPattern();
}

void Serial::on_iconEditorPattern_imageChanged()
{
    updateFullPattern();
}

void Serial::on_iconEditorRows_imageChanged()
{
    updateFullPattern();
}

void Serial::iconEditorBaseParametersChanged()
{
    shafts = ui->spinBoxShafts->value();
    threadles = ui->spinBoxThreadles->value();
    warpthreads = ui->spinBoxWarpthreads->value();
    picks = ui->spinBoxPicks->value();

    iconImageColumns = new QImage(warpthreads, shafts, QImage::Format_ARGB32);
    ui->iconEditorColumns->setIconImage(*iconImageColumns);
    ui->iconEditorColumns->setMinimumSize(ui->iconEditorColumns->sizeHint());
    ui->iconEditorColumns->forceOnePixelPerRowColumn(false, true);
    iconImageRows = new QImage(threadles, picks, QImage::Format_ARGB32);
    ui->iconEditorRows->setIconImage(*iconImageRows);
    ui->iconEditorRows->setMinimumSize(ui->iconEditorRows->sizeHint());
    ui->iconEditorRows->forceOnePixelPerRowColumn(true, false);
    //ui->iconEditorRows->setEnabled(false);
    iconImagePattern = new QImage(threadles, shafts, QImage::Format_ARGB32);
    ui->iconEditorPattern->setIconImage(*iconImagePattern);
    ui->iconEditorPattern->setMinimumSize(ui->iconEditorPattern->sizeHint());
    ui->iconEditorPattern->forceOnePixelPerRowColumn(false, false);
    iconImageFullPattern = new QImage(warpthreads, picks, QImage::Format_ARGB32);
    ui->iconEditorFullPattern->setIconImage(*iconImageFullPattern);
    ui->iconEditorFullPattern->setMinimumSize(ui->iconEditorFullPattern->sizeHint());
    updateFullPattern();


}

void Serial::on_pushButton_clicked()
{

    //ui->tableWidgetPattern->setColumnCount(ui->iconEditorPattern->image.size().height()); // columns stay # of servos connected
    ui->tableWidgetPattern->setRowCount(ui->iconEditorRows->image.size().height()); // weave size

    for (int y=0; y<ui->iconEditorRows->image.size().height(); y++) {
        int weavex=help_getWeaveX(y);
        for (int x=0; x<ui->tableWidgetPattern->columnCount(); x++) { // all servos need commands
            if (x>=ui->iconEditorPattern->image.size().height()) { // unused servo
                QTableWidgetItem *setdes = new QTableWidgetItem; // M E M O R Y   L E A K
                setdes->setText("0");
                ui->tableWidgetPattern->setItem(y, x, setdes);
            } else {
                QColor color = QColor::fromRgba(ui->iconEditorPattern->image.pixel(weavex,x));
                qDebug() << weavex << "," << x << color.red();
                QTableWidgetItem *setdes = new QTableWidgetItem; // M E M O R Y   L E A K
                if (color.red()<255) setdes->setText("1"); else setdes->setText("0"); // UPDATE THIS
                ui->tableWidgetPattern->setItem(y, x, setdes);
            }
        }
    }
}

void Serial::on_spinBoxThreadles_valueChanged(int arg1)
{
    threadles = arg1;
    iconEditorBaseParametersChanged();
}

void Serial::on_spinBoxShafts_valueChanged(int arg1)
{
    shafts = arg1;
    iconEditorBaseParametersChanged();
}

void Serial::on_spinBoxWarpthreads_valueChanged(int arg1)
{
    warpthreads = arg1;
    iconEditorBaseParametersChanged();
}

void Serial::on_spinBoxPicks_valueChanged(int arg1)
{
    picks = arg1;
    iconEditorBaseParametersChanged();
}
