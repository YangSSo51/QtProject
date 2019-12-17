#include "mainwindow.h"
#include "fpga_dot_font.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dev = open(FND_DEVICE,O_RDWR);  //fnd_device
    dev2 = open(FPGA_TEXT_LCD_DEVICE,O_RDWR);   //text_lcd_device
    dev3 = open(FPGA_PUSH_SWITCH_DEVICE,O_RDWR);
    dev4 = open(FPGA_STEP_MOTOR_DEVICE,O_RDWR);
    dev5 = open(FPGA_DIP_SWITCH_DEVICE,O_RDWR);
	dev6 = open(LED_DEVICE, O_RDWR);
	dev7 = open(FPGA_DOT_DEVICE, O_RDWR);

    if (dev<0){
        printf("Device open error : %s\n",FND_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & Device node");
        msgBox.exec();
        exit(1);
    }

    if (dev2<0){
        printf("Device open error : %s\n",FPGA_TEXT_LCD_DEVICE);
        msgBox.setText("Device Open Error!");
        msgBox.setInformativeText("Check Module & Device node");
        msgBox.exec();
        exit(1);
    }

    if(dev3<0){
           printf("Device open error:%s\n",FPGA_PUSH_SWITCH_DEVICE);
           msgBox.setText("Device Open Error!");
           msgBox.setInformativeText("Check Module & DEVICE node");
           msgBox.exec();
           exit(1);
       }

       buff_size=sizeof(push_sw_buff);

       if(dev4<0){
              printf("Device open error:%s\n",FPGA_STEP_MOTOR_DEVICE);
              msgBox.setText("Device Open Error!");
              msgBox.setInformativeText("Check Module & DEVICE node");
              msgBox.exec();
              exit(1);
          }

       if(dev5<0){
               printf("Device Open Error : %s\n",FPGA_DIP_SWITCH_DEVICE);
               msgBox.setText("Device Open Error!");
               msgBox.setInformativeText("Check Module & Device node");
               msgBox.exec();
               exit(1);
           }

           if(dev6<0){
               printf("Device2 Open Error : %s\n",LED_DEVICE);
               msgBox.setText("Device2 Open Error!");
               msgBox.setInformativeText("Check Module & Device node");
               msgBox.exec();
               exit(1);
           }
     if (dev7<0){
          printf("Device open error : %s\n",FPGA_DOT_DEVICE);
         msgBox.setText("Device Open Error!");
         msgBox.setInformativeText("Check Module & Device node");
         msgBox.exec();
         exit(1);
      }

       QTimer *timer2 = new QTimer(this);

       connect(timer2, SIGNAL(timeout()),this,SLOT(update_led()));

       timer2->start(100);

       motor_action=0;

       motor_direction=0;

       motor_speed=100;

       speed=150;

       check=false;

       on_Active_BT_toggled(true);

       ui->textEdit_2->setText("0");
       ui->textEdit_2->setAlignment(Qt::AlignCenter);
       i=0;
       on_comboBox_activated(0);
       result=0;
       QString s;
}



void MainWindow::write_fnd_lcd(){

    memset(string,0,sizeof(string));

    //text_lcd
    const char *text_value = "ReadMessageOnFND";
    str_size=strlen(text_value);
    const char *text_value2 = "LEDUsingDip";
    str_size2=strlen(text_value2);
    int retval;
    //fnd
    const char *text_value3 = "0252";

    //text_lcd
    if(str_size>0){
        strncat(string,text_value,str_size);



        memset(string+str_size,' ',LINE_BUFF-str_size);



    }



    if(str_size2>0){



        strncat(string,text_value2,str_size2);



        memset(string+LINE_BUFF+str_size2,' ',LINE_BUFF-str_size2);



    }







    write(dev2,string,MAX_BUFF);    //write to text_lcd





    str_size = (strlen(text_value3));



    for(i=0;i<str_size;i++)



    {



        if((text_value3[i]<0x30)||(text_value3[i]>0x39)){



            printf("Error! Invalid Value!\n");



            msgBox.setText("Error! Invalid Value!");



            msgBox.exec();



            goto fail;



        }



        data[i]=text_value3[i]-0x30;

    }



    retval = write(dev,&data,4);    //write to fnd



    if(retval<0)



    {



        printf("Write Error\n");



        msgBox.setText("Write Error");



        msgBox.exec();



    }







    fail:



        printf("Set Value");

}



