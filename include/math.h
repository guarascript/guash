/*
 * math.h
 *
 *     This library implements the math functions for an expression interpreter.
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
 * RCS: @(#) $Id: math.h,v 2.2 2013/11/14 15:35:00 monteiro Exp $
 * 
 */

/* It's not defined in <math.h>. */
double round(double x);

#define MATH_VERSION "2.2"

#define MATH_E  2.7182818284590452354
#define MATH_PI 3.14159265358979323846

Gua_Status Math_MathFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Math_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
