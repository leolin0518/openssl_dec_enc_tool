#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QDebug>
#include <QString>
#include <iostream>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QFileInfo>
#include <QMessageBox>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
void test();


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->lineEdit_key->hide();
//    ui->pushButton_dec_file->hide();
//    ui->label_file_path->hide();
//    ui->pushButton_open_file->hide();
    ui->pushButton_enc_file->hide();
    ui->label_input_key->hide();
    ui->pushButton_open_dir->hide();
    ui->pushButton_clipboard->hide();
    setWindowTitle("Password Generator");// password_generator

}




void MainWindow::openssl_cmd()
{

    QString AppPath = QApplication::applicationFilePath();
    qDebug() << "AppPath:" << AppPath;

    QString  openssl_path_dir = AppPath;
    openssl_path_dir = openssl_path_dir.replace(QString("cmd_openssl.exe"), QString("openssl/bin/"));
    qDebug() << "openssl_path_dir:" << openssl_path_dir;

    QString  openssl_path=AppPath;
    openssl_path = openssl_path.replace(QString("cmd_openssl.exe"), QString("openssl/bin/openssl.exe"));
    qDebug() << "openssl_path:" << openssl_path;

    QString in_file_path = openssl_path_dir + QString("in.log");
    QString out_file_path = openssl_path_dir + QString("out.log");
    QString openssl_enc_all;
    openssl_enc_all = openssl_path + QString(" enc -base64 -in ") + in_file_path + QString(" -out ") + out_file_path;
    qDebug() << "openssl_enc_all:" << openssl_enc_all;

    //enc -aes-128-cbc -d -in out.log -out in.log -a -k 12345678

    QProcess p;
    QString windwos_cmd="cmd.exe";
    p.start(windwos_cmd,QStringList() << "/c" <<openssl_enc_all );
    qDebug() << "-----------------------1";
    if (p.waitForStarted())
    {
       p.waitForFinished();
       qDebug() << p.readAllStandardOutput();
       qDebug() << "-------ok------";
       p.close();
    }
    else
        qDebug() << "Failed to start";


}

void MainWindow::openssl_enc_psw(QString psw)
{
/*
OpenSSL> dgst -md5  in.log
MD5(in.log)= 47bce5c74f589f4867dbd57e9ca9f808
*/





    create_file(psw);

    ui->plainTextEdit_info->clear();
   //ui->plainTextEdit_info->appendPlainText(QString("--------------密码加密---------------"));
    QString AppPath = QApplication::applicationFilePath();
    qDebug() << "AppPath--->:" << AppPath;

    QFileInfo fileInfo(AppPath);
    QString app_name = fileInfo.fileName();
    qDebug() << "fileName--->:" << fileInfo.fileName();
    qDebug() << "baseName--->:" <<fileInfo.baseName();



    QString  openssl_path=AppPath;
    //openssl_path = openssl_path.replace(QString("cmd_openssl.exe"), QString("openssl/bin/openssl.exe"));
    openssl_path = openssl_path.replace(app_name, QString("openssl/bin/openssl.exe"));
    qDebug() << "openssl_path--->:" << openssl_path;
    QString openssl_md5_all = openssl_path + QString(" dgst -md5  ") + glb_psw_tmp_file_path;
    qDebug() << "openssl_md5_all--->:" << openssl_md5_all;

    QProcess p;
    QString windwos_cmd="cmd.exe";
    p.start(windwos_cmd,QStringList() << "/c" <<openssl_md5_all );
    if (p.waitForStarted())
    {
       p.waitForFinished();
       QString p_read = p.readAllStandardOutput();
       qDebug() <<p_read;

       QStringList p_read_strlist =p_read.split("=");
       qDebug() <<"p_read_strlist" << p_read_strlist;
       qDebug() << "-------strlist------" << p_read_strlist.last();
       ui->plainTextEdit_info->clear();

       QString enc_psw = p_read_strlist.last();
       qDebug() <<"enc_psw" << enc_psw;
       enc_psw = enc_psw.trimmed();//去除字符串首尾的空格
       qDebug() <<"enc_psw.trimmed" << enc_psw;
       enc_psw = enc_psw.mid(0, 10);
       qDebug() <<"enc_psw 前十位" << enc_psw;

       ui->plainTextEdit_info->appendPlainText(enc_psw);
       qDebug() << "-------ok------";
      // ui->plainTextEdit_info->appendPlainText(QString("------------加密完成-------------"));
       p.close();
    }
    else
    {
        ui->plainTextEdit_info->appendPlainText(QString("------------加密失败-------------"));
        qDebug() << "Failed to start";
    }



}

