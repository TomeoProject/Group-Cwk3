//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QTime>
#include <QWidget>
#include <QToolButton>
#include <QBoxLayout>
#include <QScrollArea>
#include <iostream>
using namespace std;
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QVideoProbe;
class QVideoWidget;


//full screen mode implementation
class VideoWidget : public QVideoWidget {
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};


class ThePlayer : public QMediaPlayer {

    Q_OBJECT


private:
    QMediaPlayer::State playerState;
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;

    QMediaPlayer::State playing_state = QMediaPlayer::PlayingState;
    QTimer* mTimer;
    long updateCount = 0;
    bool playerMuted = false; //boolean that gives indicator whether mute is on/off

public:

    bool isMuted() const; //gives indicator whether mute is on/off
    bool isPlayerAvailable() const;

    QMediaPlayer *player;


    QPushButton *m_fullScreenButton; //allows the video to click

    QSlider *slider;                //< progress indicator slider
    QLabel *durationLabel;          //< progress text indicator label

    ThePlayer() : QMediaPlayer(NULL) {

        player = new QMediaPlayer(this);


        //< Initialize slider and duration label
        slider  = new QSlider(Qt::Horizontal, nullptr);
        durationLabel = new QLabel("--:--/--:--");
        slider->setRange(0,0);

        setVolume(0); // be slightly less annoying

        //< connecting signals and slots which update the playback position, slider and duration label
        connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
        connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(durationUpdated(qint64)));
        connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    }


    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QString composeDurationLabel(int progress, int duration); //< compose QString for duration label widget

signals:
    void fullScreenChanged(bool fullScreen);

private slots:

    //mute/unmute called
    void muteClicked();
    //play/pause called
    void playClicked();

public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void seek(int seconds);                         //< seek within the video
    void durationUpdated(qint64 msec);              //< load new duration
    void positionChanged(qint64 progress);          //< update position

    void toStop (); //slot to stop
    void toPlay (); //slot to play/pause

    //mute and unmute happens here
    void muted(bool muted);

};


#endif //CW2_THE_PLAYER_H
