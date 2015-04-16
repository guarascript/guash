/*
 * system.h
 *
 *     This library implements the system functions for an expression interpreter.
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
 * RCS: @(#) $Id: system.h,v 2.2 2014/08/05 09:20:00 monteiro Exp $
 * 
 */

#define SYSTEM_VERSION "2.2"

#ifdef _LINUX_
    #define SYSTEM_HOST "linux"
    #define SYSTEM_ARCH "x86"
#elif _MAC_OS_X_
    #define SYSTEM_HOST "macosx"
    #define SYSTEM_ARCH "x86"
#elif _WINDOWS_
    #define SYSTEM_HOST "windows"
    #define SYSTEM_ARCH "x86"
#else
    #define SYSTEM_HOST "unix"
    #define SYSTEM_ARCH "x86"
#endif

Gua_Status System_TimeFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_StrftimeFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