void MainWindow::openssl_enc_aes()//目前加密功能隐藏，不使用 17/07/06
{
    ui->plainTextEdit_info->clear();
    ui->plainTextEdit_info->appendPlainText(QString("--------------encryption---------------"));
    ui->plainTextEdit_info->appendPlainText(QString("Encrypted file:") + glb_file_name);
    ui->plainTextEdit_info->appendPlainText(QString("Encrypted key:") + ui->lineEdit_key->text());

    QString AppPath = QApplication::applicationFilePath();//app的路径
    //qDebug() << "AppPath:" << AppPath;
    QFileInfo fileInfo(AppPath);
    QString app_name = fileInfo.fileName();//app的名字
    qDebug() << "fileName--->:" << fileInfo.fileName();


    //获取openssl的具体路径openssl_path
    QString  openssl_path=AppPath;
    //openssl_path = openssl_path.replace(QString("cmd_openssl.exe"), QString("openssl/bin/openssl.exe"));
    openssl_path = openssl_path.replace(app_name, QString("openssl/bin/openssl.exe"));
    //qDebug() << "openssl_path:" << openssl_path;


    QString in_file_path = glb_in_put_file_path; //C:/Users/arcadyan/Desktop/new 5.txt
    QString in_file_dir_path = in_file_path;
    in_file_dir_path = in_file_dir_path.replace(glb_file_name, QString("")); //输入文件的文件夹


    QString out_file_name = glb_file_basename +QString("_encrypt.") + glb_file_completeSuffix;
    QString out_file_path = in_file_dir_path + out_file_name;//甚至输出文件的路径 在文件名中添加_encrypt字符串

    QString openssl_enc_all;
    QString enc_key = QString(" -a -k ") + ui->lineEdit_key->text();//key目前hide，代码写死。目前加密功能隐藏，不使用
    qDebug() << "enc_key:" << enc_key;
    qDebug() << "glb_in_put_file_path:" << glb_in_put_file_path;

    openssl_enc_all = openssl_path + QString(" enc -aes-128-cbc -in ") + in_file_path + QString(" -out ") + out_file_path + enc_key;
    qDebug() << "openssl_enc_all:" << openssl_enc_all;

    //enc -aes-128-cbc  -in in.log -out out.log -a -k 12345678

    QProcess p;
    QString windwos_cmd="cmd.exe";
    p.start(windwos_cmd,QStringList() << "/c" <<openssl_enc_all );
    if (p.waitForStarted())
    {
       p.waitForFinished();
       qDebug() << p.readAllStandardOutput();
       qDebug() << "-------ok------";
       ui->plainTextEdit_info->appendPlainText(QString("加密文件在同级目录下，文件为：") + out_file_name);
       ui->plainTextEdit_info->appendPlainText(QString("------------加密完成-------------"));
       p.close();
    }
    else
    {
        ui->plainTextEdit_info->appendPlainText(QString("------------加密失败-------------"));
        qDebug() << "加密失败";
    }
}

