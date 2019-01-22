//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif
//#endif  // _DEBUG

//#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "audios/audiostream.h"
#include "graphics/shader.h"
#include "maths/fft.h"
#define GODKNOWS "src/GodKnows.wav"

#define WIDTH 1280
#define HEIGHT 720
#define SHADER_FILE_WAVE "src/shaders/Shader_wave.frag"
#define SHADER_FILE_SPECTRUM "src/shaders/Shader_spectrum.frag"
#define SHADER_FILE_EQUALIZER "src/shaders/Shader_equalizer.frag"
#define SHADER_FILE_VERTEX "src/shaders/wave.vert"

enum VISUALIZATION_MODE {
	WAVE, SPECTRUM, EQUALIZER
};

static const int WAVE_DATA_SIZE = 256;
static const int EQUALIZER_COLUMNS = 32;
static const int EQUALIZER_INERTIA = 500;
static const int COLUMNS_MARGIN = 4;

float waveData[WAVE_DATA_SIZE];
float spectrumData[WAVE_DATA_SIZE];
float previousSpectrumData[WAVE_DATA_SIZE];
float barData[EQUALIZER_COLUMNS * 4];
int columnsInertia[EQUALIZER_COLUMNS];
int currentShader = 0;
int currentSong = 0;

sf::SoundBuffer soundBuffer;
AudioStream fftAudioStream;
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLint posAttrib;
GLint timeLoc;
GLint sampleLoc;
GLint waveLoc;
GLint resLoc;
GLuint vao;
GLuint vbo;
GLint lowFilterLoc;
GLint highFilterLoc;
GLuint shaderid;

float shaderVertices[] = {
		-0.6f, -0.6f,
		0.6f, 0.6f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
};
//
//void waveDataVisualization(const std::vector<complex> dataVector) {
//	if (dataVector.data() != NULL) {
//		int picker = FFTAudioStream::SAMPLES_TO_STREAM / WAVE_DATA_SIZE;
//		for (int i = 0; i < WAVE_DATA_SIZE; i++) {
//			waveData[i] += (float)dataVector[i * picker].real;
//			waveData[i] *= 0.0001;
//		}
//	}
//}

void equalizerVisualization(const std::vector<complex> dataVector) {
	if (dataVector.data() != NULL) {
		int picker = 1024 / EQUALIZER_COLUMNS;
		float sum = 0;
		for (int columnNumber = 0; columnNumber < EQUALIZER_COLUMNS; columnNumber++) {
			for (int i = 0; i < EQUALIZER_COLUMNS; i++) {
				sum += dataVector[columnNumber * picker].real;
			}
			sum *= 0.00000015;
			barData[4 * columnNumber] = 2.f / EQUALIZER_COLUMNS * columnNumber + -1.f;
			barData[4 * columnNumber + 1] = 0;
			barData[4 * columnNumber + 2] = 2.f / EQUALIZER_COLUMNS * columnNumber + -1.f;
			barData[4 * columnNumber + 3] = sum;

			/*for (int i = 0; i < columnWidth; i++) {
				int currentIndex = columnWidth * columnNumber + i;
				if (i < columnWidth - COLUMNS_MARGIN) {
					if (sum > previousSpectrumData[currentIndex]) {
						columnsInertia[columnNumber] = EQUALIZER_INERTIA;
						spectrumData[currentIndex] = sum;
					}
					else {
						spectrumData[currentIndex] = previousSpectrumData[currentIndex] -
							previousSpectrumData[currentIndex] *
							((float)EQUALIZER_INERTIA + 1 -
								columnsInertia[columnNumber]) / ((float)EQUALIZER_INERTIA);
					}
				}
				else {
					spectrumData[currentIndex] = 0.0;
				}
			}
			sum = 0;*/
			sum = 0;
		}
	}
}

