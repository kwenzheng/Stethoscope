#include "stethoscope.h"
#include "ui_stethoscope.h"
#include<QSerialPortInfo>
#include<QMessageBox>
#include<QDebug>
#include<QTextCodec>
#include<QFileDialog>
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
    serial_port=new QSerialPort(this);
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serial_port->setPort(info);
        // 判断端口能否以读写方式打开
        if(serial_port->open(QIODevice::ReadWrite))
        {
            port_info_list.append(serial_port->portName());
            serial_port->close(); // 然后自动关闭等待人为开启（通过"打开串口按钮"）
        }
    }
    ui->serialport_combox->addItems(port_info_list);
    connect(&time,SIGNAL(timeout()),this,SLOT(SendToProgress()));
    connect(this,SIGNAL(FinishOneTransport()),this,SLOT(FinishOneTransportSlot()));
}
Stethoscope::~Stethoscope()
{
    delete ui;
}
void Stethoscope::SendToProgress()
{
//    count_progress++;
//    progressDlg->setValue(count_progress);
//    if(count_progress==8)
//    {
//        time.stop();
//        count_progress=0;
//    }
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
            QMessageBox::information(NULL,"提示","串口打开成功",QMessageBox::Ok);
            ui->listWidget->clear();
        }
        //设置波特率
        serial_port->setBaudRate(921600);
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
        flag=false;
    }
}
void Stethoscope::ReadMycom()
{
    QByteArray value_serial;
    value_serial=serial_port->readAll();
    QString str;
    QByteArray arry;
    QStringList value;
    for (int i=0;i<value_serial.size();i++) {
        arry.append(value_serial.at(i));
        str=arry.toHex();
        value.append(str);
        arry.clear();
    }
    //qDebug()<<"value"<<value;
    for (int i=0;i<value.size();i++) {
        if(flag)
        {
            if(value.size()-1-i>4&&value.at(i)==tr("5a"))
            {
                if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("81"))
                {
                    QByteArray arr_tmp;
                    for(int j=i;j<i+10;j++)
                    {
                        arr_tmp.append(value_serial.at(j));
                    }
                    uint8_t uint8_tmp[8];
                    for (int k=0;k<8;k++) {
                        uint8_tmp[k]=arr_tmp.at(k);
                    }
                    uint16_t uint16_tmp;
                    uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,8);
                    QStringList list_tmp=Uint16toHex(uint16_tmp);
                    QByteArray str1,str2;
                    str1.append(arr_tmp.at(8));
                    str2.append(arr_tmp.at(9));
                    if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                    {
                        QByteArray tmp;
                        for (int k=7;k>=4;k--) {
                            tmp.append(arr_tmp.at(k));
                        }
                        uint32_t m;
                        QString tmp_str_1;
                        tmp_str_1=tmp.toHex();
                        bool ok;
                        m=tmp_str_1.toUInt(&ok,16);
                        qDebug()<<"m_2"<<m<<num_file;
                        if(m==num_file)
                        {
                            QByteArray arry;
                            arry.resize(9);
                            arry[0]=0x5A;
                            arry[1]=0xA5;
                            arry[2]=0x00;
                            arry[3]=0x80;
                            arry[4]=0x00;
                            arry[5]=0x81;
                            arry[6]=0x01;
                            arry[7]=0x71;
                            arry[8]=0xA0;
                            serial_port->write(arry);

                            ui->listWidget->addItems(file_name);
                            qDebug()<<"file_name_size"<<file_name.size();
                            ui->current_file_num_label->setText(QString::number(file_name.size()));
                            num_file=0;
                            flag=false;
                            QMessageBox::information(NULL,"提示","数据传输成功",QMessageBox::Ok);
                            break;
                        }
                    }
                }
            }
            //qDebug()<<"i"<<i;
            wav_name.append(value_serial.at(i));
           // qDebug()<<"wave_name_size"<<wav_name.size();
            if(wav_name.size()==60)
            {
                //qDebug()<<"wave_name"<<wav_name;
                uint8_t uint8_tmp[58];
                for (int k=0;k<58;k++) {
                    uint8_tmp[k]=wav_name.at(k);
                }
                //qDebug()<<"wave_name_size"<<wav_name.size();
                uint16_t uint16_tmp;
                uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,58);
                QStringList list_tmp=Uint16toHex(uint16_tmp);
                QByteArray str1,str2;
                str1.append(wav_name.at(58));
                str2.append(wav_name.at(59));
                qDebug()<<list_tmp<<str1.toHex()<<str2.toHex();
                if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                {

                    QByteArray file_index_tmp;
                    file_index_tmp.append(wav_name[8]);
                    file_index_tmp.append(wav_name[9]);
                    file_index.append(file_index_tmp);
                    QByteArray tmp_arr;
                    for(int k=10;k<=57;k++)
                    {
                        if(wav_name.at(k)==NULL&&wav_name.at(k+1)==NULL&&wav_name.at(k+2)==NULL&&wav_name.at(k+3)==NULL&&wav_name.at(k+4)==NULL&&wav_name.at(k+5)==NULL)
                            break;

                        tmp_arr.append(wav_name.at(k));
                        word.append(tmp_arr.toHex());
                        //获取十六进制数
                        bool ok;
                        int dec = word.toInt(&ok,16);          //十六进制转十进制
                        char st = dec;                          //十进制数用char类型表示--->必须有此步直接转化不可行
                        QString str_ascii = QString(st);
                        file_name_str=file_name_str+str_ascii;
                        word.clear();
                        tmp_arr.clear();
                    }
                    file_name.append(file_name_str);
                    file_name_str.clear();
                    wav_name.clear();
                    num_file++;
                }
                else {
                    qDebug()<<"CRC校验失败"<<num_file;
                    wav_name.clear();
                }
            }
        }
        if(!flag)
        {
            if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("00"))
            {
                for (int j=0;j<value_serial.size();j++) {
                    wav_name.append(value_serial.at(j));
                }
                QByteArray tmp;
                for (int k=9;k>=8;k--) {
                    tmp.append(wav_name.at(k));
                }
                uint32_t m;
                QString tmp_str_1;
                tmp_str_1=tmp.toHex();
                bool ok;
                m=tmp_str_1.toUInt(&ok,16);
                qDebug()<<"m"<<m;
                ui->all_file_num_label->setText(QString::number(m));
                num_file++;
                wav_name.clear();
                flag=true;
                break;
            }
        }
    }
}
void Stethoscope::on_close_serial_button_clicked()
{
    if(ui->serialport_combox->currentIndex()!=-1)
    {
        serial_port->close();
        QMessageBox::information(NULL,"提示","串口关闭成功",QMessageBox::Ok);
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
            buf[i]=value&0x01;
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
        ret.prepend(byte);
    }
    QString temp=ret.left(2);
    ret=ret.right(2)+temp;
    temp="";
    temp.append(ret.at(0));
    temp.append(ret.at(1));
    QStringList strlist;
    strlist.append(temp.toLower());
    temp="";
    temp.append(ret.at(2));
    temp.append(ret.at(3));
    strlist.append(temp.toLower());
    return strlist;
}