void MainWindow::on_Active_BT_toggled(bool checked)



{



    if(checked)



    {



        printf("Start Step Motor\n");



        memset(motor_state,0,sizeof(motor_state));



        motor_action=1;



        motor_state[0]=(unsigned char)motor_action;



        motor_state[1]=(unsigned char)motor_direction;



        motor_state[2]=(unsigned char)motor_speed;



        write(dev4,motor_state,3);



    }



    else



    {



        printf("Stop Step Motor\n");



        memset(motor_state,0,sizeof(motor_state));



        motor_action=0;



        motor_state[0]=(unsigned char)motor_action;



        motor_state[1]=(unsigned char)motor_direction;



        motor_state[2]=(unsigned char)motor_speed;



        write(dev4,motor_state,3);



    }



}







void MainWindow::on_Direction_BT_toggled(bool checked)



{



    if(checked)



    {



        printf("Left \n");



        memset(motor_state,0,sizeof(motor_state));



        motor_direction=1;



        motor_state[0]=(unsigned char)motor_action;



        motor_state[1]=(unsigned char)motor_direction;



        motor_state[2]=(unsigned char)motor_speed;



        write(dev4,motor_state,3);



    }



    else



    {



        printf("RIGHT \n");



        memset(motor_state,0,sizeof(motor_state));



        motor_direction=0;



        motor_state[0]=(unsigned char)motor_action;



        motor_state[1]=(unsigned char)motor_direction;



        motor_state[2]=(unsigned char)motor_speed;



        write(dev4,motor_state,3);



    }



}







void MainWindow::on_Speed_Bar_sliderMoved(int position)



{



    printf("Change Value %d \n",position);



    QString Value = QString::number(position);



    memset(motor_state,0,sizeof(motor_state));



    motor_speed = position;



    motor_state[0]=(unsigned char)motor_action;



    motor_state[1]=(unsigned char)motor_direction;



    motor_state[2]=(unsigned char)motor_speed;



    write(dev4,motor_state,3);



}



void MainWindow::update_step_motor()
{

    read(dev3, &push_sw_buff,buff_size);



    for(i=0;i<MAX_BUTTON;i++)



    {



        if(push_sw_buff[i]==1)



        {



            switch(i)



            {



            case 0:
                break;
            case 1:
                b=1;

                if(speed<250&&speed>0){



                    speed=speed+50;


                }else{



                    speed=250;



                }


                printf("%d",speed);



                break;



            case 2:


                break;



            case 3:



                on_Direction_BT_toggled(false);
                break;



            case 4:


                check=!check;

                a=1;

                break;



            case 5:

                c=1;

                on_Direction_BT_toggled(true);

                break;



            case 6:


                break;



            case 7:


                if(speed<=250&&speed>0){



                    speed-=50;



                }else{



                    speed=50;



                }


                printf("%d",speed);



                break;



            case 8:


                break;



            }



        }



        on_Speed_Bar_sliderMoved(speed);



        on_Active_BT_toggled(check);

    }

    if(a+b+c==3&&flag2==1){
        result+=40;
        s = QString::number(result);
        ui->label_2->setText(s);
        flag2=0;
    }



}



void MainWindow::update_led()



{



    int dip[8];



    int i;



    read(dev5,&dip_sw_buff,1);







    for(i=0;i<8;i++){



        dip[i] = dip_sw_buff%2;



        dip_sw_buff = dip_sw_buff/2;



    }







    if(dip[0]==1){ // off



        data2[0] = 0;



    }



    else{ // on



        data2[0] = 128;



    }







    if(dip[1]==1){



        data2[1] = 0;



    }



    else{



        data2[1] = 64;



    }



    if(dip[2]==1){



        data2[2] = 0;



    }



    else{



        data2[2] = 32;



    }



    if(dip[3]==1){



        data2[3] = 0;



    }



    else{



        data2[3] = 16;



    }



    if(dip[4]==1){



        data2[4] = 0;



    }



    else{



        data2[4] = 8;



    }



    if(dip[5]==1){



        data2[5] = 0;



    }



    else{



        data2[5] = 4;



    }



    if(dip[6]==1){



        data2[6] = 0;



    }



    else{



        data2[6] = 2;



    }



    if(dip[7]==1){



        data2[7] = 0;



    }



    else{



        data2[7] = 1;



    }







    for(int i = 0; i < 8; i++){



       sum += data2[i];



    }



    write_data = sum;

    write(dev6, &write_data, 1);


    if(sum==252&&flag==1){
        result+=30;
        s = QString::number(result);
        ui->label_2->setText(s);
        flag=0;
    }

    sum = 0;



}



