//
// Created by twak on 11/11/2019.
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "the_button.h"
#include <vector>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include <QTime>
#include <QWidget>
#include <QToolButton>
using namespace std;
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QVideoProbe;
class QVideoWidget;

class ThePlayer : public QMediaPlayer {

    Q_OBJECT

private:
    vector<TheButtonInfo>* infos;
    vector<TheButton*>* buttons;

    QMediaPlayer::State pstate = QMediaPlayer::PlayingState;
    QTimer* mTimer;
    long updateCount = 0;
    bool playerMuted = false; //boolean that gives indicator whether mute is on/off


public:
    QSlider *slider;                //< progress indicator slider
    QLabel *durationLabel;          //< progress text indicator label
    ThePlayer() : QMediaPlayer(NULL) {

        //< Initialize slider and duration label
        slider  = new QSlider(Qt::Horizontal, nullptr);
        durationLabel = new QLabel("--:--/--:--");
        slider->setRange(0,0);

        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );

        //< connecting signals and slots which update the playback position, slider and duration label
        connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
        connect(this, SIGNAL(durationChanged(qint64)), this, SLOT(durationUpdated(qint64)));
        connect(this, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));

    }


    // all buttons have been setup, store pointers here
    void setContent(vector<TheButton*>* b, vector<TheButtonInfo>* i);

    QString composeDurationLabel(int progress, int duration); //< compose QString for duration label widget
    bool isMuted() const; //gives indicator whether mute is on/off

private slots:
    // change the image and video for one button every one second



    void muteClicked();

public slots:
    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void seek(int seconds);                         //< seek within the video
    void durationUpdated(qint64 msec);              //< load new duration
    void positionChanged(qint64 progress);          //< update position

    void toStop (); //slot to stop

    void toPlay (); //slot to play/pause

    void muted(bool muted); // slot with boolean to able mute and unmute

signals:
};

#endif //CW2_THE_PLAYER_H
