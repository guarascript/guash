/**
 * File:
 *     glft.c
 *
 * Package:
 *     Glft
 *
 * Description:
 *     This file implements a BDF, Glyph Bitmap Distribution Format,
 *     OpenGL render library.
 *
 * Copyright:
 *     Copyright (C) 2015 Roberto Luiz Souza Monteiro.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: glft.c,v 1.0 2015/10/04 00:11:00 monteiro Exp $
 * 
 */
 
#include <stddef.h>
#include <GL/gl.h>
#include "glft.h"

/**
 * Group:
 *     C
 *
 * Function:
 *     int Glft_DrawText(float x, float y, char *file, char *text)
 *
 * Description:
 *     Draw a text string in an OpenGL surface.
 *
 * Arguments:
 *     x,       top left x coordinate;
 *     y,       top left y coordinate;
 *     file,    font file;
 *     text,    text string.
 *
 * Results:
 *     The function draw a text string in an OpenGL surface.
 */
int Glft_DrawText(float x, float y, char *file, char *text)
{
    Bdf_Font font;
    Bdf_Bitmap bitmap;
    Bdf_Bitmap flipped;
    
    Bdf_ClearFont(font);
    
    /* Load font. */
    if (!Bdf_LoadFont(&font, file)) {
        return 0;
    }
    
    /* Draw text on a bitmap surface. */
    if (!Bdf_DrawText(&font, (char *)text, &bitmap)) {
        Bdf_FreeFont(&font);
        
        return 0;
    }
    
    /* Flip the bitmap. */
    Bdf_FlipBitmap(&flipped, &bitmap);
    
    /* Draw bitmap on display. */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glRasterPos2f(x, y);
    
    glBitmap(Bdf_BitmapWidth(flipped), Bdf_BitmapHeight(flipped), 0.0, 0.0, (float)(Bdf_BitmapWidth(flipped)), 0.0, (const GLubyte *)(Bdf_BitmapImage(flipped)));
    
    /* Free resources. */
    Bdf_FreeBitmap(&bitmap);
    Bdf_FreeBitmap(&flipped);
    Bdf_FreeFont(&font);
    
    return 1;
}

