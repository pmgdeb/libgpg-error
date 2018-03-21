/* argparse.h - Argument parser for option handling.
 * Copyright (C) 1997-2001, 2006-2008, 2013-2017 Werner Koch
 * Copyright (C) 1998-2001, 2006-2008, 2012 Free Software Foundation, Inc.
 * Copyright (C) 2015-2017 g10 Code GmbH
 *
 * This file is part of GnuPG.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef GNUPG_COMMON_ARGPARSE_H
#define GNUPG_COMMON_ARGPARSE_H

#include <stdio.h>

typedef struct
{
  int  *argc;	      /* Pointer to ARGC (value subject to change). */
  char ***argv;	      /* Pointer to ARGV (value subject to change). */
  unsigned int flags; /* Global flags.  May be set prior to calling the
                         parser.  The parser may change the value.  */
  int err;            /* Print error description for last option.
                         Either 0,  ARGPARSE_PRINT_WARNING or
                         ARGPARSE_PRINT_ERROR.  */

  int r_opt; 	      /* Returns option code. */
  int r_type;	      /* Returns type of option value.  */
  union {
    int   ret_int;
    long  ret_long;
    unsigned long ret_ulong;
    char *ret_str;
  } r;		      /* Return values */

  struct {
    int idx;
    int inarg;
    int stopped;
    const char *last;
    void *aliases;
    const void *cur_alias;
    void *iio_list;
  } internal;	    /* Private - do not change. */
} ARGPARSE_ARGS;

typedef struct
{
  int          short_opt;
  const char  *long_opt;
  unsigned int flags;
  const char  *description; /* Optional option description. */
} ARGPARSE_OPTS;

/* Short options.  */
#define ARGPARSE_SHORTOPT_HELP 32768
#define ARGPARSE_SHORTOPT_VERSION 32769
#define ARGPARSE_SHORTOPT_WARRANTY 32770
#define ARGPARSE_SHORTOPT_DUMP_OPTIONS 32771


/* Global flags (ARGPARSE_ARGS).  */
#define ARGPARSE_FLAG_KEEP       1   /* Do not remove options form argv.     */
#define ARGPARSE_FLAG_ALL        2   /* Do not stop at last option but return
                                        remaining args with R_OPT set to -1. */
#define ARGPARSE_FLAG_MIXED      4   /* Assume options and args are mixed.   */
#define ARGPARSE_FLAG_NOSTOP     8   /* Do not stop processing at "--".      */
#define ARGPARSE_FLAG_ARG0      16   /* Do not skip the first arg.           */
#define ARGPARSE_FLAG_ONEDASH   32   /* Allow long options with one dash.    */
#define ARGPARSE_FLAG_NOVERSION 64   /* No output for "--version".           */

#define ARGPARSE_FLAG_STOP_SEEN 256  /* Set to true if a "--" has been seen. */

/* Flags for each option (ARGPARSE_OPTS).  The type code may be
   ORed with the OPT flags.  */
#define ARGPARSE_TYPE_NONE        0  /* Does not take an argument.        */
#define ARGPARSE_TYPE_INT         1  /* Takes an int argument.            */
#define ARGPARSE_TYPE_STRING      2  /* Takes a string argument.          */
#define ARGPARSE_TYPE_LONG        3  /* Takes a long argument.            */
#define ARGPARSE_TYPE_ULONG       4  /* Takes an unsigned long argument.  */
#define ARGPARSE_OPT_OPTIONAL (1<<3) /* Argument is optional.             */
#define ARGPARSE_OPT_PREFIX   (1<<4) /* Allow 0x etc. prefixed values.    */
#define ARGPARSE_OPT_IGNORE   (1<<6) /* Ignore command or option.         */
#define ARGPARSE_OPT_COMMAND  (1<<7) /* The argument is a command.        */

#define ARGPARSE_TYPE_MASK  7  /* Mask for the type values (internal).  */

