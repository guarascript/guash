/*
 * glutil.h
 *
 *     This library implements an OpenGL Utility library wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright (C) 2013 Roberto Luiz Souza Monteiro,
 *                    Hernane Borges de Barros Pereira,
 *                    Marcelo A. Moret.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: glutil.h,v 1.1 2014/08/06 11:08:00 monteiro Exp $
 * 
 */

#define GUA_GLUTIL_VERSION "1.1"

Gua_String Glutil_LoadPPM(Gua_String file, Gua_Short *width, Gua_Short *height);
Gua_Status Glutil_SavePPM(Gua_String file, Gua_String image, Gua_Short width, Gua_Short height);
Gua_Status Glutil_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Glutil_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

