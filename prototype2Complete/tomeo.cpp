/*
 *
 *    ______
 *   /_  __/___  ____ ___  ___  ____
 *    / / / __ \/ __ `__ \/ _ \/ __ \
 *   / / / /_/ / / / / / /  __/ /_/ /
 *  /_/  \____/_/ /_/ /_/\___/\____/
 *              video for sports enthusiasts...
 *
 *  2811 cw3 : twak
 */

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QSlider>
#include <QLabel>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"
#include "QScrollArea"


using namespace std;

// read in videos and thumbnails to this directory
vector<TheButtonInfo> getInfoIn (string loc) {

    vector<TheButtonInfo> out =  vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

        if (f.contains("."))

#if defined(_WIN32)
            if (f.contains(".wmv"))  { // windows
#else
            if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

                QString thumb = f.left( f .length() - 4) +".png";
                if (QFile(thumb).exists()) { // if a png thumbnail exists
                    QImageReader *imageReader = new QImageReader(thumb);
                    QImage sprite = imageReader->read(); // read the thumbnail
                    if (!sprite.isNull()) {
                        QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                        QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else
                        qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
                }
                else
                    qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
            }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::question(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! download, unzip, and add command line argument to \"quoted\" file location. Download videos from Tom's OneDrive?"),
                    QMessageBox::Yes |
                    QMessageBox::No );

        switch( result )
        {
        case QMessageBox::Yes:
            QDesktopServices::openUrl(QUrl("https://leeds365-my.sharepoint.com/:u:/g/personal/scstke_leeds_ac_uk/EcGntcL-K3JOiaZF4T_uaA4BHn6USbq2E55kF_BTfdpPag?e=n1qfuN"));
            break;
        default:
            break;
        }
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    //< Add the slider and duration(progress) label of the player to a QHBoxLayout.
    QWidget *sliderArea = new QWidget();
    sliderArea->setFixedHeight(40);
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(player->slider);
    sliderLayout->addWidget(player->durationLabel);
    sliderArea->setLayout(sliderLayout);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the buttons
    for ( uint i = 0; i < videos.size(); i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        QVBoxLayout* lay = new QVBoxLayout;
        lay->addWidget(button);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo* ))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addLayout(lay);
        button->init(&videos.at(i));
    }
    //creates a horizontal scroll area for the buttons above
    QScrollArea* scroll = new QScrollArea;
    scroll->setWidget(buttonWidget);
    scroll->setMinimumHeight(150);
    scroll->setMaximumHeight(150);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
     //creates a layout for play/pause and mute etc
    QHBoxLayout* playback = new QHBoxLayout;

    //creating play/pause button
    QPushButton *playButton = new QPushButton();
    playButton -> setText("Play/Pause"); //creating text for the button
    playButton->connect(playButton, SIGNAL(clicked()), player, SLOT (toPlay()));
    playback->addWidget(playButton);
    //creating stop button
    QPushButton *stopButton = new QPushButton();
    stopButton->setText("Restart"); //creating text for the button
    stopButton->connect(stopButton, SIGNAL(clicked()), player, SLOT (toStop()));
    playback->addWidget(stopButton);

    //creating a mute button
    QPushButton* muteButton = new QPushButton();
    muteButton->setText("mute");
    muteButton->connect(muteButton, SIGNAL(clicked()), player, SLOT(muteClicked()));
    playback->addWidget(muteButton);

    //creating a slider for the volume
    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setFixedWidth(100);
    volumeSlider->setValue(0); //sets the volume to zero
    volumeSlider->connect(volumeSlider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));
    playback->addWidget(volumeSlider);



    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);



    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(sliderArea); //< Out progress indicator
    top->addLayout(playback); // adds stop, play, stop mute button and volume silder
    top->addWidget(scroll); //allows the




    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
