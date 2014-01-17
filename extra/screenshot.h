/*
 * screenshot.h
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
 */

#ifndef _SCREENSHOT_H_
#define _SCREENSHOT_H_

typedef enum {
    SCREENSHOT_PPM,
	SCREENSHOT_TGA,
	SCREENSHOT_BMP,
} SCREENSHOT_FORMAT;

/*
 * SCREENSHOT_Take will dump the given portion of the GL framebuffer to
 * the given filename.  The file will be overwritten
 *
 *
 * x, y, width, height definre the rectangle to capture.
 *
 * 0, 0 is the upper-left corner
 *
 * The image will be stored in the given format
 *
 * Returns negative on failure, zero on success
 */

int SCREENSHOT_Take(int x, int y, int width, int height,
                     const char *fname,
					 SCREENSHOT_FORMAT format);

#endif // !_SCREENSHOT_H_
