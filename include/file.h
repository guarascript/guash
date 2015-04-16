/*
 * file.h
 *
 *     This library implements a virtual file system for an expression interpreter.
 *
 * Copyright (C) 2005 Roberto Luiz Souza Monteiro.
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
 * RCS: @(#) $Id: file.c,v 1.3 2013/09/24 19:28:00 monteiro Exp $
 * 
 */

#define FILE_VERSION "1.3"

void File_Clearerr(void *h);
Gua_Short File_Close(void *h);
Gua_Short File_Eof(void *h);
Gua_Short File_Error(void *h);
Gua_Short File_Fileno(void *h);
Gua_Short File_Flush(void *h);
Gua_String File_Gets(Gua_String s, Gua_Short l, void *h);
Gua_File *File_Open(Gua_String p, Gua_String m);
Gua_Short File_Puts(Gua_String s, void *h);
Gua_Integer File_Read(Gua_String s, Gua_Integer l, Gua_Integer n, void *h);
void File_Rewind(void *h);
Gua_Short File_Seek(void *h, Gua_Integer o, Gua_Short w);
Gua_Integer File_Tell(void *h);
Gua_Integer File_Write(Gua_String s, Gua_Integer l, Gua_Integer n, void *h);
Gua_Status File_FileFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status File_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

