/*
 * matrix.h
 *
 *     This library implements the matrix functions for an expression interpreter.
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
 * RCS: @(#) $Id: matrix.h,v 2.3 2017/01/07 17:49:00 monteiro Exp $
 * 
 */

#define MATRIX_VERSION "2.3"

Gua_Real Matrix_GaussMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Real Matrix_JordanMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Matrix_DetMatrix(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_TransMatrix(Gua_Object *a, Gua_Object *b, Gua_String error);
Gua_Status Matrix_Cross(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Matrix_Dot(Gua_Object *a, Gua_Object *b, Gua_Object *c, Gua_String error);
Gua_Status Matrix_Sum(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Sum2(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_SumCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Avg(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_AvgCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Count(Gua_Object *a, Gua_Object *object, Gua_String error);
Gua_Status Matrix_CountCells(Gua_Object *a, Gua_Object x1, Gua_Object y1, Gua_Object x2, Gua_Object y2, Gua_Object *object, Gua_String error);
Gua_Status Matrix_DelRow(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error);
Gua_Status Matrix_DelCol(Gua_Object *source, Gua_Object n, Gua_Object *object, Gua_String error);
Gua_Status Matrix_MatrixFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Matrix_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);