MainWindow::~MainWindow()



{



    delete ui;



}


void MainWindow::on_Up_BT_clicked()
{
    j++;
    if(j>9)
        j=0;
    char dot_value[1];
    itoa(j,dot_value,10);
    ui->textEdit_2->setText(dot_value);
    ui->textEdit_2->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_Down_BT_clicked()
{
    j--;
    if(j<0)
        j=9;
    char dot_value[1];
    itoa(j,dot_value,10);
    ui->textEdit_2->setText(dot_value);
    ui->textEdit_2->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_Set_BT_clicked()
{
    set_num = j;

    if(set_num<0 || set_num>9)
    {
        printf("Invalid Number (0~9)! \n");
        msgBox.setText("Invalid Number (0~9)");
        msgBox.exec();
        goto fail;
    }

    str_size = sizeof(fpga_number[set_num]);
    write(dev7,fpga_number[set_num],str_size);

    if(set_num==9){
        result+=30;
        s = QString::number(result);
        ui->label_2->setText(s);
    }
    fail:
        printf("retry");
}

void MainWindow::strreverse(char* begin, char* end)
{
    char aux;
    while(end>begin)
        aux=* end, *end --=* begin, *begin ++=aux;
}

void MainWindow::itoa(int value,char* str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr = str;
    int sign;

    if(base<2 || base>35)
    {
        *wstr = '\0';
        return;
    }

    if((sign=value) < 0)
        value = -value;
    do
        *wstr++ = num[value%base];

    while(value /= base);

    if(sign < 0)
        *wstr ++='_';
    *wstr = '\0';

    strreverse(str,wstr-1);
}



void MainWindow::on_Close_BT_clicked()
{
    this->close();
}



void MainWindow::on_comboBox_activated(int index)
{
    ui->comboBox->setVisible(false);
    page = index;
    if(index ==0)
    {
        ui->textEdit->setFontPointSize(30);
        ui->textEdit->setText("Let's start game!!");
        ui->dial->setVisible(false);
        ui->textEdit_5->setVisible(false);
        setting(false);
        flag=0;
    }else if(index ==1)
    {
        ui->textEdit->setText("Read Message on Text LCD");
        write_fnd_lcd();
        ui->textEdit_3->setVisible(false);
        ui->dial->setVisible(false);
        setting(false);
        flag=1;
    }else if(index ==2)
    {
        ui->textEdit->setText("If you got some number on LED, write to dot!");
        ui->textEdit_2->setFontPointSize(60);
        ui->textEdit_3->setVisible(false);
        ui->dial->setVisible(false);
        setting(true);
        flag=0;
    }else{
        ui->textEdit->setText("press push button");
        ui->textEdit_3->setVisible(false);
        setting(false);
        ui->dial->setVisible(true);
        ui->textEdit_5->setVisible(true);
        ui->textEdit_5->setFontPointSize(15);
        ui->textEdit_5->setText("The ball spins quickly to the right!");
        ui->pushButton->setVisible(false);
        flag2=1;

        QTimer *timer = new QTimer(this);



        connect(timer, SIGNAL(timeout()),this,SLOT(update_step_motor()));



        timer->start(100);
        dial();
    }
    ui->label->setText(ui->textEdit_3->toPlainText());

    ui->textEdit->setAlignment(Qt::AlignCenter);
}

void MainWindow::setting(bool boolean){
    ui->textEdit_2->setVisible(boolean);
    ui->Up_BT->setVisible(boolean);
    ui->Down_BT->setVisible(boolean);
    ui->Set_BT->setVisible(boolean);
}

void MainWindow::on_pushButton_clicked()
{
    on_comboBox_activated(page+1);
}

void MainWindow::dial(){
    ui->dial->setNotchesVisible(true);
    ui->dial->setValue(10);


    /*for(k=0;k<100;k++){
       //ui->dial->setValue(k);
      // ui->dial->sliderMoved(k);
      // ui->dial->SliderPageStepAdd(k);
        //ui->dial->setValue(k);
     // ui->dial->setPageStep(k);

    }*/
}
