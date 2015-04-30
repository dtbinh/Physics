/*
 * read_texture() - read in an image file in SGI 'libimage' format
 * 	currently its very simple minded and converts all images
 *      to RGBA8 regardless of the input format and returns the
 *	original number of components in the appropriate parameter.
 *    
 *     
 *	the components are converted as follows
 *		L    -> LLL 1.0
 *		LA   -> LLL A
 *		RGB  -> RGB 1.0
 *		RGBA -> RGB A
 *
 */
unsigned *
read_texture(const char *name, int *width, int *height, int *components);

 /** 
  * Read a PPM raw (type P6) file.  The PPM file has a header
  * that should look something like:
  *
  *    P6
  *    # comment
  *    width height max_value
  *    rgbrgbrgb...
  *
  * where "P6" is the magic cookie which identifies the file type and
  * should be the only characters on the first line followed by a
  * carriage return.  Any line starting with a # mark will be treated
  * as a comment and discarded.   After the magic cookie, three integer
  * values are expected: width, height of the image and the maximum
  * value for a pixel (max_value must be < 256 for PPM raw files).  The
  * data section consists of width*height rgb triplets (one byte each)
  * in binary format (i.e., such as that written with fwrite() or
  * equivalent).
  *
  * The rgb data is returned as an array of unsigned chars (packed
  * rgb).  The malloc()'d memory should be free()'d by the caller.  If
  * an error occurs, an error message is sent to stderr and NULL is
  * returned.
  *
  * filename   - name of the .ppm file.
  * width      - will contain the width of the image on return.
  * height     - will contain the height of the image on return.
  * Function copied from texture tutorial by Nate Robins
 */
GLubyte* readPPM(char* filename, int* width, int* height);
