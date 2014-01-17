/*
 * screenshot.c
 *
 * Routines for taking screen shots from GL
 *
 * Copyright (C) 2001 Ryan T. Sammartino
 * http://members.shaw.ca/ryants/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * revised 2003/04/03 by Nick Van Der Horst
 */
//#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <malloc.h>
#include "screenshot.h"

#ifndef GL_BGR

/*#define GL_BGR     8*/

#define GL_BGR     GL_RGB

#endif



#include <iostream>
//#define GL_BGR     8
#define BMP_HEADER_SIZE	(14)
#define BMP_INFO_SIZE	(40)

static int SCREENSHOT_WriteBMP(const char *fname,
				const unsigned char *image,
				int w, int h)

{
	/* See http://www.dcs.ed.ac.uk/home/mxr/gfx/2d/BMP.txt  */
	struct BMPHeader {
		unsigned short type;
		unsigned int size;
		unsigned short res1;
		unsigned short res2;
		unsigned int offset;
	}  /*_attribute__((packed))*/ header;
	struct BMPInfo {
		unsigned int size;
		unsigned int width;
		unsigned int height;
		unsigned short planes;
		unsigned short bit_count;
		unsigned int comp;
		unsigned int sizeImage;
		unsigned int x_pels_per_meter;
		unsigned int y_pels_per_meter;
		unsigned int clr_used;
		unsigned int clr_important;
	} /*_attribute__((packed))*/ info;
	FILE *fp;
	int ret = 0;
	/* Open file */
	fp = fopen(fname, "wb");
	if (!fp) {
		fprintf(stderr,
			"Unable to open %s for writing\n",
			fname);
		ret = -1;
	} else {
		/* Header */
		header.type = 'B' | 'M' << 8;
		header.size = BMP_HEADER_SIZE + BMP_INFO_SIZE + w * h * 3;

		std::cout << header.size << " = " << sizeof(header) << " + " << sizeof(info) << " + " << w * h * 3 << std::endl;

		header.res1 = header.res2 = 0;
		header.offset = BMP_HEADER_SIZE + BMP_INFO_SIZE;

		info.size = BMP_INFO_SIZE;
		info.width = w;
		info.height = h;
		info.planes = 1;
		info.bit_count = 24;
		info.comp = 0;
		info.sizeImage = w * h * 3;
		info.x_pels_per_meter = info.y_pels_per_meter = 0;
		info.clr_used = 0;
		info.clr_important = 0;

		//fwrite(&header, sizeof(header), 1, fp);
		fwrite(&header.type, sizeof(unsigned short), 1, fp);
		fwrite(&header.size, sizeof(unsigned int), 1, fp);
		fwrite(&header.res1, sizeof(unsigned short), 1, fp);
		fwrite(&header.res2, sizeof(unsigned short), 1, fp);
		fwrite(&header.offset, sizeof(unsigned int), 1, fp);

		//fwrite(&info, sizeof(info), 1, fp);
		fwrite(&info.size, sizeof(unsigned int), 1, fp);
		fwrite(&info.width, sizeof(unsigned int), 1, fp);
		fwrite(&info.height, sizeof(unsigned int), 1, fp);
		fwrite(&info.planes, sizeof(unsigned short), 1, fp);
		fwrite(&info.bit_count, sizeof(unsigned short), 1, fp);
		fwrite(&info.comp, sizeof(unsigned int), 1, fp);
		fwrite(&info.sizeImage, sizeof(unsigned int), 1, fp);
		fwrite(&info.x_pels_per_meter, sizeof(unsigned int), 1, fp);
		fwrite(&info.y_pels_per_meter, sizeof(unsigned int), 1, fp);
		fwrite(&info.clr_used, sizeof(unsigned int), 1, fp);
		fwrite(&info.clr_important, sizeof(unsigned int), 1, fp);

		fwrite(image, sizeof(unsigned char), h*w*3, fp);
	}
	if (fp) {
		fclose(fp);
	}
	return ret;
}
static int SCREENSHOT_WriteTGA(const char *fname,
				const unsigned char *image,
				int w, int h)
{
	int i;
	FILE *fp;
	int ret = -1;
	/* See http://www.dcs.ed.ac.uk/home/mxr/gfx/2d/TGA.txt  for spec */
	struct TGAHeader {
		unsigned char idfield_len;
		unsigned char cmap_type;
		unsigned char image_type;
		unsigned char cmap_spec[5];
		unsigned char x_orig[2];
		unsigned char y_orig[2];
		unsigned char width[2];
		unsigned char height[2];
		unsigned char pixel_size;
		unsigned char image_desc;
	} /*_attribute__((packed))*/ header;
	/* Open file */
	fp = fopen(fname, "wb");
	if (!fp) {
		fprintf(stderr,
			"Unable to open %s for writing\n",
			fname);
		ret = -1;
	} else {
		/* Construct header */
		header.idfield_len = 0;
		header.cmap_type = 0;
		header.image_type = 2;
		for (i = 0; i < 5; i++) {
			header.cmap_spec[i] = 0;
		}
		for (i = 0; i < 2; i++) {
			header.x_orig[i] = 0;
			header.y_orig[i] = 0;
		}
		/* Lo bits */
		header.width[0] = w & 0xFF;
		/* Hi bits */
		header.width[1] = (w >> 8) & 0xFF;
		header.height[0] = h & 0xFF;
		header.height[1] = (h >> 8) & 0xFF;
		header.pixel_size = 24;
		header.image_desc = 0;
		/* Output header */
		fwrite(&header, sizeof(header), 1, fp);

		/* Output image */
		fwrite(image, sizeof(unsigned char), w*h*3, fp);
	}
	if (fp) {
		fclose(fp);
	}
	return ret;
}
static int SCREENSHOT_WritePPM(const char *fname,
				const unsigned char *image,
				int w, int h)
{
	unsigned char *row;
	int i;
	FILE *fp;
	int ret = 0;
	/* Open file */
	fp = fopen(fname, "wb");
	if (!fp) {
		fprintf(stderr,
			"Unable to open %s for writing\n",
			fname);
			ret = -1;
	} else {
		/* Output header */
		fprintf(fp,
			"P6\n#Screenshot %s\n%d %d\n%d\n",
			fname, w, h, 255);

		/* GL returns the data upside down */
		for (i = h - 1; i >= 0; i--) {
			row = (unsigned char*)image + w * i * 3;
			fwrite(row, sizeof(unsigned char), w*3, fp);
		}
	}
	if (fp) {
		fclose(fp);
	}
	return ret;
}
/*
 * SCREENSHOT_Take will dump the given portion of the GL framebuffer to
 * the given filename
 *
 *
 * x, y, width, height define the rectangle to capture.
 *
 * The image will be stored in the given format
 *
 * Returns negative on failure, zero on success
 */
