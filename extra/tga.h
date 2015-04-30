#ifndef __TGA_H__
#define __TGA_H__

#include <stdio.h>
#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/OpenGL.h>
#else
    #include <GL/glut.h>
#endif

typedef struct
{
    GLubyte	* imageData; // Image Data (Up To 32 Bits)
    GLuint	bpp;	//Image Color Depth In Bits Per Pixel
    GLuint	width;	//Image Width
    GLuint	height;	//Image Height
    GLuint	texID; // Texture ID Used To Select A Texture
    GLuint	type;	//Image Type (GL_RGB, GL_RGBA)
} Texture;

typedef struct
{
	GLubyte Header[12];	//TGA File Header 
} TGAHeader;

typedef struct
{
	GLubyte		header[6]; // First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel; //Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize; // Used To Store The Image Size When Setting Aside Ram
	GLuint		temp; //Temporary Variable
	GLuint		type;
	GLuint		Height;	// Height of Image
	GLuint		Width;	//Width of Image 
	GLuint		Bpp; //Bits Per Pixel
} TGA;


TGAHeader tgaheader; // TGA header
TGA tga; //TGA image data


GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0}; // Uncompressed TGA Header
GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0}; // Compressed TGA Header


#endif
