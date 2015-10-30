/*
 * regexp.h
 *
 *     This library implements a regexp like library wrapper
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
 * RCS: @(#) $Id: regexp.h,v 1.1 2015/10/24 16:43:00 monteiro Exp $
 * 
 */

#define OKP  1
#define NOP  0

#define CHR   1
#define ANY   2
#define CCL   3
#define BOL   4
#define EOL   5
#define BOT   6
#define EOT   7
#define BOW   8
#define EOW   9
#define REF  10
#define CLO  11

#define END  0

#define MAXCHR  128
#define CHRBIT  8
#define BITBLK  MAXCHR/CHRBIT
#define BLKIND  0170
#define BITIND  07

#define ASCIIB  0177

#ifdef NO_UCHAR
typedef char CHAR;
#else
typedef unsigned char CHAR;
#endif

#define MAXNFA  1024
#define MAXTAG  10

typedef struct {
    int tagstk[MAXTAG];  /* Sub-pattern tag stack. */
    CHAR nfa[MAXNFA];    /* Automaton. */
    int sta;             /* Status of last pattern. */
    CHAR bittab[BITBLK]; /* Bit table for CCL. */
    char *bol;
    char *bopat[MAXTAG];
    char *eopat[MAXTAG];
} Regexp_Regex;

#define REGEXP_VERSION "1.1"

Gua_String Regexp_Compile(Regexp_Regex *regex, Gua_String pattern);
Gua_Integer Regexp_Execute(Regexp_Regex *regex, Gua_String lp);
Gua_String Regexp_PatternMatch(Regexp_Regex *regex, Gua_String lp, Gua_String ap);
Gua_Integer Regexp_Replace(Regexp_Regex *regex, Gua_String src, Gua_String dst);
void Regex_Fail(Gua_String err, Gua_Char c);
Gua_Status Regexp_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error);
Gua_Status Regexp_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error);

