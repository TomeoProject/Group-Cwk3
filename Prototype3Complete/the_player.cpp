//
// Created by twak on 11/11/2019.
//

#include "the_player.h"
#include "the_button.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
    play();


}
//either pause or calls the toPlay button to play/pause the video
void ThePlayer::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit toPlay();
        break;
    case QMediaPlayer::PlayingState:
        pause();
        break;
    }
}

//shows what mute is on or not
bool ThePlayer::isMuted() const
{
    return playerMuted;
}
//changes the mute bool and set mute or unmute
void ThePlayer::muted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;
        if(muted==true){
            setMuted(true);
        }
        if(muted==false){
            setMuted(false);
        }
    }
}

void ThePlayer::muteClicked()
{
    muted(!isMuted());
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();


}
//play/pause video
void ThePlayer::toPlay() {
    switch (playing_state) {
    case QMediaPlayer::State::PlayingState:
        pause(); // starting playing again...
        playing_state = QMediaPlayer::State::PausedState;
        break;
    case QMediaPlayer::State::PausedState:
        setPlaybackRate(1);
        play(); // starting playing again...
        playing_state = QMediaPlayer::State::PlayingState;
        break;
    case QMediaPlayer::State::StoppedState:
        setPlaybackRate(1);
        play(); // starting playing again...
        playing_state = QMediaPlayer::State::PlayingState;
        break;
    default:
        break;
    }
}
//restart the viso playing
void ThePlayer::toStop() {
    stop();
    playing_state = QMediaPlayer::State::StoppedState;
}

//< Progress slider and label related slots and function implementation
void ThePlayer::seek(int seconds){
    setPosition(seconds*1000); //< set the position to where the user want's to jump
}

void ThePlayer::durationUpdated(qint64 msec){
    slider->setMaximum(msec/1000); //< update the range of the slider everytime duration changes
}

void ThePlayer::positionChanged(qint64 progress){
    //< Update the slider and duration label progress indicators
    durationLabel->setText(composeDurationLabel(progress/1000, duration()/1000));
    slider->setSliderPosition(progress/1000);
}


QString ThePlayer::composeDurationLabel(int prog, int duration){
    QString str = "--:-- / --:--";  //< default string

    if (duration){
        //< Set string to mm:ss or hh:mm:ss depending on the length of the video
        QTime progress(prog/3600, (prog/60)%60, prog%60, 0);
        QTime max(duration/3600, (duration/60)%60, duration%60, 0);
        QString format = duration <= 3600 ? "mm:ss": "hh:mm:ss";
        str = progress.toString(format) + " / " + max.toString(format);
    }
    return str;
}


//Full Screen function implementation

bool ThePlayer::isPlayerAvailable() const {
    return player->isAvailable();
}

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

}
//Allows the user to escape the full screen
void VideoWidget::keyPressEvent(QKeyEvent *event) {
    if (event -> key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        event->accept();
    } else if(event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
        setFullScreen(!isFullScreen());
        event->accept();
    } else {
        QVideoWidget::keyPressEvent(event);
    }
}
//allows the video player to accept double clicks event
void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    setFullScreen(!isFullScreen());
    event->accept();
}
//allows the video player to accept mouse input
void VideoWidget::mousePressEvent(QMouseEvent *event) {
    QVideoWidget::mousePressEvent(event);
}

