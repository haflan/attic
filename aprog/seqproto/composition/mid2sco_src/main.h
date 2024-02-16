
/* mid2sco - MIDI file to score conversion utility	*/
/* written by Istvan Varga, Aug 2001			*/

#ifndef MID2SCO_MAIN_H
#define MID2SCO_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct {
	int	channel;		/* channel number (0 - 15)	*/
	double	note_on_time;		/* note-on time in beats	*/
	double	note_off_time;		/* note-off time in beats	*/
	double	duration;		/* note duration in beats	*/
	int	note_num;		/* note number			*/
	int	note_on_velocity;	/* note-on velocity		*/
	int	note_off_velocity;	/* note-off velocity		*/
} midi_note;

typedef struct {
	char		*name;		/* file name			*/
	int		fsize;		/* file size in bytes		*/
	unsigned char	*mdata;		/* MIDI file loaded into memory	*/
	int		nr_tracks;	/* number of tracks		*/
	int		cur_track;	/* current track number		*/
	int		pos;		/* read position		*/
	int		cur_event;	/* current event		*/
	int		prv_event;	/* previous event		*/
	double		tempo;		/* tempo in beats per minute	*/
	double		tick_beat;	/* ticks per beat		*/
	double		tick_sec;	/* ticks per second		*/
	double		cur_time;	/* current time in beats	*/
	midi_note	notes[16][128];	/* note table for current track	*/
} midi_file;

typedef struct {
	FILE		*sfile;		/* output score file		*/
	char		*name;		/* output file name		*/
	int		trk_instr;	/* 0: instr = channel number,	*/
					/* 1: instr = track number	*/
	int		off_vel;	/* enable printing of note-off	*/
					/* velocity			*/
} score_file;

/* -------- functions from fileio.c -------- */

/* print error message */

void	err_msg (char*);

/* print error message with string */

void	err_msg_s (char*, char*);

/* print error message with filename and string */

void	err_msg_s2 (char*, char*, char*);

/* load MIDI file into memory */

void	load_midi_file (midi_file*);

/* free memory used by MIDI file structure */

void	close_midi_file (midi_file*);

/* read an integer with length of n bytes */

int	get_int (midi_file*, int);

/* read a dynamic length value */

int	get_dyn_data (midi_file*);

/* open output file */

void	open_output_file (score_file*);

/* close output file */

void	close_output_file (score_file*);

/* print to score file with error checking */

void	sc_printf (score_file*, char*, ...);

/* print note event */

void	print_note (midi_file*, score_file*, int, int);

/* print tempo value */

void	print_tempo (midi_file*, score_file*);

/* -------- functions from midi2score.c -------- */

/* open MIDI file and read header */

void	open_midi_file (midi_file*);

/* read and convert all tracks in MIDI file */

void	convert_all_tracks (midi_file*, score_file*);

#endif			/* MID2SCO_MAIN_H */

