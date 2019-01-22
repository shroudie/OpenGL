#pragma once
#include <vector>

struct complex
{
	float real, imag;

	complex() : real(0.f), imag(0.f) {}
	complex(float r, float i) : real(r), imag(i) {}
	complex(float v) : real(v), imag(0.f) {}

	complex &operator=(const float val) {
		real = val;
		imag = 0.;
		return *this;
	}

	complex operator*(const complex &other) const
	{
		return complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
	}

	complex operator-(const complex &other)
	{
		return complex(real - other.real, imag - other.imag);
	}

	complex operator+(const complex &other)
	{
		return complex(real + other.real, imag + other.imag);
	}

	complex& operator+=(const complex &other)
	{
		real += other.real;
		imag += other.imag;
		return *this;
	}

	complex& operator*=(const complex &other)
	{
		float tmp = real;
		real = real * other.real - imag * other.imag;
		imag = tmp * other.imag + imag * other.real;
		return *this;
	}
};

class fft
{
public:
	static std::vector<complex> forward(std::vector<complex> &data)
	{
		if (data.size() == 1) return data;
		int M = data.size() / 2;

		std::vector<complex> even(M, 0), odd(M, 0);
		for (int i = 0; i != M; ++i) {
			even[i] = data[2 * i];
			odd[i] = data[2 * i + 1];
		}
		std::vector<complex> feven = forward(even);
		std::vector<complex> fodd = forward(odd);
		std::vector<complex> freq(data.size(), 0);
		for (unsigned int k = 0; k != data.size() / 2; ++k) {

		}
	}

	static void forward(complex *const data, unsigned int N) {
		unsigned int k = N, n;
		float thetaT = 3.14159265358979323846264338328f / N;
		complex phiT = complex(cos(thetaT), -sin(thetaT)), T;
		while (k > 1)
		{
			n = k;
			k >>= 1;
			phiT = phiT * phiT;
			T = 1.0L;
			for (unsigned int l = 0; l < k; l++)
			{
				for (unsigned int a = l; a < N; a += n)
				{
					unsigned int b = a + k;
					complex t = data[a] - data[b];
					data[a] += data[b];
					data[b] = t * T;
				}
				T *= phiT;
			}
		}
		// Decimate
		unsigned int m = (unsigned int)log2(N);
		for (unsigned int a = 0; a < N; a++)
		{
			unsigned int b = a;
			// Reverse bits
			b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
			b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
			b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
			b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
			b = ((b >> 16) | (b << 16)) >> (32 - m);
			if (b > a)
			{
				complex t = data[a];
				data[a] = data[b];
				data[b] = t;
			}
		}
	}
};