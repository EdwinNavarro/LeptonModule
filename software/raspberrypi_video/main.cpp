#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "MyLabel.h"

const int SCALING_FACTOR = 2;

void printUsage(char *cmd) {
        char *cmdname = basename(cmd);
	printf("Usage: %s [OPTION]...\n"
               " -h      display this help and exit\n"
               " -cm x   select colormap\n"
               "           1 : rainbow\n"
               "           2 : grayscale\n"
               "           3 : ironblack [default]\n"
               " -tl x   select type of Lepton\n"
               "           2 : Lepton 2.x [default]\n"
               "           3 : Lepton 3.x\n"
               "               [for your reference] Please use nice command\n"
               "                 e.g. sudo nice -n 0 ./%s -tl 3\n"
               " -ss x   SPI bus speed [MHz] (10 - 30)\n"
               "           20 : 20MHz [default]\n"
               " -min x  override minimum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -min 30000\n"
               " -max x  override maximum value for scaling (0 - 65535)\n"
               "           [default] automatic scaling range adjustment\n"
               "           e.g. -max 32000\n"
               " -d x    log level (0-255)\n"
               "", cmdname, cmdname);
	return;
}


int main( int argc, char **argv )
{
	int typeColormap = 3; // colormap_ironblack
	int typeLepton = 2; // Lepton 2.x
	int spiSpeed = 20; // SPI bus speed 20MHz
	int rangeMin = -1; //
	int rangeMax = -1; //
	int loglevel = 0;
	for(int i=1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printUsage(argv[0]);
			exit(0);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			int val = 3;
			if ((i + 1 != argc) && (strncmp(argv[i + 1], "-", 1) != 0)) {
				val = std::atoi(argv[i + 1]);
				i++;
			}
			if (0 <= val) {
				loglevel = val & 0xFF;
			}
		}
		else if ((strcmp(argv[i], "-cm") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((val == 1) || (val == 2)) {
				typeColormap = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-tl") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if (val == 3) {
				typeLepton = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-ss") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((10 <= val) && (val <= 30)) {
				spiSpeed = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-min") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMin = val;
				i++;
			}
		}
		else if ((strcmp(argv[i], "-max") == 0) && (i + 1 != argc)) {
			int val = std::atoi(argv[i + 1]);
			if ((0 <= val) && (val <= 65535)) {
				rangeMax = val;
				i++;
			}
		}
	}

	//create the app
	QApplication a( argc, argv );
	
	QWidget *myWidget = new QWidget;
	myWidget->setGeometry(400, 300, SCALING_FACTOR*340, SCALING_FACTOR*400);

	

	//create an image placeholder for myLabel
	//fill the top left corner with red, just bcuz
	QImage myImage;
	myImage = QImage(SCALING_FACTOR*320, SCALING_FACTOR*240, QImage::Format_RGB888);
	QRgb red = qRgb(255,0,0);
	for(int i=0;i<80;i++) {
		for(int j=0;j<60;j++) {
			myImage.setPixel(i, j, red);
		}
	}

	//create a label, and set it's image to the placeholder
	MyLabel myLabel(myWidget);
	myLabel.setGeometry(SCALING_FACTOR*10, SCALING_FACTOR*10, SCALING_FACTOR*320, SCALING_FACTOR*240); //*** MODIFY TO INCREASE IMAGE SIZE
	myLabel.setPixmap(QPixmap::fromImage(myImage));

	
	
	//create a FFC button
//	QPushButton *button1 = new QPushButton("Perform FFC", myWidget);
//	button1->setGeometry(320/2-50, 290-35, 100, 30);
	
	//QString text = QString("Temp %1").arg(1);
	//QLabel *myString = new QLabel(text, myWidget);
	//myString->setFrameStyle(QFrame::Panel);
	//myString->setGeometry(SCALING_FACTOR*50, SCALING_FACTOR*305,SCALING_FACTOR*80,SCALING_FACTOR*30);

	MyLabel tempUpd(myWidget);
	tempUpd.setGeometry(SCALING_FACTOR*50 , SCALING_FACTOR*305, SCALING_FACTOR*80, SCALING_FACTOR*30);
	tempUpd.setFrameStyle(QFrame::Panel);

	QPushButton *button_up = new QPushButton("^", myWidget);
	button_up->setGeometry(SCALING_FACTOR*(320 -100),SCALING_FACTOR*( 255), SCALING_FACTOR*30 , SCALING_FACTOR*30);

	QPushButton *button_left = new QPushButton("<", myWidget);
	button_left->setGeometry(SCALING_FACTOR*(320 -150), SCALING_FACTOR*305, SCALING_FACTOR*30 , SCALING_FACTOR*30);

	QPushButton *button_right = new QPushButton(">", myWidget);
	button_right->setGeometry(SCALING_FACTOR*(320 -50), SCALING_FACTOR*305, SCALING_FACTOR*30 , SCALING_FACTOR*30);

	QPushButton *button_down = new QPushButton("v", myWidget);
	button_down->setGeometry(SCALING_FACTOR*(320 -100), SCALING_FACTOR*355, SCALING_FACTOR*30 , SCALING_FACTOR*30);

	QPushButton *button_valve = new QPushButton("Blast", myWidget);
	button_valve->setGeometry(SCALING_FACTOR*50, SCALING_FACTOR*255, SCALING_FACTOR*80 , SCALING_FACTOR*30);

	//FIREBOT LABELS
	//QLabel *temp_label = new QLabel("Temperature", myWidget);
	//temp_label->setGeometry(SCALING_FACTOR*50 , SCALING_FACTOR*305, SCALING_FACTOR*80, SCALING_FACTOR*30);


	//create a thread to gather SPI data
	//when the thread emits updateImage, the label should update its image accordingly
	LeptonThread *thread = new LeptonThread();
	thread->setLogLevel(loglevel);
	thread->useColormap(typeColormap);
	thread->useLepton(typeLepton);
	thread->useSpiSpeedMhz(spiSpeed);
	thread->setAutomaticScalingRange();
	if (0 <= rangeMin) thread->useRangeMinValue(rangeMin);
	if (0 <= rangeMax) thread->useRangeMaxValue(rangeMax);
	QObject::connect(thread, SIGNAL(updateImage(QImage)), &myLabel, SLOT(setImage(QImage)));
	
	////connect ffc button to the thread's ffc action
	//QObject::connect(button1, SIGNAL(clicked()), thread, SLOT(performFFC()));
	

	//FIREBOT BUTTONS

	//Button up
	QObject::connect(button_up, SIGNAL(pressed()), thread, SLOT(moveUp()));
	QObject::connect(button_up, SIGNAL(released()), thread, SLOT(stopPitch()));

	//button down
	QObject::connect(button_down, SIGNAL(pressed()), thread, SLOT(moveDown()));
	QObject::connect(button_down, SIGNAL(released()), thread, SLOT(stopPitch()));

	//button left
	QObject::connect(button_left, SIGNAL(pressed()), thread, SLOT(moveLeft()));
	QObject::connect(button_left, SIGNAL(released()), thread, SLOT(stopYaw()));

	//button right
	QObject::connect(button_right, SIGNAL(pressed()), thread, SLOT(moveRight()));
	QObject::connect(button_right, SIGNAL(released()), thread, SLOT(stopYaw()));

	//button valve
	QObject::connect(button_valve, SIGNAL(pressed()), thread, SLOT(openValve()));
	QObject::connect(button_valve, SIGNAL(released()), thread, SLOT(closeValve()));

	QObject::connect(thread, SIGNAL(valueChanged(QString)), &tempUpd, SLOT(updTemp(QString)));
	//	temp_label->setText(QString("Value: %1").arg(tempValue));;
   // });

	thread->start();
	
	myWidget->show();

    QTimer *timer = new QTimer(myWidget);
    QObject::connect(timer, SIGNAL(timeout()), thread, SLOT(tempUpd()));
    timer->start(500); // Change value every .5 second



	return a.exec();
}

