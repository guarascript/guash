/*
 * glut.h
 *
 *     This library implements an OpenGL GLUT wrapper
 *     for the GuaraScript interpreter.
 *
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
 * RCS: @(#) $Id: glut.h,v 1.0 2013/09/09 18:47:00 monteiro Exp $
 * 
 */

#define GUA_GLUT_VERSION "1.0"

void Glut_ButtonBoxFunc(int button, int state);
void Glut_CreateMenu(int value);
void Glut_DialsFunc(int dial, int value);
void Glut_DisplayFunc(void);
void Glut_EntryFunc(int state);
void Glut_IdleFunc(void);
void Glut_KeyboardFunc(unsigned char key, int x, int y);
void Glut_MenuStateFunc(int s);
void Glut_MenuStatusFunc(int s, int x, int y);
void Glut_MotionFunc(int x, int y);
void Glut_MouseFunc(int button, int state, int x, int y);
void Glut_OverlayDisplayFunc(void);
void Glut_PassiveMotionFunc(int x, int y);
void Glut_ReshapeFunc(int width, int height);
void Glut_SpaceballButtonFunc(int button, int state);
void Glut_SpaceballMotionFunc(int x, int y, int z);
void Glut_SpaceballRotateFunc(int x, int y, int z);
void Glut_SpecialFunc(int key, int x, int y);
void Glut_TabletButtonFunc(int button, int state, int x, int y);
void Glut_TabletMotionFunc(int x, int y);
void Glut_TimerFunc(int value);
void Glut_VisibilityFunc(int state);
Gua_Status Glut_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Glut_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

