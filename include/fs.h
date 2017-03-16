/*
 * fs.h
 *
 *     This library implements the file system access functions.
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
 * RCS: @(#) $Id: fs.h,v 2.0 2015/09/17 01:34:00 monteiro Exp $
 * 
 */

#define FS_VERSION "2.0"

Gua_Status Fs_Copy(Gua_String source, Gua_String target, Gua_String error);
Gua_Status Fs_Delete(Gua_String file, Gua_String error);
Gua_Integer Fs_Exists(Gua_String file);
void Fs_Extension(Gua_String file, Gua_String *extension);
void Fs_FullPath(Gua_String file, Gua_String *path);
Gua_Integer Fs_IsDirectory(Gua_String file);
Gua_Integer Fs_IsExecutable(Gua_String file);
Gua_Integer Fs_IsReadable(Gua_String file);
Gua_Integer Fs_IsWritable(Gua_String file);
Gua_Integer Fs_Link(Gua_String source, Gua_String target);
Gua_Status Fs_List(Gua_String directory, Gua_Object *list, Gua_String error);
Gua_Integer Fs_MkDir(Gua_String directory);
void Fs_Name(Gua_String file, Gua_String *name);
void Fs_Path(Gua_String file, Gua_String *path);
Gua_Status Fs_Rename(Gua_String source, Gua_String target, Gua_String error);
Gua_Integer Fs_Size(Gua_String file);
Gua_Real Fs_Time(Gua_String file);
Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

