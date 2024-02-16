
/* fileio.c - file input / output functions */

#include "main.h"

#define BLKSIZE		4096		/* read block size */

/* print error message */

void	err_msg (char *errmsg)
{
	fprintf (stderr, "mid2sco: %s\n", errmsg);
	exit (-1);
}

/* print error message with string */

void	err_msg_s (char *errmsg, char *s)
{
	fprintf (stderr, "mid2sco: %s: %s\n", errmsg, s);
	exit (-1);
}

/* print error message with filename and string */

void	err_msg_s2 (char *errmsg, char *fname, char *s)
{
	fprintf (stderr, "mid2sco: %s %s: %s\n", errmsg, fname, s);
	exit (-1);
}

/* load MIDI file into memory */

void	load_midi_file (midi_file *mfile)
{
	int	i, m;
	FILE	*infile;

	/* open input file */

	if (!(strcmp (mfile->name, "-"))) {
		infile = stdin;			/* read from stdin */
	} else {
		infile = fopen (mfile->name, "rb");
		if (infile == NULL) err_msg_s2 ("error opening", mfile->name,
							strerror (errno));
	}

	mfile->fsize = m = 0;		/* file size */
	mfile->mdata = NULL;

	do {
		/* allocate memory for MIDI file */

		while ((m - mfile->fsize) < BLKSIZE) {
			m += BLKSIZE;
			mfile->mdata = (unsigned char *)
					realloc ((void *) mfile->mdata,
						 sizeof (unsigned char) * m);
			if (mfile->mdata == NULL)
				err_msg ("not enough memory");
		}

		/* read next block */

		i = fread (mfile->mdata + mfile->fsize, sizeof (unsigned char),
			   BLKSIZE, infile);
		mfile->fsize += i;
	} while (i == BLKSIZE);

	if (ferror (infile)) {
		if (!(strcmp (mfile->name, "-"))) {
			err_msg_s2 ("error reading from", "stdin",
				    strerror (errno));
		} else {
			err_msg_s2 ("error reading from", mfile->name,
				    strerror (errno));
		}
	}

	if (strcmp (mfile->name, "-")) fclose (infile);

	mfile->pos = 0;			/* read position	*/
}

/* free memory used by MIDI file structure */

void	close_midi_file (midi_file *mfile)
{
	free (mfile->mdata); mfile->fsize = 0;
}

/* read an integer with length of n bytes */

int	get_int (midi_file *mfile, int n)
{
	int	x, i;

	i = x = 0; while (i++ < n) {
		if (mfile->pos >= mfile->fsize)
			err_msg ("unexpected end of MIDI file");
		x = ((x & 0x007FFFFF) << 8) + (int) mfile->mdata[mfile->pos];
		(mfile->pos)++;
	}

	return x;
}

/* read a dynamic length value */

int	get_dyn_data (midi_file *mfile)
{
	int	x, i, j;

	i = x = 0; do {
		j = get_int (mfile, 1);
		x = ((x & 0x00FFFFFF) << 7) + (j & 0x7F);
		if (++i > 4) err_msg ("invalid dynamic length value");
	} while (j & 0x80);

	return x;
}

/* open output file */

void	open_output_file (score_file *outfile)
{
	if (!(strcmp (outfile->name, "-"))) {
		outfile->sfile = stdout;		/* output to stdout */
	} else {
		outfile->sfile = fopen (outfile->name, "wt");
		if (outfile->sfile == NULL)
			err_msg_s2 ("error opening", outfile->name,
					strerror (errno));
	}
}

/* close output file */

void	close_output_file (score_file *outfile)
{
	fflush (outfile->sfile);
	if (ferror (outfile->sfile)) {
		if (strcmp (outfile->name, "-")) {
			err_msg_s2 ("error writing to", outfile->name,
					strerror (errno));
		} else {
			err_msg_s2 ("error writing to", "stdout",
					strerror (errno));
		}
	}
	if (strcmp (outfile->name, "-")) {
		if (fclose (outfile->sfile))
			err_msg_s2 ("error closing", outfile->name,
					strerror (errno));
	}
}

/* print to score file with error checking */

void	sc_printf (score_file *scfile, char *format, ...)
{
	va_list	args;

	va_start (args, format);
	vfprintf (scfile->sfile, format, args);
	va_end (args);
	if (ferror (scfile->sfile)) {
		if (strcmp (scfile->name, "-")) {
			err_msg_s2 ("error writing to", scfile->name,
					strerror (errno));
		} else {
			err_msg_s2 ("error writing to", "stdout",
					strerror (errno));
		}
	}
}

/* print note event */

void	print_note (midi_file *mfile, score_file *scfile, int chn, int note)
{
	sc_printf (scfile, "i %d\t%10.4f\t%10.4f\t%3d\t%3d",
			(scfile->trk_instr ? mfile->cur_track : chn) + 1,
			mfile->notes[chn][note].note_on_time,
			mfile->notes[chn][note].duration,
			mfile->notes[chn][note].note_num,
			mfile->notes[chn][note].note_on_velocity);
	if (scfile->off_vel)
		sc_printf (scfile, "\t%3d",
				mfile->notes[chn][note].note_off_velocity);
	sc_printf (scfile, "\n");
}

/* print tempo value */

void	print_tempo (midi_file *mfile, score_file *scfile)
{
	sc_printf (scfile, ";t\t%10.4f\t%10.4f\n",
			mfile->cur_time, mfile->tempo);
}

