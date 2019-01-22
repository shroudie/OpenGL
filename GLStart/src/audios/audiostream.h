#pragma once

#include <iostream>
#include <SFML/Audio/SoundStream.hpp>
#include <SFML/System/Time.hpp>
#include "../maths/fft.h"

#define CHUNK_SIZE 1024

typedef struct  WAV_HEADER
{
	/* RIFF Chunk Descriptor */
	uint8_t         RIFF[4];        // RIFF Header Magic header
	uint32_t        ChunkSize;      // RIFF Chunk Size
	uint8_t         WAVE[4];        // WAVE Header
	/* "fmt" sub-chunk */
	uint8_t         fmt[4];         // FMT header
	uint32_t        Subchunk1Size;  // Size of the fmt chunk
	uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
	uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
	uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
	uint32_t        bytesPerSec;    // bytes per second
	uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
	uint16_t        bitsPerSample;  // Number of bits per sample
	/* "data" sub-chunk */
	uint8_t         Subchunk2ID[4]; // "data"  string
	uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

class AudioStream : public sf::SoundStream
{
public:
	bool open(const char *fp)
	{
		//wav_hdr header;
		//FILE* file = fopen(fp, "r");
		//if (!file)
		//	::exit(-1);
		//fread(&header, 1, sizeof(header), file);
		///*cout << "Sampling Rate              :" << header.SamplesPerSec << endl;
		//cout << "Number of bits used        :" << header.bitsPerSample << endl;
		//cout << "Number of channels         :" << header.NumOfChan << endl;
		//cout << "Number of bytes per second :" << header.bytesPerSec << endl;
		//cout << "Data length                :" << header.Subchunk2Size << endl;
		//cout << "Audio Format               :" << header.AudioFormat << endl;*/
		//initialize(header.NumOfChan, header.SamplesPerSec);
		sf::SoundBuffer sound_buffer;
		sound_buffer.loadFromFile(fp);
		buffers.assign(sound_buffer.getSamples(), sound_buffer.getSamples() + sound_buffer.getSampleCount());
		initialize(sound_buffer.getChannelCount(), sound_buffer.getSampleRate());

		generateFilterVector();
		currentSampleWaveVector.resize(CHUNK_SIZE);

		current_buffer_location = 0;
		return true;
	}

	void generateFilterVector() {
		complex temporaryComplex;
		std::vector<complex> filterShortComplexVector;
		filterShortComplexVector.resize(CHUNK_SIZE / 4);
		for (int i = 0; i < CHUNK_SIZE / 4; i++) {
			temporaryComplex = (i > 0 && i < 1024/4) ? 1.0 : 0.0;
			filterShortComplexVector[i] = temporaryComplex;
		}
	}

	const std::vector<complex> &getCurrentSampleWaveVector() const {
		return currentSampleWaveVector;
	}

private:
	std::vector<sf::Int16> buffers;
	std::vector<complex> currentSampleWaveVector;

	size_t current_buffer_location;

	void getStreamSamples() {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			complex temporaryShortComplex = buffers[current_buffer_location + i];
			currentSampleWaveVector[i] = temporaryShortComplex;
		}
	}

	virtual bool onGetData(Chunk& data)
	{
		getStreamSamples();
		fft::forward(currentSampleWaveVector.data(), CHUNK_SIZE);

		data.samples = &buffers[current_buffer_location];
		data.sampleCount = CHUNK_SIZE;
		current_buffer_location += CHUNK_SIZE;
		return true;
	}
	virtual void onSeek(sf::Time timeOffset)
	{
		// Change the current position in the stream source
	}
};