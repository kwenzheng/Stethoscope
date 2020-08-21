#ifndef STETHOSCOPE_H
#define STETHOSCOPE_H

#include <QWidget>
#include <QSerialPort>
namespace Ui {
class Stethoscope;
}

class Stethoscope : public QWidget
{
    Q_OBJECT

public:
    explicit Stethoscope(QWidget *parent = nullptr);
    ~Stethoscope();

private slots:
    void on_open_serial_button_clicked();
    void ReadMycom();

    void on_close_serial_button_clicked();

private:
    uint16_t CRC16CHECKMODBUS( uint8_t * pucFrame, uint16_t usLen);
    QStringList Uint16toHex(uint16_t value);
    Ui::Stethoscope *ui;
    QStringList port_info_list;
    QSerialPort *serial_port;
    QStringList file_index;
    QStringList reply_list;
    QStringList file_name_list;
    QStringList file_name;//保存最终解析出来的文件名
    QString word;
    QString file_name_str;
    bool flag;//用作判断下位机响应
};

#endif // STETHOSCOPE_H
