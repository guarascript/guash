/*
 * match.h
 *
 *     This library implements a glob like library wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright (C) 2015 Roberto Luiz Souza Monteiro,
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
 * RCS: @(#) $Id: match.h,v 1.0 2015/10/24 14:58:00 monteiro Exp $
 * 
 */

#define MATCH_VERSION "1.0"

Gua_Integer Match_Match(Gua_String string, Gua_String pattern);
Gua_Status Match_Glob(Gua_String directory, Gua_String pattern, Gua_Object *list, Gua_Integer max, Gua_String error);
Gua_Status Match_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Match_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

