/*
 * array.h
 *
 *     This library implements the array functions for an expression interpreter.
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
 * RCS: @(#) $Id: array.h,v 1.7 2015/10/08 21:51:00 monteiro Exp $
 * 
 */

#define ARRAY_ASCENDING_ORDER   0
#define ARRAY_DESCENDING_ORDER  1

#define ARRAY_VERSION "1.7"

Gua_Status Array_Intersection(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Array_Sort(Gua_Object *target, Gua_Object *source, Gua_Integer order, Gua_String error);
Gua_Status Array_ArrayFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Array_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

