#include "grpctest.h"
#include "ui_grpctest.h"
#define _WIN32_WINNT 0x0600

#

grpcTest::grpcTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::grpcTest)
{
    ui->setupUi(this);
    setLayout(ui->horizontalLayout_2);

    connect( ui->GetTimeButton, SIGNAL( clicked() ), this, SLOT(GetTimeButton_cliked()) );
    connect( ui->GetName, SIGNAL( clicked() ), this, SLOT(GetName_cliked()) );
    connect( ui->ConvertString, SIGNAL( clicked() ), this, SLOT(ConvertString_cliked()) );

    client= new  GRPCTestClient(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));


}

 grpcTest::~grpcTest()
{
    delete ui;
}


void grpcTest::GetTimeButton_cliked()
{
     QString time("now");
     try
     {
         std::string reply =client->GetTime(time.toStdString());
          ui->label->setText(QString::fromStdString(reply));

     }

     catch(...)
     {
        return;
     }

}

void grpcTest::GetName_cliked()
{
     QString name("Who");
     std::string reply = client->GetServerName(name.toStdString());

     ui->label_2->setText(QString::fromStdString(reply));

}
void grpcTest::ConvertString_cliked()
{
     QString conv(ui->lineEdit->text());

     std::string reply = client->Convert(conv.toStdString());

     ui->label_3->setText(QString::fromStdString(reply));
    ;
}
