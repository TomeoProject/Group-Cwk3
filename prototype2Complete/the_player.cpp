//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;

    //infoqs = q;
    jumpTo(buttons -> at(0) -> info);
    play();
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
}
// make the video player to play
void ThePlayer::toPlay() {
    switch (pstate) {// change name
    case QMediaPlayer::State::PlayingState:
        pause(); // starting playing again...
        pstate = QMediaPlayer::State::PausedState;
        break;
    case QMediaPlayer::State::PausedState:
        setPlaybackRate(1);
        play(); // starting playing again...
        pstate = QMediaPlayer::State::PlayingState;
        break;
    case QMediaPlayer::State::StoppedState:
        setPlaybackRate(1);
        play(); // starting playing again...
        pstate = QMediaPlayer::State::PlayingState;
        break;
    default:
        break;
    }}
// make the video player to pause
void ThePlayer::toStop() {
    stop();
    pstate = QMediaPlayer::State::StoppedState;
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
//make the video mute/unmute
//function calls Muted and uses a boolean thats is opposite of isMuted
void ThePlayer::muteClicked()
{
    muted(!isMuted());

}