int main()
{
	///*sf::SoundBuffer buffer;
	//if (!buffer.loadFromFile(GODKNOWS))
	//	return -1;

	//sf::Sound sound;
	//sound.setBuffer(buffer);
	//sound.play();*/
	//AudioStream audio;
	//audio.open(GODKNOWS);
	//audio.play();
	//system("pause");
	glfwInit();
	GLFWwindow *window = glfwCreateWindow(1280, 720, "test", NULL, NULL);
	glfwMakeContextCurrent(window);

	Shader shader;
	shaderid = shader.load_shaders(SHADER_FILE_VERTEX, SHADER_FILE_WAVE);
	glUseProgram(shaderid);

	fftAudioStream.open(GODKNOWS);

	waveLoc = glGetUniformLocation(shaderid, "waveData");
	timeLoc = glGetUniformLocation(shaderid, "iGlobalTime");
	resLoc = glGetUniformLocation(shaderid, "iResolution");

	cout << waveLoc << endl;
	cout << timeLoc << endl;
	cout << resLoc << endl;

	glUniform3f(resLoc, WIDTH, HEIGHT, WIDTH * HEIGHT);

	fftAudioStream.play();
	int counter = 6;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		std::vector<complex> data = fftAudioStream.getCurrentSampleWaveVector();
		equalizerVisualization(data);

		GLfloat time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;
		if (time > counter) {

			for (int i = 0; i < EQUALIZER_COLUMNS; i++) {
				cout << barData[i] << " ";
			}
			cout << endl;
			counter += 2;
		}

		////waveDataVisualization(data);
		//glUniform1fv(waveLoc, WAVE_DATA_SIZE, waveData);
		//GLfloat time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;
		glUniform1f(timeLoc, time);

		//if (time > i) {
		//	i++;
		//	for (unsigned int i = 0; i < 4; i++)
		//		std::cout << data[i].real << " + " << data[i].imag << "i" << endl;
		//}

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4* EQUALIZER_COLUMNS, &barData[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);
		glLineWidth(5.0);
		glDrawArrays(GL_LINES, 0, EQUALIZER_COLUMNS*4);

		//equalizerVisualization(fftAudioStream.getCurrentSampleSpectrumVector());
		//glUniform1fv(waveLoc, WAVE_DATA_SIZE, spectrumData);
		//glUniform1fv(sampleLoc, WAVE_DATA_SIZE, spectrumData);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	fftAudioStream.stop();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//int main(void)
//{
//	wav_hdr wavHeader;
//	int headerSize = sizeof(wav_hdr), filelength = 0;
//
//	const char* filePath = "src/GodKnows.wav";
//
//	FILE* wavFile = fopen(filePath, "r");
//	if (wavFile == nullptr)
//	{
//		fprintf(stderr, "Unable to open wave file: %s\n", filePath);
//		return 1;
//	}
//
//	//Read the header
//	size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
//	cout << "Header Read " << bytesRead << " bytes." << endl;
//	if (bytesRead > 0)
//	{
//		//Read the data
//		uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
//		uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
//		static const uint16_t BUFFER_SIZE = 4096;
//		int8_t* buffer = new int8_t[BUFFER_SIZE];
//		while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
//		{
//			/** DO SOMETHING WITH THE WAVE DATA HERE **/
//			cout << "Read " << bytesRead << " bytes." << endl;
//			for (int i = 0; i < BUFFER_SIZE; i++) {
//				int16_t value = static_cast<int16_t>(buffer[i]);
//				if (value > 0) {
//					std::cout << value << " " ;
//				}
//			}
//			cout << endl;
//		}
//		delete[] buffer;
//		buffer = nullptr;
//		filelength = getFileSize(wavFile);
//
//		cout << "File is                    :" << filelength << " bytes." << endl;
//		cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
//		cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
//		cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
//		cout << "Data size                  :" << wavHeader.ChunkSize << endl;
//
//		// Display the sampling Rate from the header
//		cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
//		cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
//		cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
//		cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
//		cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
//		cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
//		// Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
//
//		cout << "Block align                :" << wavHeader.blockAlign << endl;
//		cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
//	}
//	fclose(wavFile);
//	system("pause");
//	return 0;
//}


//int main(void)
//{
//	Application::init_window_components();
//	Application::init_renderer_components();
//	Application::init_imgui_content();
//
//	double lastTime = glfwGetTime();
//	int nbFrames = 0;
//	glEnable(GL_DEPTH_TEST);
//    while (!glfwWindowShouldClose(Application::window))
//    {
//		Application::clear_window();
//		Application::render_layers();
//		Application::imgui_new_frame();
//		Application::update_window();
//
//		double currentTime = glfwGetTime();
//		//cout << currentTime << endl;
//		nbFrames++;
//		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
//			// printf and reset timer
//			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
//			nbFrames = 0;
//			lastTime += 1.0;
//		}
//
//	}
//
//	Application::destroy_window();
//	//_CrtDumpMemoryLeaks();
//	::exit(EXIT_SUCCESS);
//}