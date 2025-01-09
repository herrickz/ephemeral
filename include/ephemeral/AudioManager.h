#pragma once

#include <string>
#include <thread>
#include <rtaudio/RtAudio.h>


/*
 * AudioManager
 *
 * - Play sounds using any mp3 or wav file
 * - Note: All sound files must be stereo
 */
class AudioManager {

public:

    static AudioManager &GetInstance();

    /*
     * Plays a sound on a specified channel.
     *
     *      If there is currrently a sound playing on that channel
     *      it will interrupt it and immediately play the new sound
     *
     * @param filePath: File path to either mp3 or wav file
     * @param channel: The channel to play the sound on
     */
    void Play(const std::string &filePath, int channel);

    /*
     * Stop all sounds playing and terminate AudioManager thread
     *
     */
    void StopAll();

    /*
     * Returns atomic boolean of whether or not the user has
     * requested all audio to stop playing
     *
     */
    bool ShouldStopAllAudio();

private:

    AudioManager();

    static AudioManager &mAudioManager;
    std::atomic<bool> mShouldStopAllAudio = false;
    RtAudio mRtAudio;
    std::thread *mAudioThread;
};