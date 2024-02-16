
/* mid2sco - MIDI file to score conversion utility	*/
/* written by Istvan Varga, Aug 2001			*/

#include "main.h"

int	main (int argc, char **argv)
{
	midi_file	infile;			/* MIDI file for input	*/
	score_file	outfile;		/* output score file	*/
	char		def_name[2] = "-";	/* default file name	*/
	int		i;

	/* default settings */

	infile.name = def_name;			/* read from stdin	*/
	infile.fsize = 0;
	infile.mdata = NULL;

	outfile.name = def_name;		/* write to stdout	*/
	outfile.sfile = NULL;
	outfile.trk_instr = 0;			/* instr = channel	*/
	outfile.off_vel = 0;			/* do not print		*/
						/* note-off velocity	*/
	/* command-line options */

	i = 0; while (++i < argc) {
		if (!(strcmp (argv[i], "-h"))) {	/* print usage	     */
			fprintf (stderr, "usage: mid2sco [options ...]\n\n");
			fprintf (stderr, "options:\n");
			fprintf (stderr, "    -i: input file name");
			fprintf (stderr, " (default: read from stdin)\n");
			fprintf (stderr, "    -o: output file name");
			fprintf (stderr, " (default: write to stdout)\n");
			fprintf (stderr, "    -t: instrument = track number");
			fprintf (stderr, " (default: instrument = channel)\n");
			fprintf (stderr, "    -V: print note-off velocity\n");
			fprintf (stderr, "\n");
			exit (0);
		} else if (!(strcmp (argv[i], "-i"))) {	/* input file	     */
			if (++i >= argc)
				err_msg ("missing filename for -i");
			infile.name = argv[i];
		} else if (!(strcmp (argv[i], "-o"))) {	/* output file	     */
			if (++i >= argc)
				err_msg ("missing filename for -o");
			outfile.name = argv[i];
		} else if (!(strcmp (argv[i], "-t"))) {	/* instr = track     */
			outfile.trk_instr = 1;
		} else if (!(strcmp (argv[i], "-V"))) {	/* note-off velocity */
			outfile.off_vel = 1;
		} else {
			err_msg_s ("invalid option", argv[i]);
		}
	}

	/* load MIDI file into memory */

	open_midi_file (&infile);

	/* open score file for writing */

	open_output_file (&outfile);

	/* convert MIDI to score */

	convert_all_tracks (&infile, &outfile);

	/* close files */

	close_output_file (&outfile);
	close_midi_file (&infile);

	return 0;
}