int SCREENSHOT_Take(int x, int y, int width, int height,
			const char *fname,
			SCREENSHOT_FORMAT format)
{
	unsigned char *image = NULL;
	int ret = 0;
	/* Reserve some memory */
	image = (unsigned char*)malloc(sizeof(unsigned char)*width*height*3*2);
	if (!image) {
		fprintf(stderr,
			"Unable to allocate image buffer\n");
		ret = -1;
	} else {
		switch(format) {
		case SCREENSHOT_PPM:
          //std::cout << x << ", " << y << ", " << width << ", " << height << "\n";
			glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
			ret = SCREENSHOT_WritePPM(fname, image, width, height);
			break;
		case SCREENSHOT_TGA:
//vbz			glReadPixels(x, y, width, height, GL_BGR, GL_UNSIGNED_BYTE, image);
			glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
			ret = SCREENSHOT_WriteTGA(fname, image, width, height);
			break;
		case SCREENSHOT_BMP:
			glReadPixels(x, y, width, height, GL_BGR, GL_UNSIGNED_BYTE, image);
//vbz			glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
			ret = SCREENSHOT_WriteBMP(fname, image, width, height);
			break;
		default:
			fprintf(stderr, "Invalid format %d\n", format);
			ret = -1;
			break;
		}
	}
	if (image) {
		free(image);
	}
	return ret;
}