/* A set of macros to make option definitions easier to read.  */
#define ARGPARSE_x(s,l,t,f,d) \
     { (s), (l), ARGPARSE_TYPE_ ## t | (f), (d) }

#define ARGPARSE_s(s,l,t,d) \
     { (s), (l), ARGPARSE_TYPE_ ## t, (d) }
#define ARGPARSE_s_n(s,l,d) \
     { (s), (l), ARGPARSE_TYPE_NONE, (d) }
#define ARGPARSE_s_i(s,l,d) \
     { (s), (l), ARGPARSE_TYPE_INT, (d) }
#define ARGPARSE_s_s(s,l,d) \
     { (s), (l), ARGPARSE_TYPE_STRING, (d) }
#define ARGPARSE_s_l(s,l,d) \
     { (s), (l), ARGPARSE_TYPE_LONG, (d) }
#define ARGPARSE_s_u(s,l,d) \
     { (s), (l), ARGPARSE_TYPE_ULONG, (d) }

#define ARGPARSE_o(s,l,t,d) \
     { (s), (l), (ARGPARSE_TYPE_ ## t  | ARGPARSE_OPT_OPTIONAL), (d) }
#define ARGPARSE_o_n(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_NONE   | ARGPARSE_OPT_OPTIONAL), (d) }
#define ARGPARSE_o_i(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_INT    | ARGPARSE_OPT_OPTIONAL), (d) }
#define ARGPARSE_o_s(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_STRING | ARGPARSE_OPT_OPTIONAL), (d) }
#define ARGPARSE_o_l(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_LONG   | ARGPARSE_OPT_OPTIONAL), (d) }
#define ARGPARSE_o_u(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_ULONG  | ARGPARSE_OPT_OPTIONAL), (d) }

#define ARGPARSE_p(s,l,t,d) \
     { (s), (l), (ARGPARSE_TYPE_ ## t  | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_p_n(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_NONE   | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_p_i(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_INT    | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_p_s(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_STRING | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_p_l(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_LONG   | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_p_u(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_ULONG  | ARGPARSE_OPT_PREFIX), (d) }

#define ARGPARSE_op(s,l,t,d) \
     { (s), (l), (ARGPARSE_TYPE_ ## t \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_op_n(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_NONE \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_op_i(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_INT \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_op_s(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_STRING \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_op_l(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_LONG \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }
#define ARGPARSE_op_u(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_ULONG \
                  | ARGPARSE_OPT_OPTIONAL | ARGPARSE_OPT_PREFIX), (d) }

#define ARGPARSE_c(s,l,d) \
     { (s), (l), (ARGPARSE_TYPE_NONE | ARGPARSE_OPT_COMMAND), (d) }

#define ARGPARSE_ignore(s,l) \
     { (s), (l), (ARGPARSE_OPT_IGNORE), "@" }

#define ARGPARSE_group(s,d) \
     { (s), NULL, 0, (d) }

/* Placeholder options for help, version, warranty and dump-options.  See arg_parse(). */
#define ARGPARSE_end() \
     { 0, NULL, 0, NULL }, \
     { 0, NULL, 0, NULL }, \
     { 0, NULL, 0, NULL }, \
     { 0, NULL, 0, NULL }, \
     { 0, NULL, 0, NULL }


/* Other constants.  */
#define ARGPARSE_PRINT_WARNING  1
#define ARGPARSE_PRINT_ERROR    2


/* Error values.  */
#define ARGPARSE_IS_ARG            (-1)
#define ARGPARSE_INVALID_OPTION    (-2)
#define ARGPARSE_MISSING_ARG       (-3)
#define ARGPARSE_KEYWORD_TOO_LONG  (-4)
#define ARGPARSE_READ_ERROR        (-5)
#define ARGPARSE_UNEXPECTED_ARG    (-6)
#define ARGPARSE_INVALID_COMMAND   (-7)
#define ARGPARSE_AMBIGUOUS_OPTION  (-8)
#define ARGPARSE_AMBIGUOUS_COMMAND (-9)
#define ARGPARSE_INVALID_ALIAS     (-10)
#define ARGPARSE_OUT_OF_CORE       (-11)
#define ARGPARSE_INVALID_ARG       (-12)


int arg_parse (ARGPARSE_ARGS *arg, ARGPARSE_OPTS *opts);
int optfile_parse (FILE *fp, const char *filename, unsigned *lineno,
		   ARGPARSE_ARGS *arg, ARGPARSE_OPTS *opts);
void usage (int level);
const char *strusage (int level);
void set_strusage (const char *(*f)( int ));
void argparse_register_outfnc (int (*fnc)(int, const char *));

#endif /*GNUPG_COMMON_ARGPARSE_H*/