void Stethoscope::on_save_button_clicked()
{
    fileName = QFileDialog::getExistingDirectory(
                this, "choose src Directory",
                "/");
    if (fileName.isEmpty())
    {
        return;
    }
    disconnect(serial_port, SIGNAL(readyRead()),0,0);
    connect(serial_port, SIGNAL(readyRead()), this, SLOT(ReadAllFileContent()));
}

void Stethoscope::on_transport_button_clicked()
{
    int row=ui->listWidget->currentRow();
    if(row==-1)
        return;
    fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                            tr("Files (*.wav)"));
    uint8_t tmp_uint8[8]={0x5A,0xA5,0x00,0x82};
    tmp_uint8[4]=file_index[row].at(0);
    tmp_uint8[5]=file_index[row].at(1);
    QByteArray arr;
    uint16_t temp_uint16_t;
    temp_uint16_t=CRC16CHECKMODBUS(tmp_uint8,6);
    bool ok;
    QStringList temp_stringlist=Uint16toHex(temp_uint16_t);
    int dec = temp_stringlist[0].toInt(&ok,16);          //十六进制转十进制
    char st = dec;
    tmp_uint8[6]=st;
    dec = temp_stringlist[1].toInt(&ok,16);
    st = dec;
    tmp_uint8[7]=st;
    for(int i=0;i<8;i++)
    {
        arr.append(tmp_uint8[i]);
    }
    serial_port->write(arr);