void MainWindow::openssl_dec_aes(QString file_path)//解密
{
    /*
        //enc -aes-256-cbc -d -in out.log -out in.log -a -k 12345678

    */

        ui->plainTextEdit_info->clear();
        ui->plainTextEdit_info->appendPlainText(QString("--------------Decrypt---------------"));
        ui->plainTextEdit_info->appendPlainText(QString("Decrypt file:") + glb_file_name);
        //ui->plainTextEdit_info->appendPlainText(QString("Decrypted key:") + ui->lineEdit_key->text());


        QString AppPath = QApplication::applicationFilePath();
        //qDebug() << "AppPath:" << AppPath;
        QString  openssl_path_dir = AppPath;
        openssl_path_dir = openssl_path_dir.replace(QString("cmd_openssl.exe"), QString("openssl/bin/"));
        //qDebug() << "openssl_path_dir:" << openssl_path_dir;

        QFileInfo fileInfo(AppPath);
        QString app_name = fileInfo.fileName();
        qDebug() << "fileName--->:" << fileInfo.fileName();
        qDebug() << "baseName--->:" <<fileInfo.baseName();

        QString  openssl_path=AppPath;
        //openssl_path = openssl_path.replace(QString("cmd_openssl.exe"), QString("openssl/bin/openssl.exe"));
        openssl_path = openssl_path.replace(app_name, QString("openssl/bin/openssl.exe"));
        //qDebug() << "openssl_path:" << openssl_path;

        QString in_file_path = glb_in_put_file_path; //C:/Users/arcadyan/Desktop/new 5.txt
        QString in_file_dir_path = in_file_path;
        in_file_dir_path = in_file_dir_path.replace(glb_file_name, QString("")); //C:/Users/arcadyan/Desktop/
        QString file_base_name = glb_file_name;
        file_base_name = file_base_name.replace(QString(".bin"), QString("_decrypt.tgz"));
        QString out_file_name = file_base_name;

        QString out_file_path = in_file_dir_path + out_file_name;

        QString openssl_enc_all;
       // QString enc_key = QString(" -a -k ") + ui->lineEdit_key->text();
        QString enc_key = QString(" -k ") + QString("arc256TS");//密钥目前写死的

        qDebug() << "enc_key:" << enc_key;
        qDebug() << "glb_in_put_file_path:" << glb_in_put_file_path;
        openssl_enc_all = openssl_path + QString(" enc -aes-256-cbc -salt -d -a -in ") + file_path + QString(" -out ") + out_file_path + enc_key;
        qDebug() << "openssl_enc_all:" << openssl_enc_all;


        QProcess p;
        QString windwos_cmd="cmd.exe";
        p.start(windwos_cmd,QStringList() << "/c" <<openssl_enc_all );// /c是执行完命令后关闭命令窗口
        if (p.waitForStarted())
        {
           p.waitForFinished();
           qDebug() << p.readAllStandardOutput();//获取命令返回的信息
           qDebug() << "-------ok------";
           ui->plainTextEdit_info->appendPlainText(QString("Decrypt files at the same level directory, file as：") + out_file_name);
           //ui->plainTextEdit_info->appendPlainText(QString("如果输入的密钥为非法密约，解密出来的文件将会为乱码数据"));
           ui->plainTextEdit_info->appendPlainText(QString("------------Finished decrypting-------------"));
           p.close();
        }
        else
        {
            ui->plainTextEdit_info->appendPlainText(QString("------------Error-------------"));
        }

}



void MainWindow::create_file(QString str)
{

    QString AppPath = QApplication::applicationFilePath();
//  qDebug() << "AppPath:" << AppPath;

    QString  openssl_path_dir = AppPath;
    openssl_path_dir = openssl_path_dir.replace(QString("cmd_openssl.exe"), QString("openssl/bin/"));
//  qDebug() << "openssl_path_dir:" << openssl_path_dir;

    QString fileName=openssl_path_dir + "psw_tmp.txt";
//  qDebug() << "fileName:" << fileName;
    glb_psw_tmp_file_path = fileName;
    QFile f(fileName);

    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()  << "Open failed.";
        QMessageBox::warning(this,"file error","can't open",QMessageBox::Yes);
    }

    QTextStream in(&f);
    in << str;
    f.close();
}

void MainWindow::rm_dec_md5_eof(QString file_path)
{
//    file_path = glb_in_put_file_path;
//    if(file_path.isEmpty())
//    {
//        QMessageBox::warning(this, tr("rm_dec_md5_eof error"), tr("open rm_dec_md5_eof file error："));
//        return;
//    }
//   QFile fileIn(file_path);

//   //判断是否正确打开
//   if( ! fileIn.open( QIODevice::ReadOnly ) )
//   {
//       //打开错误
//       QMessageBox::warning(this, tr("打开错误")
//                            , tr("打开文件错误：") + fileIn.errorString());
//       return; //不处理文件
//   }
//   //读取并解析文件
//   while( ! fileIn.atEnd() )
//   {
//       //读取一行
//       QByteArray baLine = fileIn.readLine();
//      // baLine = baLine.trimmed();  //剔除字符串两端空白
//       //判断是否为注释行
//       if( baLine.startsWith('#') )
//       {
//           continue;   //不处理注释行
//       }
//       fileIn
//       qDebug()  << "Open failed.";


//   }
//   //提示加载完毕
//   QMessageBox::information(this, tr("加载配置"), tr("加载配置项完毕！"));
}

void test()
{
    int nLine=0;
    int Index=0;
    //算出行数nLine
    QString strall="abc\nme\ndo\n";
    while(Index!=-1)
    {
       Index=strall.indexOf('\n',Index+1);
       qDebug()  << Index;

       nLine++;
    }
    qDebug()  << nLine;
}



void MainWindow::DeleteOneline(int nNum, QString &strall)
{
    int nLine=0;
    int Index=0;
    //算出行数nLine
    while(Index!=-1)
    {
       Index=strall.indexOf('\n',Index+1);
       nLine++;
    }
    qDebug()  << "算出行数nLine:" << nLine;
    glb_in_put_file_line_number = nLine;//glb_in_put_file_line_number 为打开的文件的行数
    //如果是直接从位置0开始删除\n算一个字符"abc\nme\ndo" \n的index是3要删除3+1个字符，即index+1个
    if(nNum==0)
    {
       int nIndex=strall.indexOf('\n');
       strall.remove(0,nIndex+1);
    }
    else
    {
       int nTemp=nNum;
       int nIndex=0,nIndex2=0;
       while(nTemp--)
       {
           //
           nIndex=strall.indexOf('\n',nIndex+1);//这里会更新nIndex
           if(nIndex!=-1)//说明是有效的
           {
               nIndex2=strall.indexOf('\n',nIndex+1);
           }
       }
       //删除的行不是最后一行（从nIndex+1这个位置起nIndex2-nIndex个字符全部抹去）
       if(nNum<nLine-1)
       {
           strall.remove(nIndex+1, nIndex2-nIndex);//不用减一
       }
       //删除的是最后一行（从nIndex起始len-nIndex个字符全抹去）
       //不能从nIndex+1处开始，
       else if(nNum==nLine-1)
       {
           int len=strall.length();
           strall.remove(nIndex,len-nIndex);
       }
       else
       {
       }
    }
}

void MainWindow::delete_one_line_in_file(int nNumLine, QString &filename)//删除文件的某一行
{
    QString strall;
    QFile readfile(filename);
    if( readfile.open(QIODevice::ReadOnly) )
    {
        QTextStream stream(&readfile);
        strall=stream.readAll();
    }
    readfile.close();
    DeleteOneline(nNumLine, strall);
    qDebug() << "filename"<<filename;
    glb_in_put_file_path_tmp = (filename + QString(".tmp"));
//    qDebug() << "glb_in_put_file_path_tmp" << glb_in_put_file_path_tmp;
//    QFile writefile(filename + QString(".tmp"));
    QFile writefile(glb_in_put_file_path_tmp);
    if(writefile.open(QIODevice::WriteOnly))
    {
        QTextStream wrtstream(&writefile);
        wrtstream<<strall;
    }
    writefile.close();

}

QString MainWindow::read_file(QString file_path)
{
    QFile f(file_path);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()  << "Open failed.";
        QMessageBox::warning(this,"sdf","can't open",QMessageBox::Yes);
    }

    QTextStream txtOutput(&f);
    QString lineStr;
    QString all_line_str;
    while(!txtOutput.atEnd())
    {
        lineStr = txtOutput.readLine();
        all_line_str += lineStr;
        qDebug()  << "lineStr:" << lineStr;
    }
    qDebug()  << "all_lineStr:" << all_line_str;

    qDebug()  << file_path;
    f.close();
   // f.remove(file_path);
    return all_line_str;
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_open_file_clicked()//打开文件
{
    QString desktop_path = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug()<< "desktop_path:" << desktop_path;
    QStringList fileNames = QFileDialog::getOpenFileNames(this,tr("Select file"),
                                                desktop_path);
     qDebug()<< "fileNames:" << fileNames;
     QString fileName = fileNames.join(",");
     glb_in_put_file_path = fileName;
     qDebug()<< "glb_in_put_file_path:" << glb_in_put_file_path;

     QFileInfo fileInfo(glb_in_put_file_path);
     glb_file_name = fileInfo.fileName();
     glb_file_basename =  fileInfo.baseName();
     glb_file_completeSuffix = fileInfo.completeSuffix();
     qDebug()<< "文件名称："<< glb_file_name << "glb_file_basename:" <<glb_file_basename << "glb_file_completeSuffix" << glb_file_completeSuffix;
     ui->plainTextEdit_info->clear();
     ui->label_file_path->setText(fileName);
     ui->plainTextEdit_info->appendPlainText(QString("File name:") + glb_file_name);
}

void MainWindow::on_pushButton_dec_file_clicked()//解密按钮
{
    QString key=ui->lineEdit_key->text();//获取密钥，因为当前是不需要用户输入，目前是界面上key是hide的，key目前是写死的
    qDebug() << key;

    //因为加密的文件是在第十一行插入md5，删除第十一行，以及删除最后一行的EOF 标记
    delete_one_line_in_file(10, glb_in_put_file_path);//从0开始计行数 10->11
    qDebug() << "glb_in_put_file_line_number-2:" << glb_in_put_file_line_number-2;
    int last_eof = glb_in_put_file_line_number - 2 - 1;
    delete_one_line_in_file(last_eof, glb_in_put_file_path_tmp);


    //开始解密
    openssl_dec_aes(glb_in_put_file_path_tmp);


    //删除临时文件
//    QString fileNameTmpTmp=glb_in_put_file_path_tmp;
//    qDebug() << "del tmp tmp:" << glb_in_put_file_path_tmp;
//    QFile file;
//    file.remove(fileNameTmpTmp);

//    QString file_tmp = fileNameTmpTmp.replace(QString(".tmp.tmp"), QString(".tmp"));
//    qDebug() << "del tmp" << glb_in_put_file_path_tmp;
//    file.remove(file_tmp);

    //
    ui->pushButton_open_dir->show();

}

void MainWindow::on_pushButton_enc_file_clicked()//加密 目前hide掉
{
    QString key=ui->lineEdit_key->text();
    qDebug() << key;
    if(key == "")
    {
        QMessageBox::information(NULL,"Key Error","密钥错误，请重新输入");
    }
    else
    {
         openssl_enc_aes();
    }
}

void MainWindow::on_pushButton_enc_psw_clicked()//密码加密按钮
{
    QString mac = ui->lineEdit_mac->text().toLower();
    QString sn = ui->lineEdit_sn->text();
  //  mac = "E4:3E:D7:07:b0:3d";
  //  sn="10000509";
    qDebug()<< "mac:" << mac << "sn:" << sn;

    if( mac == "" || sn == "" )
    {
        QMessageBox::information(NULL,"Error", "Mac or SN, Error\n");
        return;
    }

    QString  mac_str_4 = mac.section(':', 4, 4);
    QString  mac_str_5 = mac.section(':', 5, 5);
    qDebug()<< "mac_str_4:" << mac_str_4 ;
    qDebug()<< "mac_str_5:" << mac_str_5 ;
    //mac_str.replace(QString(":"), QString(""));
    QString mac_sn_mac = mac_str_4 + sn + mac_str_5;
    qDebug()<< "mac_sn_mac:" << mac_sn_mac ;

    openssl_enc_psw(mac_sn_mac);//密码加密

    //删除临时文件
    QString fileName=glb_psw_tmp_file_path;
    QFile file;
    file.remove(fileName);

    ui->pushButton_clipboard->show();

}

void MainWindow::on_pushButton_clipboard_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    QString originalText = clipboard->text();         //获取剪贴板上文本信息
    qDebug()<< "originalText:" << originalText ;
    qDebug()<< "PlainText:" << ui->plainTextEdit_info->toPlainText() ;
    clipboard->setText(ui->plainTextEdit_info->toPlainText());                  //设置剪贴板内容

    ui->plainTextEdit_info->appendPlainText(QString("\nIt has copy the password to the clipboard."));
}

void MainWindow::on_pushButton_open_dir_clicked()
{
    QFileInfo fileInfo(glb_in_put_file_path);
    fileInfo.path(); // returns "/home/dipper"
    qDebug() << "fileInfo.path()" <<  fileInfo.path();
    QDesktopServices::openUrl(QUrl::fromLocalFile( fileInfo.path()));
}
