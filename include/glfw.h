/*
 * glfw.h
 *
 *     This library implements an OpenGL GLFW wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright (C) 2015 Roberto Luiz Souza Monteiro.
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
 * RCS: @(#) $Id: glfw.h,v 1.0 2015/03/03 11:33:00 monteiro Exp $
 * 
 */
#include <GLFW/glfw3.h>

#define GUA_GLFW_VERSION "1.0"

void Glfw_GLFWcharfun(GLFWwindow *window, unsigned int codepoint);
void Glfw_GLFWcharmodsfun(GLFWwindow *window, unsigned int codepoint, int mods);
void Glfw_GLFWcursorenterfun(GLFWwindow *window, int entered);
void Glfw_GLFWcursorposfun(GLFWwindow *window, double xpos, double ypos);
void Glfw_GLFWdropfun(GLFWwindow *window, int count, const char **names);
void Glfw_GLFWerrorfun(int error, const char *description);
void Glfw_GLFWkeyfun(GLFWwindow *window, int key, int scancode, int action, int mods);
Gua_Status Glfw_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Glfw_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

