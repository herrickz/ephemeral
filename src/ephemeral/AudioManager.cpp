
#include <ephemeral/AudioManager.h>
#include <ephemeral/Logger.h>

#include <mpg123.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include <algorithm>
#include <map>

std::mutex bufferMutex;

// PCM Buffers and Volume Control for each MP3 file
std::vector<std::vector<short>> pcmBuffers;
std::map<int, float> volumeLevels; // Map of stream index to volume level

int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                  double streamTime, RtAudioStreamStatus status, void *userData) {

    // Convert the passed in userData void* to the vector of vectors which contain
    // the PCM data
    std::vector<std::vector<short>> *buffers = (std::vector<std::vector<short>> *)userData;
    short *output = (short *)outputBuffer;

    // Fill the output buffer with double the amount of frames
    // for two channel audio with silence
    std::fill(output, output + nBufferFrames * 2, 0);

    std::lock_guard<std::mutex> lock(bufferMutex);

    // Loop through all of the buffers and combine their audio
    for (size_t streamIndex = 0; streamIndex < buffers->size(); ++streamIndex) {

        auto &buffer = (*buffers)[streamIndex];
        float volume = volumeLevels[streamIndex];

        // If there isnt enough data, just let it be silent
        if (buffer.size() < nBufferFrames * 2) {
            continue;
        }

        // Mix PCM data from each buffer with volume adjustment
        for (unsigned int i = 0; i < nBufferFrames * 2; ++i) {
            int sample = output[i] + static_cast<int>(buffer[i] * volume);

            // I don't think this is necessary since short signed data will always
            // be within this range
            output[i] = std::clamp(sample, -32768, 32767); // Avoid overflow
        }

        // Remove used PCM data
        buffer.erase(buffer.begin(), buffer.begin() + nBufferFrames * 2);
    }

    // TODO: handle audio stop if necessary
    return 0;
}

void LoadMp3PcmData(std::string filePath, std::vector<short> &pcmData, long &rate, int &channels) {

    // TODO: Could consider moving the mpg123 lifecycle to outside
    // of this function to improve performance
    mpg123_init();
    mpg123_handle* mpgHandle = mpg123_new(nullptr, nullptr);

    if (mpg123_open(mpgHandle, filePath .c_str()) != MPG123_OK) {
        // TODO: Log an error but don't exit the entire application
        std::cerr << "Failed to open MP3 file." << std::endl;
        mpg123_delete(mpgHandle);
        exit(1);
    }

    int encoding;

    mpg123_getformat(mpgHandle, &rate, &channels, &encoding);

    if (encoding != MPG123_ENC_SIGNED_16) {
        // TODO: Log an error but don't exit the entire application
        std::cerr << "Unsupported encoding. Expected 16-bit signed PCM." << std::endl;
        mpg123_close(mpgHandle);
        mpg123_delete(mpgHandle);
        exit(1);
    }

    std::vector<unsigned char> buffer(4096);
    size_t bytesRead;

    while (mpg123_read(mpgHandle, buffer.data(), buffer.size(), &bytesRead) == MPG123_OK) {
        size_t samplesDecoded = bytesRead / sizeof(short);
        pcmData.insert(pcmData.end(), (short *)buffer.data(), (short *)buffer.data() + samplesDecoded);
    }

    mpg123_close(mpgHandle);
    mpg123_delete(mpgHandle);
    mpg123_exit();
}

void LoadWavePcmData(std::string filePath, std::vector<short> &pcmData) {

    std::ifstream file(filePath);

    // Seek to the end to find the file size
    file.seekg(0, std::ios::end);

    // Get the file size and subtract 44 bytes for only data section
    // WAV files contain 44 bytes of header information
    int fileSize = static_cast<int>(file.tellg()) - 44;

    // Seek the file pointer 44 bytes from the beginning to skip
    // over the header
    file.seekg(44, std::ios::beg);

    if (fileSize % sizeof(short) != 0) {
        std::cerr << "Error: File size is not a multiple of short (16-bit)" << std::endl;
    } else {
        pcmData.resize(fileSize / sizeof(short));

        file.read(reinterpret_cast<char *>(pcmData.data()), fileSize);
    }

}

void PlayAudio(RtAudio &audio) {

    // Hardcode to 2-channel (Stereo) audio only
    RtAudio::StreamParameters outputParams;
    outputParams.deviceId = audio.getDefaultOutputDevice();
    outputParams.nChannels = 2;
    outputParams.firstChannel = 0;

    unsigned int bufferFrames = 512;

    try {

        RtAudioErrorType error;

        error = audio.openStream(&outputParams, nullptr, RTAUDIO_SINT16, 44100, &bufferFrames, &audioCallback, &pcmBuffers);

        if(error != RTAUDIO_NO_ERROR) {
            throw std::runtime_error("Error with opening audio stream");
        }

        error = audio.startStream();

        if(error != RTAUDIO_NO_ERROR) {
            throw std::runtime_error("Error starting audio stream");
        }

        // Run the audio stream and only break out when someone
        // invokes StopAllAudio()
        while (!AudioManager::GetInstance().ShouldStopAllAudio()) {

        }

        // Clean up audio stream
        if(audio.isStreamRunning()) {
            audio.abortStream();
        }

        audio.closeStream();

    } catch(std::runtime_error &exception) {
        std::cerr << exception.what() << std::endl;
        exit(1);
    }
    catch (...) {
        LOG_F("RtAudio error");
        exit(1);
    }

}

AudioManager::AudioManager() {

    // 3 channel volume levels
    volumeLevels[0] = 0.5f;
    volumeLevels[1] = 1.0f;
    volumeLevels[2] = 1.0f;

    // background music 1
    pcmBuffers.emplace_back();

    // background music 2
    pcmBuffers.emplace_back();

    // Sound effects
    pcmBuffers.emplace_back();

    // Initialize the audio thread to start playing audio
    mAudioThread = new std::thread(PlayAudio, std::ref(mRtAudio));
}

bool AudioManager::ShouldStopAllAudio() {
    return mShouldStopAllAudio;
}

AudioManager &AudioManager::GetInstance() {
    static AudioManager mAudioManager;
    return mAudioManager;
}

void AudioManager::Play(const std::string &filePath, int channel) {
    std::lock_guard<std::mutex> lock(bufferMutex);

    long rate;
    int channels;

    // Clear the PCM buffer at that channel stopping the audio
    pcmBuffers[channel].clear();

    // Load the PCM data for whatever file format it is
    if(filePath.find(".mp3") != std::string::npos) {
        LoadMp3PcmData(filePath, pcmBuffers[channel], rate, channels);
    } else if(filePath.find(".wav") != std::string::npos) {
        LoadWavePcmData(filePath, pcmBuffers[channel]);
    }

}

void AudioManager::StopAll() {

    // This sets the boolean atomically
    mShouldStopAllAudio = true;

    // Wait for the AudioThread to die and then delete it
    if (mAudioThread && mAudioThread->joinable()) {
        mAudioThread->join();
        delete mAudioThread;
    }

    mAudioThread = nullptr;
}