//    progressDlg = new QProgressDialog;
//    progressDlg->setWindowModality(Qt::WindowModal);
//    progressDlg->setMinimumDuration(0);//dialog出现需等待的时间
//    progressDlg->setWindowTitle("Please Wait...");
//    progressDlg->setLabelText("Saving....");
//    progressDlg->setCancelButtonText("Cancel");
//    progressDlg->setRange(0,8);
    //time.start(500);
    disconnect(serial_port, SIGNAL(readyRead()),0,0);
    connect(serial_port, SIGNAL(readyRead()), this, SLOT(ReadFileContent()));

}
void Stethoscope::ReadFileContent()
{
    QByteArray value_serial;
    value_serial=serial_port->readAll();
    QString str;
    QByteArray arry;
    QStringList value;
    for (int i=0;i<value_serial.size();i++) {
        arry.append(value_serial.at(i));
        str=arry.toHex();
        value.append(str);
        arry.clear();
    }

    for (int i=0;i<value.size();i++) {
        if(flag)
        {
            if(value.size()-1-i>2&&value.at(i)==tr("5a"))
            {
                if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("81"))
                {
                    QByteArray arr_tmp;
                    for(int j=i;j<i+10;j++)
                    {
                        arr_tmp.append(value_serial.at(j));
                    }
                    uint8_t uint8_tmp[8];
                    for (int k=0;k<8;k++) {
                        uint8_tmp[k]=arr_tmp.at(k);
                    }
                    uint16_t uint16_tmp;
                    uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,8);
                    QStringList list_tmp=Uint16toHex(uint16_tmp);
                    QByteArray str1,str2;
                    str1.append(arr_tmp.at(8));
                    str2.append(arr_tmp.at(9));
                    if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                    {
                        QByteArray tmp;
                        for (int k=7;k>=4;k--) {
                            tmp.append(arr_tmp.at(k));
                        }

                        uint32_t m;
                        QString tmp_str_1;
                        tmp_str_1=tmp.toHex();
                        bool ok;
                        m=tmp_str_1.toUInt(&ok,16);
                        //qDebug()<<"m"<<m;
                        if(m==num)
                        {
                            QByteArray arry;
                            arry.resize(9);
                            arry[0]=0x5A;
                            arry[1]=0xA5;
                            arry[2]=0x00;
                            arry[3]=0x80;
                            arry[4]=0x00;
                            arry[5]=0x82;
                            arry[6]=0x01;
                            arry[7]=0x71;
                            arry[8]=0x50;
                            serial_port->write(arry);
                            //获取创建的csv文件名
                            if (fileName.isEmpty())
                                return;
                            //打开.csv文件
                            file.setFileName(fileName);
                            if(!file.open(QIODevice::WriteOnly))
                            {
                                qDebug() << "Cannot open file for writing: ";
                                return;
                            }
                            file.write(wav_content);
                            ui->file_size_label->setText(QString::number(wav_content.size()));
                            file.close();
                            qDebug()<<"帧数 "<<num<<m;
                            num=0;
                            flag=false;
                            flag_content=false;
                            wav_content.clear();
                            QMessageBox::information(NULL,"提示","数据传输成功",QMessageBox::Ok);
                            break;
                        }
                    }
                }
            }
            wav.append(value_serial.at(i));
            if(wav.size()==522)
            {
                //qDebug()<<"wav"<<wav.toHex();
                uint8_t uint8_tmp[520];
                for (int k=0;k<520;k++) {
                    uint8_tmp[k]=wav.at(k);
                }
                uint16_t uint16_tmp;
                uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,520);
                QStringList list_tmp=Uint16toHex(uint16_tmp);
                QByteArray str1,str2;
                str1.append(wav.at(520));
                str2.append(wav.at(521));
                //qDebug()<<list_tmp<<str1.toHex()<<str2.toHex();
                if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                {
                    if(!flag_content)
                    {
                        for (int k=8;k<68;k++) {
                            wav_content.append(wav.at(k));
                        }
                    }
                    if(flag_content)
                    {
                        for(int k=8;k<=519;k++)
                        {
                            if(wav.at(k)==NULL&&wav.at(k+1)==NULL&&wav.at(k+2)==NULL&&wav.at(k+3)==NULL&&wav.at(k+4)==NULL&&wav.at(k+5)==NULL&&wav.at(k+6)==NULL&&wav.at(k+7)==NULL&&wav.at(k+8)==NULL&&wav.at(k+9)==NULL&&wav.at(k+10)==NULL&&wav.at(k+11)==NULL&&wav.at(k+12)==NULL&&wav.at(k+13)==NULL&&wav.at(k+14)==NULL&&wav.at(k+15)==NULL&&wav.at(k+16)==NULL&&wav.at(k+17)==NULL)
                                break;
                            wav_content.append(wav.at(k));
                        }
                    }
                    flag_content=true;
                    num++;
                }
                else {
                    qDebug()<<"CRC校验失败"<<num;
                }
                wav.clear();
            }
        }
        if(!flag)
        {
            if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("80"))
            {
                break;
            }
            if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("02"))
            {
                flag=true;
                for (int j=i;j<value.size();j++) {
                    wav.append(value_serial.at(j));
                }
                break;
            }
        }
    }
}

