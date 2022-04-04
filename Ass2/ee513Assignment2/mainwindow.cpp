#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include <iostream>

MainWindow *handle;

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("ee513/CPUTemp");
    ui->comboBox->addItem("ee513/Pitch/Roll");
    ui->comboBox_2->addItem("Temperature");
    ui->comboBox_2->addItem("Pitch");
    ui->comboBox_2->addItem("Roll");

    this->count = 50;
    this->time = 0;
    this->setWindowTitle("EE513 Assignment 2");
    this->ui->customPlot->addGraph();
    this->ui->customPlot->yAxis->setLabel("Pitch (degrees)");
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    this->ui->customPlot->xAxis->setTicker(timeTicker);
    this->ui->customPlot->yAxis->setRange(-50,50);
    this->ui->customPlot->replot();
    QObject::connect(this, SIGNAL(messageSignal(QString)),
                     this, SLOT(on_MQTTmessage(QString)));
    ::handle = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update(){
    // For more help on real-time plots, see: http://www.qcustomplot.com/index.php/demos/realtimedatademo
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    ui->customPlot->graph(0)->addData(key,count);
    ui->customPlot->graph(0)->rescaleKeyAxis(true);
    ui->customPlot->replot();
    QString text = QString("Value added is %1").arg(this->count);
    ui->outputEdit->setText(text);
}

void MainWindow::on_downButton_clicked()
{
    this->count-=10;
    this->update();
}

void MainWindow::on_upButton_clicked()
{
    this->count+=10;
    this->update();
}

void MainWindow::on_connectButton_clicked()
{
    if(ui->comboBox_2->currentText() == "Temperature" && ui->comboBox->currentText()=="ee513/Pitch/Roll"){
           this->ui->outputText->setPlainText("Topic has no information for temperature, switch to ee513/CPUTemp ");

       }
    else
    {

    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    int rc;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;
    opts.username = AUTHMETHOD;
    opts.password = AUTHTOKEN;


    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered)==0){
        ui->outputText->appendPlainText(QString("Callbacks set correctly"));
    }
    if ((rc = MQTTClient_connect(client, &opts)) != MQTTCLIENT_SUCCESS) {
        ui->outputText->appendPlainText(QString("Failed to connect, return code %1").arg(rc));
    }
    ui->outputText->appendPlainText(QString("Subscribing to topic for client " CLIENTID));
int x=0;
qDebug()<<ui->comboBox->currentText();
    if(ui->comboBox->currentText()== "ee513/CPUTemp")
    {
         x = MQTTClient_subscribe(client, TOPIC1, QOS);
    }
    if(ui->comboBox->currentText()== "ee513/Pitch/Roll")
    {
         x = MQTTClient_subscribe(client, TOPIC2, QOS);
    }
    ui->outputText->appendPlainText(QString("Result of subscribe is %1 (0=success)").arg(x));
}
}
void delivered(void *context, MQTTClient_deliveryToken dt) {
    (void)context;
    // Please don't modify the Window UI from here
    qDebug() << "Message delivery confirmed";
    handle->deliveredtoken = dt;
}

/* This is a callback function and is essentially another thread. Do not modify the
 * main window UI from here as it will cause problems. Please see the Slot method that
 * is directly below this function. To ensure that this method is thread safe I had to
 * get it to emit a signal which is received by the slot method below */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    (void)context; (void)topicLen;
    qDebug() << "Message arrived (topic is " << topicName << ")";
    qDebug() << "Message payload length is " << message->payloadlen;
    QString payload;
    payload.sprintf("%s", (char *) message->payload).truncate(message->payloadlen);
    emit handle->messageSignal(payload);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int MainWindow::parseJSONdata(QString str){
   QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
   QJsonObject obj = doc.object();
   QString label;
   if(ui->comboBox_2->currentText() == "Temperature" && ui->comboBox->currentText()=="ee513/CPUTemp")
   {
       this->ui->customPlot->yAxis->setRange(38,50);
   this->temperature = (float) obj["CPUTemp"].toDouble();
   cout << "The temperature is " << temperature  << endl;
   this->count=temperature;
   label="Temperature";
   this->update();
   }
   else if(ui->comboBox_2->currentText() == "Temperature" && ui->comboBox->currentText()=="ee513/Pitch/Roll"){
       this->ui->outputText->setPlainText("Topic has no information for temperature, switch to ee513/CPUTemp ");

   }
   else if (ui->comboBox_2->currentText() == "Pitch")
   {
       this->ui->customPlot->yAxis->setRange(-15,20);
       this->pitch = (float) obj["Pitch"].toDouble();
       cout << "The temperature is " << pitch  << endl;
       this->count=pitch;
       label="Pitch";
       this->update();
   }
   else if (ui->comboBox_2->currentText() == "Roll")
   {
       this->ui->customPlot->yAxis->setRange(-5,15);
       this->roll = (float) obj["Roll"].toDouble();
       cout << "The temperature is " << roll << endl;
       this->count=roll;
       label="Roll";
       this->update();
   }
   this->ui->customPlot->yAxis->setLabel(label);
   return 0;
}





/** This is the slot method. Do all of your message received work here. It is also safe
 * to call other methods on the object from this point in the code */
void MainWindow::on_MQTTmessage(QString payload){
    ui->outputText->appendPlainText(payload);
    ui->outputText->ensureCursorVisible();

    //ADD YOUR CODE HERE
    parseJSONdata(payload);


}

void connlost(void *context, char *cause) {
    (void)context; (void)*cause;
    // Please don't modify the Window UI from here
    qDebug() << "Connection Lost" << endl;
}

void MainWindow::on_disconnectButton_clicked()
{
    qDebug() << "Disconnecting from the broker" << endl;
    MQTTClient_disconnect(client, 10000);
    //MQTTClient_destroy(&client);
}
