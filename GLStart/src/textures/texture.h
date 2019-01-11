#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <png.h>

#include <iostream>
#include <cstdlib>

class Texture {
public:
	Texture() {}

	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	GLubyte *textureImage;

	GLuint loadTexture(const char*, int&, int&);
	png_byte *image_data;
	bool load_png_into_texture(const char *name) {
		FILE *fp = fopen(name, "rb");
		if (!fp) {
			printf("Unable to open file\n");
			return false;
		}
		//unsigned char sig[8];

		//fread(sig, 1, 8, fp);
		//if (!png_sig_cmp(sig, 0, 8)) {
		//	printf("Not a png file\n");
		//	return false;
		//}

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) return false;

		png_infop info_ptr = png_create_info_struct(png);
		if (!info_ptr) {
			png_destroy_read_struct(&png, NULL, NULL);
			return false;
		}

		if (setjmp(png_jmpbuf(png))) {
			png_destroy_read_struct(&png, &info_ptr, NULL);
			fclose(fp);
			/* If we get here, we had a
			 * problem reading the file */
			return false;
		}

		png_init_io(png, fp);
		png_set_sig_bytes(png, 0);
		png_read_info(png, info_ptr);

		width = png_get_image_width(png, info_ptr);
		height = png_get_image_height(png, info_ptr);
		png_byte color_type = png_get_color_type(png, info_ptr);
		png_byte bit_depth = png_get_bit_depth(png, info_ptr);

		if (bit_depth == 16)
			png_set_strip_16(png);

		if (color_type == PNG_COLOR_TYPE_PALETTE) {
			std::cout << "PALLTER color" << std::endl;
			png_set_palette_to_rgb(png);
		}
		if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
			std::cout << "GATRG color" << std::endl;
			png_set_expand_gray_1_2_4_to_8(png);
		}
		
		if (png_get_valid(png, info_ptr, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		if (color_type == PNG_COLOR_TYPE_RGBA) {
			std::cout << "rgba color" << std::endl;
		}

		if (color_type == PNG_COLOR_TYPE_GRAY) {
			std::cout << "GRAG color" << std::endl;
			png_set_gray_to_rgb(png);
		}
		png_read_update_info(png, info_ptr);

		int rowbytes = png_get_rowbytes(png, info_ptr);
		png_byte *image_data = new png_byte[rowbytes * height];

		png_bytep *row_pointers = new png_bytep[height];
		for (int i = 0; i < height; ++i)
			row_pointers[height - 1 - i] = image_data + i * rowbytes;

		//row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
		//for (int y = 0; y < height; y++) {
		//	row_pointers[y] = (png_byte*)malloc(rowbytes);
		//}

		png_read_image(png, row_pointers);

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_RGBA);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image_data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		png_destroy_read_struct(&png, &info_ptr, NULL);
		delete[] image_data;
		delete[] row_pointers;

		fclose(fp);


		//png_uint_32 width, height;
		//int bit_depth;
		//png_get_IHDR(png, info_ptr, &width, &height, &bit_depth, &color_type,
		//	&interlace_type, NULL, NULL);
		//outWidth = width;
		//outHeight = height;

		//unsigned int row_bytes = png_get_rowbytes(png, info_ptr);
		//*outData = (unsigned char*)malloc(row_bytes * outHeight);

		//png_bytepp row_pointers = png_get_rows(png, info_ptr);

		//for (int i = 0; i < outHeight; i++) {
		//	// note that png is ordered top to
		//	// bottom, but OpenGL expect it bottom to top
		//	// so the order or swapped
		//	memcpy(*outData + (row_bytes * (outHeight - 1 - i)), row_pointers[i], row_bytes);
		//}

		///* Clean up after the read,
		// * and free any memory allocated */
		//png_destroy_read_struct(&png, &info_ptr, NULL);

		///* Close the file */
		//fclose(fp);

		///* That's it */
		return true;
	}

	unsigned int load_texture(const char *fp);
};