void Stethoscope::CreateFile(const QString &filePath,const QString &name)
{
    QDir tempDir;
    //临时保存程序当前路径
    QString currentDir = tempDir.currentPath();
    //如果filePath路径不存在，创建它
    if(!tempDir.exists(filePath))
    {
        qDebug()<<"不存在该路径"<<endl;
        tempDir.mkpath(filePath);
    }
    //QFile tempFile;
    //将程序的执行路径设置到filePath下
    tempDir.setCurrent(filePath);
    qDebug()<<tempDir.currentPath();
    //检查filePath路径下是否存在文件fileName,如果停止操作。
    if(file_wav_content.exists(name))
    {
        qDebug()<<"文件存在";
        return ;
    }
    //此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
    file_wav_content.setFileName(name);
    if(!file_wav_content.open(QIODevice::WriteOnly))
    {
        qDebug()<<"打开失败";
    }
    //file.close();
    //将程序当前路径设置为原来的路径
    //    tempDir.setCurrent(currentDir);
    //    qDebug()<<tempDir.currentPath();
}

void Stethoscope::ReadAllFileContent()
{
    QByteArray value_serial;
    value_serial=serial_port->readAll();
    QString str;
    QByteArray arry;
    QStringList value;
    for (int i=0;i<value_serial.size();i++) {
        arry.append(value_serial.at(i));
        str=arry.toHex();
        value.append(str);
        arry.clear();
    }

    for (int i=0;i<value.size();i++) {
        if(flag)
        {
            if(value.size()-1-i>2&&value.at(i)==tr("5a"))
            {
                if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("81"))
                {
                    QByteArray arr_tmp;
                    for(int j=i;j<i+10;j++)
                    {
                        arr_tmp.append(value_serial.at(j));
                    }
                    uint8_t uint8_tmp[8];
                    for (int k=0;k<8;k++) {
                        uint8_tmp[k]=arr_tmp.at(k);
                    }
                    uint16_t uint16_tmp;
                    uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,8);
                    QStringList list_tmp=Uint16toHex(uint16_tmp);
                    QByteArray str1,str2;
                    str1.append(arr_tmp.at(8));
                    str2.append(arr_tmp.at(9));
                    if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                    {
                        QByteArray tmp;
                        for (int k=7;k>=4;k--) {
                            tmp.append(arr_tmp.at(k));
                        }

                        uint32_t m;
                        QString tmp_str_1;
                        tmp_str_1=tmp.toHex();
                        bool ok;
                        m=tmp_str_1.toUInt(&ok,16);
                        if(m==num)
                        {
                            QByteArray arry;
                            arry.resize(9);
                            arry[0]=0x5A;
                            arry[1]=0xA5;
                            arry[2]=0x00;
                            arry[3]=0x80;
                            arry[4]=0x00;
                            arry[5]=0x82;
                            arry[6]=0x01;
                            arry[7]=0x71;
                            arry[8]=0x50;
                            serial_port->write(arry);
                            qint64 u;
                            u=file_wav_content.write(wav_content);
                            ui->save_number_label->setText(QString::number(count_file_number+1));
                            wav_content.clear();
                            qDebug()<<"写入文件个数"<<u;
                            file.close();
                            qDebug()<<"帧数 "<<num<<m;
                            num=0;
                            flag=false;
                            flag_content=false;
                            count_file_number++;
                            emit FinishOneTransport();

                            break;
                        }
                    }
                }
            }
            wav.append(value_serial.at(i));
            if(wav.size()==522)
            {
                uint8_t uint8_tmp[520];
                for (int k=0;k<520;k++) {
                    uint8_tmp[k]=wav.at(k);
                }
                uint16_t uint16_tmp;
                uint16_tmp=CRC16CHECKMODBUS(uint8_tmp,520);
                QStringList list_tmp=Uint16toHex(uint16_tmp);
                QByteArray str1,str2;
                str1.append(wav.at(520));
                str2.append(wav.at(521));
                if(str1.toHex()==list_tmp[0]&&str2.toHex()==list_tmp[1])
                {
                    if(!flag_content)
                    {
                        for (int k=8;k<68;k++) {
                            wav_content.append(wav.at(k));
                        }
                    }
                    if(flag_content)
                    {
                        for(int k=8;k<=519;k++)
                        {
                            if(wav.at(k)==NULL&&wav.at(k+1)==NULL&&wav.at(k+2)==NULL&&wav.at(k+3)==NULL&&wav.at(k+4)==NULL&&wav.at(k+5)==NULL&&wav.at(k+6)==NULL&&wav.at(k+7)==NULL&&wav.at(k+8)==NULL&&wav.at(k+9)==NULL&&wav.at(k+10)==NULL&&wav.at(k+11)==NULL&&wav.at(k+12)==NULL&&wav.at(k+13)==NULL&&wav.at(k+14)==NULL&&wav.at(k+15)==NULL&&wav.at(k+16)==NULL&&wav.at(k+17)==NULL)
                                break;
                            wav_content.append(wav.at(k));
                        }
                    }
                    flag_content=true;
                    num++;
                }
                else {
                    qDebug()<<"CRC校验失败"<<num;
                }
                wav.clear();
            }
        }
        if(!flag)
        {
            if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("80"))
            {
                break;
            }
            if(value.at(i)==tr("5a")&&value.at(i+1)==tr("a5")&&value.at(i+2)==tr("01")&&value.at(i+3)==tr("02"))
            {
                flag=true;
                for (int j=i;j<value.size();j++) {
                    wav.append(value_serial.at(j));
                }
                break;
            }
        }
    }
}
void Stethoscope::FinishOneTransportSlot()
{
    qDebug()<<"count_file_number"<<count_file_number;
    if(count_file_number==file_name.size())
    {
        QMessageBox::information(NULL,"提示","传输成功",QMessageBox::Ok);
        count_file_number=0;
        return;
    }
    else {
        CreateFile(fileName,file_name[count_file_number]+".wav");
        int row=count_file_number;
        uint8_t tmp_uint8[8]={0x5A,0xA5,0x00,0x82};
        tmp_uint8[4]=file_index[row].at(0);
        tmp_uint8[5]=file_index[row].at(1);
        QByteArray arr;
        uint16_t temp_uint16_t;
        temp_uint16_t=CRC16CHECKMODBUS(tmp_uint8,6);
        bool ok;
        QStringList temp_stringlist=Uint16toHex(temp_uint16_t);
        int dec = temp_stringlist[0].toInt(&ok,16);          //十六进制转十进制
        char st = dec;
        tmp_uint8[6]=st;
        dec = temp_stringlist[1].toInt(&ok,16);
        st = dec;
        tmp_uint8[7]=st;
        for(int i=0;i<8;i++)
        {
            arr.append(tmp_uint8[i]);
        }
        serial_port->write(arr);
    }

}
void Stethoscope::on_all_transport_button_clicked()
{
    //file_name保存之前储存的文件名，fileName保存了文件存放的路径
    CreateFile(fileName,file_name[count_file_number]+".wav");
    int row=count_file_number;
    uint8_t tmp_uint8[8]={0x5A,0xA5,0x00,0x82};
    tmp_uint8[4]=file_index[row].at(0);
    tmp_uint8[5]=file_index[row].at(1);
    QByteArray arr;
    uint16_t temp_uint16_t;
    temp_uint16_t=CRC16CHECKMODBUS(tmp_uint8,6);
    bool ok;
    QStringList temp_stringlist=Uint16toHex(temp_uint16_t);
    int dec = temp_stringlist[0].toInt(&ok,16);          //十六进制转十进制
    char st = dec;
    tmp_uint8[6]=st;
    dec = temp_stringlist[1].toInt(&ok,16);
    st = dec;
    tmp_uint8[7]=st;
    for(int i=0;i<8;i++)
    {
        arr.append(tmp_uint8[i]);
    }
    serial_port->write(arr);
}
