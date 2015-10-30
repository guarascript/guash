/*
 * glo.h
 *
 *     This library implements an OpenGL 3D object model library wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright (C) 2013, 2015 Roberto Luiz Souza Monteiro,
 *                          Hernane Borges de Barros Pereira,
 *                          Marcelo A. Moret.
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
 * RCS: @(#) $Id: glo.h,v 1.2 2015/10/04 20:37:00 monteiro Exp $
 * 
 */

#define GUA_GLO_VERSION "1.2"

Gua_String Glo_LoadPPM(Gua_String file, Gua_Short *width, Gua_Short *height);
Gua_Status Glo_SavePPM(Gua_String file, Gua_String image, Gua_Short width, Gua_Short height);
Gua_Status Glo_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Glo_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

