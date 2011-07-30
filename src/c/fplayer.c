//#!/usr/bin/tcc -run
/****************************************************************
 * $ID: fplayer.c      Thu, 30 Mar 2006 09:44:29 +0800  mhfan $ *
 *                                                              *
 * Description:                                                 *
 *                                                              *
 * Maintainer:  ∑∂√¿ª‘(MeiHui FAN)  <mhfan@ustc.edu>            *
 *                                                              *
 * CopyRight (c)  2006  M.H.Fan                                 *
 *   All rights reserved.                                       *
 *                                                              *
 * This file is free software;                                  *
 *   you are free to modify and/or redistribute it   	        *
 *   under the terms of the GNU General Public Licence (GPL).   *
 ****************************************************************/

#include "swf.h"
#include "bench.h"
#include "common.h"
#include "version.h"

#if 0
#define	_GNU_SOURCE			1
#include <getopt.h>
#else
#include <argp.h>
#endif

#define	EXIT_FAILURE			1

#ifdef	ENABLE_NLS
#include <libintl.h>
#define _(Text)				gettext(Text)
#else
#define textdomain(Domain)
#define	_(Text)				Text
#endif//ENABLE_NLS
#define N_(Text)			Text

struct arg_opt {   // Option flags and variables, initialized in parse_opts
    FILE *of, *lf;
    char /**on, *ln, */*cd, *dir;
    int interactive, quiet, brief, verbose, dry_run, no_warn;
    struct { int nfile;  char** files; };
}   opts;

enum {	DUMMY_KEY = 129,		// argp option keys
	BRIEF_KEY, DRYRUN_KEY, NOWARN_KEY, CD_KEY, DIR_KEY
};

static struct argp_option argp_opts[] = {
    { "interactive",	'i',		NULL,		0,
	N_("Prompt for confirmation"),  0 },
    { "output",		'o',		N_("FILE"),	0,
	N_("Send output to FILE instead of standard output"), 0 },
    { "quiet",		'q',		NULL,		0,
	N_("Inhibit usual output"), 0 },
    { "silent",	0, NULL, OPTION_ALIAS,  NULL, 0 },
    { "brief",		BRIEF_KEY,	NULL,		0,
	N_("Shorten output"), 0 },
    { "verbose",	'v',		NULL,		0,
	N_("Print more information"), 0 },
    { "dry-run",	DRYRUN_KEY,	NULL,		0,
	N_("Take no real actions"), 0 },
    { "no-warn",	NOWARN_KEY,	NULL,		0,
	N_("Disable warnings"), 0 },
    { "cd",		CD_KEY,		N_("DIR"),	0,
	    N_("Change to DIR before proceeding"), 0 },
    { "dir",		DIR_KEY,	N_("DIR"), 0,
	    N_("Use directory DIR"), 0 },
    { NULL, 0, NULL, 0, NULL, 0 }
};

static error_t parse_opts(int key, char* arg, struct argp_state* state)
{   // Parse a single option.
    switch (key) {
    case ARGP_KEY_INIT:
	opts.nfile = 0;
	opts.files = NULL;
	opts.cd = opts.dir = NULL;
	opts.of = opts.lf  = stdout;
	//opts.on = opts.ln  = "stdout";
	opts.quiet = opts.dry_run = opts.interactive =
	opts.brief = opts.no_warn = opts.verbose = 0;
					break;	// Set up default values.
    case 'o':	//opts.on = arg;
	if (!(opts.of = fopen(arg, "w")))
	    argp_failure(state, EXIT_FAILURE, errno,
			 _("Cannot open %s for writing"), arg);
					break;	// --output
    case 'l':	//opts.ln = arg;
	if (!(opts.lf = fopen(arg, "w")))
	    argp_failure(state, EXIT_FAILURE, errno,
			 _("Cannot open %s for writing"), arg);
					break;	// --log
    case 'q':	opts.quiet = 1;		break;	// --quiet, --silent
    case 'v':	opts.verbose = 1;	break;	// --verbose
    case 'i':	opts.interactive = 1;	break;	// --interactive
    case BRIEF_KEY:  opts.brief = 1;	break;	// --brief
    case DRYRUN_KEY: opts.dry_run = 1;	break;  // --dry-run
    case NOWARN_KEY: opts.no_warn = 1;	break;	// --no-warn
    case DIR_KEY:    opts.dir = arg;	break;	// --directory
    case CD_KEY:     opts.cd  = arg;	break;	// --cd
    case ARGP_KEY_ARG:
	opts.files = realloc(opts.files, ++opts.nfile * sizeof(char*));
	opts.files[opts.nfile-1] = arg;	break;	// [FILE]...
    case ARGP_KEY_NO_ARGS:  argp_usage(state);
    default:				return ARGP_ERR_UNKNOWN;
    }					return 0;
}

static void show_version(FILE* stream, struct argp_state* state)
{   // Show the version number and copyright information.
    (void)state;			// XXX
    // Print in small parts whose localizations can hopefully
    // be copied from other programs.
    fputs(PACKAGE_NAME " " VERSION_STRING "\n", stream);
    fprintf(stream, _("Written by %s.\n\n"), AUTHOR_STRING);
    fprintf(stream, _("Copyright (C) %s %s\n"), "2006", AUTHOR_STRING);
    fputs(_("\
This program is free software; you may redistribute it under the terms of\n\
the GNU General Public License.  This program has absolutely no warranty.\n"),
	    stream);
}

// The argp functions examine these global variables.
const char* argp_program_bug_address = AUTHOR_EMAIL;
void (*argp_program_version_hook)(FILE*, struct argp_state*) = show_version;

static struct argp argp = {
    argp_opts, parse_opts, N_("[FILE...]"),
    N_("Macromedia Shock-Wave Flash file parser"),
    NULL, NULL, NULL
};

int main(int argc, char* argv[])
{
    struct swf* swf;
    textdomain(PACKAGE_NAME);
    argp_parse(&argp, argc, argv, 0, NULL, NULL);

    for (argc=0; argc < opts.nfile; ++argc) {
	if (!(swf = swf_open(opts.files[argc]))) continue;
	swf_render_init(&swf->render);
	swf_tagpsr_init( swf->tagpsr);
	swf->render.init(&swf->render, &swf->mh.fs);
	while (swf_decode_frame(swf));
	swf_close (swf);
    }

    fclose(opts.of);			free(opts.files);
    fclose(opts.lf);			return 0;
}

// vim:sts=4:ts=8:
