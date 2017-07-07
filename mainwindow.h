#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void openssl_cmd();
    void openssl_enc_psw(QString psw);
    void openssl_enc_aes();
    void openssl_dec_aes(QString file_path);
    void create_file(QString str);
    void rm_dec_md5_eof(QString file_path);

    void DeleteOneline(int nNum, QString &strall); //如果是5行，nNum最大为4
    void delete_one_line_in_file(int nNumLine, QString &filename);
    QString read_file(QString file_path);

    QString glb_in_put_file_path;//打开文件的路径
    QString glb_in_put_file_path_tmp;//打开文件的路径的tmp
    int glb_in_put_file_line_number;//打开文件的行数
    QString glb_file_name;//打开文件的文件名
    QString glb_file_basename;//打开文件的文件名，没有扩展的
    QString glb_file_completeSuffix;//打开文件名的扩展名
    QString glb_psw_tmp_file_path;//


    ~MainWindow();


private slots:


    void on_pushButton_open_file_clicked();

    void on_pushButton_dec_file_clicked();

    void on_pushButton_enc_file_clicked();

    void on_pushButton_enc_psw_clicked();

    void on_pushButton_clipboard_clicked();

    void on_pushButton_open_dir_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
