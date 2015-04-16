/*
 * tui.h
 *
 *     This library implements a Terminal User Interface for an expression interpreter.
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
 * RCS: @(#) $Id: tui.h,v 1.4 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#define TUI_VERSION "1.4"

/* Color definitions. */
#define TUI_BLACK    0
#define TUI_RED      1
#define TUI_GREEN    2
#define TUI_YELLOW   3
#define TUI_BLUE     4
#define TUI_MAGENTA  5
#define TUI_CYAN     6
#define TUI_WHITE    7

void Tui_Clreol(void);
void Tui_Clrscr(void);
int Tui_Getch(void);
int Tui_Getche(void);
int Tui_Getkey(void);
void Tui_Gotoxy(int x, int y);
void Tui_Textcolor(int color);
void Tui_Textbackground(int color);
int Tui_Entry(int x, int y, char *buffer, int size, int forecolor, int backcolor);
void Tui_Box(int x1, int y1, int x2, int y2, int bordercolor, int backcolor);
Gua_Status Tui_TuiFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Tui_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

