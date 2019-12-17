#ifndef MAINWINDOW_H



#define MAINWINDOW_H







#include <QMainWindow>
#include <QStackedWidget>


#include <QMessageBox>

#include <QTimer>


#include <string>


#include <stdio.h>



#include <stdlib.h>



#include <unistd.h>



#include <sys/types.h>



#include <sys/stat.h>



#include <fcntl.h>

#include <signal.h>
#include <QComboBox>



#include <string.h>







#define MAX_DIGIT 4

#define MAX_BUFF 32

#define LINE_BUFF 16

#define MAX_BUTTON 9

#define MOTOR_ATTRIBUTE_ERROR_RANGE 4



#define FND_DEVICE "/dev/fpga_fnd"

#define FPGA_TEXT_LCD_DEVICE "/dev/fpga_text_lcd"

#define FPGA_PUSH_SWITCH_DEVICE "/dev/fpga_push_switch"

#define FPGA_STEP_MOTOR_DEVICE "/dev/fpga_step_motor"

#define FPGA_DIP_SWITCH_DEVICE "/dev/fpga_dip_switch"

#define LED_DEVICE "/dev/fpga_led"

#define FPGA_DOT_DEVICE "/dev/fpga_dot"


namespace Ui {



class MainWindow;



}







class MainWindow : public QMainWindow



{



    Q_OBJECT







public:



    explicit MainWindow(QWidget *parent = 0);



    ~MainWindow();


    void itoa(int value, char* str, int base);

    void strreverse(char* begin, char* end);



private slots:

    void write_fnd_lcd();

    void update_step_motor();

    void update_led();

    void on_Active_BT_toggled(bool checked);

    void on_Direction_BT_toggled(bool checked);

    void on_Speed_Bar_sliderMoved(int position);


    void on_Up_BT_clicked();
    void on_Down_BT_clicked();

    void on_Set_BT_clicked();

    void on_Close_BT_clicked();

    void on_comboBox_activated(int index);

    void setting(bool boolean);
    void on_pushButton_clicked();
    void dial();
private:



    Ui::MainWindow *ui;

    QStackedWidget *stk;



    int dev,dev2,dev3,dev4,dev5,dev6,dev7;



    unsigned char data[4];



    unsigned char retval;



    int i;
    int j;
    int page;
    int a,b,c;

    int buf_size;

    QString s;
    int str_size,str_size2;



    int chk_size;



    char string[32];

    int buff_size;


    int motor_action;



    int motor_direction;



    int motor_speed;



    unsigned char motor_state[3];



    bool check;



    int speed;

    int flag;
    int flag2;
    int result;



    //dip
    unsigned char dip_sw_buff;

    unsigned char write_data;

    unsigned int  sum = 0;

    unsigned char data2[8] = {0, };






    unsigned char push_sw_buff[MAX_BUTTON];


    int set_num;

    QMessageBox msgBox;







};







#endif // MAINWINDOW_H






