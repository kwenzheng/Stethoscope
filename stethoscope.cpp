#include "stethoscope.h"
#include "ui_stethoscope.h"
#include<QSerialPortInfo>
#include<QMessageBox>
#include<QDebug>
#include<QTextCodec>
static const uint8_t aucCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40};
static const uint8_t aucCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

Stethoscope::Stethoscope(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stethoscope)
{
    ui->setupUi(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        port_info_list=QSerialPortInfo::availablePorts();
        for (int i=0;i<port_info_list.size();i++) {
            ui->serialport_combox->addItem(port_info_list[i].portName());
        }
    }
}

Stethoscope::~Stethoscope()
{
    delete ui;
}

void Stethoscope::on_open_serial_button_clicked()
{
    if(ui->serialport_combox->currentIndex()!=-1)
    {
        serial_port->setPortName(ui->serialport_combox->currentText());
        if(!serial_port->open(QIODevice::ReadWrite))
        {
            qDebug()<<"serial open fail";//用ReadWrite 的模式尝试打开串口
            QMessageBox::critical(NULL,"提示","串口打开失败",QMessageBox::Ok);
            return;
        }
        else {
            QMessageBox::critical(NULL,"提示","串口打开成功",QMessageBox::Ok);
        }
        //设置波特率
        serial_port->setBaudRate(9600);
        //设置数据位数
        serial_port->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serial_port->setParity(QSerialPort::NoParity);
        //设置停止位
        serial_port->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial_port->setFlowControl(QSerialPort::NoFlowControl);
        QByteArray arry;
        arry.resize(6);
        arry[0]=0x5A;
        arry[1]=0xA5;
        arry[2]=0x00;
        arry[3]=0x81;
        arry[4]=0xC2;
        arry[5]=0xBF;
        serial_port->write(arry);
        connect( serial_port, SIGNAL(readyRead()), this, SLOT(ReadMycom()));
    }
}
void Stethoscope::ReadMycom()
{
    QByteArray value;
    value=serial_port->read(1);
    if(reply_list.size()<=9)
    {
        QString temp=value.toHex();
        reply_list.append(temp);
    }
    else {
        QString temp=value.toHex();
        file_name_list.append(temp);
        if(file_name_list.size()==4&&file_name_list[3]=="81")//文件名传输完毕
        {
            file_name_list.clear();
            reply_list.clear();
            ui->listWidget->addItems(file_name);
            unsigned char crctablehi[] = {0x5A,0xA5,0x00,0x80,0x00,0x81,0x01,0x19,0xB9};
            QByteArray arry;
            arry.resize(9);
            for (int i=0;i<9;i++) {
                arry[i]=crctablehi[i];
            }
            serial_port->write(arry);
        }
        if(file_name_list.size()==60)
        {
            for (int i=0;i<file_name_list.size();i++) {
                if(i==8)
                {
                    file_index.append(file_name_list[i]);
                    file_index.append(file_name_list[i+1]);
                }
                if(i==10)
                {
                    for (int j=10;j<=57;j++) {
                        if(j<57&&file_name_list[j]=="00"&&file_name_list[j+1]=="00")
                            break;
                        word.append(file_name_list[j]);
                        if(word.size()==4)
                        {
                            bool ok;
                            QChar* unicode=new QChar[1];
                            unicode[0]=word.toInt(&ok,16);
                            QString str=QString::fromRawData(unicode,1);
                            file_name_str=file_name_str+str;
                            word.clear();
                        }
                    }
                    break;
                }
            }
            file_name.append(file_name_str);
            file_name_str.clear();
            file_name_list.clear();
        }

    }


}
void Stethoscope::on_close_serial_button_clicked()
{
    if(ui->serialport_combox->currentIndex()!=-1)
    {
        QMessageBox::critical(NULL,"提示","串口关闭成功",QMessageBox::Ok);
        serial_port->close();
    }

}
uint16_t Stethoscope::CRC16CHECKMODBUS( uint8_t * pucFrame, uint16_t usLen)
{
    uint8_t ucCRCHi = 0xFF;
    uint8_t ucCRCLo = 0xFF;
    int iIndex;
    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( uint8_t)( ucCRCHi ^ aucCRCHi[iIndex] );
        ucCRCHi = aucCRCLo[iIndex];
    }
    return ( uint16_t)( ucCRCHi << 8 | ucCRCLo );
}
QStringList Stethoscope::Uint16toHex(uint16_t value)
{
    QString ret="";
    char byte;
    int buf[5];
    for(int j=0;j<4;j++)
    {
        for(int i=0;i<4;i++)
        {
            //qDebug()<<"value"<<value;
            buf[i]=value&0x01;
            //qDebug()<<QString("buf[%1]=").arg(i)<<buf[i];
            value=value>>1;
        }

        byte=buf[0]*pow(2,0)+buf[1]*pow(2,1)+buf[2]*pow(2,2)+buf[3]*pow(2,3);
        if(byte>9)
        {
            byte+=55;
        }
        else
        {
            byte+=48;
        }
        //qDebug()<<byte;
        ret.prepend(byte);
    }

    QString temp=ret.left(2);
    ret=ret.right(2)+temp;
    temp="";
    temp.append(ret.at(0));
    temp.append(ret.at(1));
    QStringList strlist;
    strlist.append(temp);
    temp="";
    temp.append(ret.at(2));
    temp.append(ret.at(3));
    strlist.append(temp);
    return strlist;
}
