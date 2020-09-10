#ifndef STETHOSCOPE_H
#define STETHOSCOPE_H

#include <QWidget>
#include <QSerialPort>
#include<QFile>
#include<QTimer>
#include<QProgressDialog>
namespace Ui {
class Stethoscope;
}

class Stethoscope : public QWidget
{
    Q_OBJECT

public:
    explicit Stethoscope(QWidget *parent = nullptr);
    ~Stethoscope();
    void CreateFile(const QString &filePath,const QString &name);

private slots:
    void on_open_serial_button_clicked();
    void ReadMycom();
    void ReadFileContent();
    void ReadAllFileContent();
    void SendToProgress();
    void FinishOneTransportSlot();

    void on_close_serial_button_clicked();

    void on_save_button_clicked();

    void on_transport_button_clicked();

    void on_all_transport_button_clicked();
signals:
    void FinishOneTransport();
private:
    uint16_t CRC16CHECKMODBUS( uint8_t * pucFrame, uint16_t usLen);
    QStringList Uint16toHex(uint16_t value);
    Ui::Stethoscope *ui;
    QStringList port_info_list;
    QSerialPort *serial_port;
    QStringList file_name_list;//最终保存的所有文件名
    QStringList file_name;//保存最终解析出来的文件名
    QString word;
    QString file_name_str;
    QString fileName;
    uint8_t per_byte[60];
    uint8_t per_byte_content[522];
    QStringList per_byte_list;
    QString file_content;
    QByteArray wav;
    QByteArray wav_content;
    QByteArray wav_name;
    QFile file;
    QFile file_wav_content;
    QList<QByteArray>file_index;//保存文件索引
    QTimer time;
    QProgressDialog *progressDlg;
    bool flag;//用作判断下位机响应
    bool flag_content=false;
    uint32_t num=0;
    uint32_t num_file=0;
    uint32_t count_progress=0;
    int count_file_number=0;//记录文件的序号
};

#endif // STETHOSCOPE_H
