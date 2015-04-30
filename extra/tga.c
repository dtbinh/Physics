#include "tga.h"
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

unsigned * loadTGA(const char* filename, int *width, int *height, int *components)
{
    Texture texture;
	FILE * fTGA;
	fTGA = fopen(filename, "rb");

	if(fTGA == NULL)
	{
		printf("Error could not open texture file");
		return NULL;	
	}

	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		printf("Error could not read file header");	
		if(fTGA != NULL)	
		{
			fclose(fTGA);	
		}
		return NULL;		
	}
    // See if header matches the predefined header of 
    // an Uncompressed TGA image
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)	
	{																
        GLuint cswap;
        if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
        {
            printf("Error could not read info header");
            if(fTGA != NULL)
            {
                fclose(fTGA);
            }
            return NULL;
        }

        texture.width  = tga.header[1] * 256 + tga.header[0];
        texture.height = tga.header[3] * 256 + tga.header[2];
        texture.bpp	= tga.header[4];
        tga.Width		= texture.width;
        tga.Height		= texture.height;
        tga.Bpp			= texture.bpp;

        if((texture.width <= 0) || (texture.height <= 0) || ((texture.bpp != 24) && (texture.bpp !=32)))
        {
            printf("Error invalid texture information");
            if(fTGA != NULL)
            {
                fclose(fTGA);
            }
            return NULL;
        }

        if(texture.bpp == 24)
        {
            texture.type	= GL_RGB;
        }
        else
        {
            texture.type	= GL_RGBA;
        }

        tga.bytesPerPixel	= (tga.Bpp / 8);
        tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
        texture.imageData	= malloc(tga.imageSize);

        if(texture.imageData == NULL)
        {
            printf("Error could not allocate memory for image");
            fclose(fTGA);
            return NULL;
        }

        if(fread(texture.imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
        {
            printf("Error could not read image data");
            if(texture.imageData != NULL)
            {
                free(texture.imageData);
            }
            fclose(fTGA);
            return NULL;
        }

        /* Byte Swapping Optimized By Steve Thomas */
        for(cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
        {
            texture.imageData[cswap] ^= texture.imageData[cswap+2] ^=
            texture.imageData[cswap] ^= texture.imageData[cswap+2];
        }

        fclose(fTGA);
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)	
	{																	
        GLuint pixelcount	= tga.Height * tga.Width;
        GLuint currentpixel	= 0;
        GLuint currentbyte	= 0;
        GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);

        if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
        {
            printf("Error could not read info header");
            if(fTGA != NULL)
            {
                fclose(fTGA);
            }
            return NULL;
        }

        texture.width  = tga.header[1] * 256 + tga.header[0];
        texture.height = tga.header[3] * 256 + tga.header[2];
        texture.bpp	= tga.header[4];
        tga.Width		= texture.width;
        tga.Height		= texture.height;
        tga.Bpp			= texture.bpp;

        if((texture.width <= 0) || (texture.height <= 0) || ((texture.bpp != 24) && (texture.bpp !=32)))
        {
            printf("Error Invalid texture information");
            if(fTGA != NULL)
            {
                fclose(fTGA);
            }
            return NULL;
        }

        tga.bytesPerPixel	= (tga.Bpp / 8);
        tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);
        texture.imageData	= (GLubyte *)malloc(tga.imageSize);

        if(texture.imageData == NULL)
        {
            printf("Error could not allocate memory for image");
            fclose(fTGA);
            return NULL;
        }

        do
        {
            GLubyte chunkheader = 0;

            if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)
            {
                printf("Error could not read RLE header");
                if(fTGA != NULL)
                {
                    fclose(fTGA);
                }
                if(texture.imageData != NULL)
                {
                    free(texture.imageData);
                }
                return NULL;
            }

            if(chunkheader < 128)
            {
                short counter;
                chunkheader++;
                for(counter = 0; counter < chunkheader; counter++)
                {
                    if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
                    {
                        printf("Error could not read image data");

                        if(fTGA != NULL)
                        {
                            fclose(fTGA);
                        }

                        if(colorbuffer != NULL)
                        {
                            free(colorbuffer);
                        }

                        if(texture.imageData != NULL)
                        {
                            free(texture.imageData);
                        }

                        return NULL;
                    }

                    texture.imageData[currentbyte		] = colorbuffer[2];
                    texture.imageData[currentbyte + 1	] = colorbuffer[1];
                    texture.imageData[currentbyte + 2	] = colorbuffer[0];

                    if(tga.bytesPerPixel == 4)
                    {
                        texture.imageData[currentbyte + 3] = colorbuffer[3];
                    }

                    currentbyte += tga.bytesPerPixel;
                    currentpixel++;

                    if(currentpixel > pixelcount)
                    {
                        printf("Error too many pixels read");

                        if(fTGA != NULL)
                        {
                            fclose(fTGA);
                        }

                        if(colorbuffer != NULL)
                        {
                            free(colorbuffer);
                        }

                        if(texture.imageData != NULL)
                        {
                            free(texture.imageData);
                        }

                        return NULL;
                    }
                }
            }
            else
            {
                /* chunkheader > 128 RLE data, next color  reapeated chunkheader - 127 times */
                short counter;
                chunkheader -= 127;
                if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
                {
                    printf("Error could not read from file");

                    if(fTGA != NULL)
                    {
                        fclose(fTGA);
                    }

                    if(colorbuffer != NULL)
                    {
                        free(colorbuffer);
                    }

                    if(texture.imageData != NULL)
                    {
                        free(texture.imageData);
                    }

                    return NULL;
                }

                for(counter = 0; counter < chunkheader; counter++)
                {
                    texture.imageData[currentbyte		] = colorbuffer[2];
                    texture.imageData[currentbyte + 1	] = colorbuffer[1];
                    texture.imageData[currentbyte + 2	] = colorbuffer[0];

                    if(tga.bytesPerPixel == 4)
                    {
                        texture.imageData[currentbyte + 3] = colorbuffer[3];
                    }

                    currentbyte += tga.bytesPerPixel;
                    currentpixel++;

                    if(currentpixel > pixelcount)
                    {
                        printf("Error too many pixels read");

                        if(fTGA != NULL)
                        {
                            fclose(fTGA);
                        }

                        if(colorbuffer != NULL)
                        {
                            free(colorbuffer);
                        }

                        if(texture.imageData != NULL)
                        {
                            free(texture.imageData);
                        }

                        return NULL;
                    }
                }
            }
        }

        while(currentpixel < pixelcount);
        fclose(fTGA);
	}
	else																
	{
		printf("Error TGA file be type 2 or type 10 ");				
		fclose(fTGA);
		return NULL;											
	}
    (*width) = texture.width;
    (*height) = texture.height;
    if(texture.bpp == 24)
        (*components)	= 3;
    else
        (*components)	= 4;
    return (unsigned *) texture.imageData;
}
