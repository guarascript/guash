/*
 * numeric.h
 *
 *     This library implements the numeric computation functions for an expression interpreter.
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
 * RCS: @(#) $Id: numeric.h,v 1.4 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#define NUMERIC_VERSION "1.4"
#define NUMERIC_GOLDEN_NUMBER 1.6180
#define NUMERIC_X "x"
#define NUMERIC_XYZ "{\"x\", \"y\", \"z\"}"

Gua_Status Numeric_GaussLSS(Gua_Object *a, Gua_Object *b, Gua_Object *x, Gua_String error);
Gua_Status Numeric_NumericFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Numeric_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
