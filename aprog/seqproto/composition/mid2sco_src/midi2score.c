
/* midi2score.c - MIDI to score conversion functions */

#include "main.h"

/* open MIDI file and read header */

void	open_midi_file (midi_file *mfile)
{
	int	i;

	/* load file into memory */

	load_midi_file (mfile);

	/* check header */

	if (get_int (mfile, 4) != 0x4D546864)		/* magic number */
		err_msg ("invalid MIDI header");
	if (get_int (mfile, 4) != 0x00000006)		/* header length */
		err_msg ("invalid MIDI header");
	i = get_int (mfile, 2);				/* file type */
	if ((i != 0) && (i != 1))
		err_msg ("file format not supported");
	mfile->nr_tracks = get_int (mfile, 2);		/* number of tracks */

	i = get_int (mfile, 2);			/* ticks per quarter */
	if (i & 0x8000) {
		if (!(i & 0xFF))
			err_msg ("invalid tick time");
		mfile->tick_beat = 0.0;
		mfile->tick_sec = 1.0 / (double) (i & 0xFF);
		i = (i >> 8) - 256;
		switch (i) {
			case -24:	mfile->tick_sec /= 24.00; break;
			case -25:	mfile->tick_sec /= 25.00; break;
			case -29:	mfile->tick_sec /= 30.00; break;
			case -30:	mfile->tick_sec /= 29.96; break;
			default:	err_msg ("invalid tick time");
		}
	} else {
		if (!(i)) err_msg ("invalid tick time");
		mfile->tick_beat = 1.0 / (double) i;
		mfile->tick_sec = 0.0;
	}
	mfile->tempo = 120.0;		/* initialize tempo */
}

/* read and convert all tracks in MIDI file */

void	convert_all_tracks (midi_file *mfile, score_file *scfile)
{
	int	i, j, k;
	double	d;

	mfile->cur_track = -1;
	while (++(mfile->cur_track) < mfile->nr_tracks) {
		/* check track header */
		i = get_int (mfile, 4);		/* magic number ("MTrk") */
		j = get_int (mfile, 4);		/* track length in bytes */
		if (i != 0x4D54726B)
			err_msg ("invalid track header");
		mfile->prv_event = -1;
		mfile->cur_time = 0.0;
		for (i = 0; i < 16; i++)
			for (j = 0; j < 128; j++)
				mfile->notes[i][j].note_on_time = -1.0;
		sc_printf (scfile, "\n; track %d\n\n", mfile->cur_track + 1);

		do {		/* read all events */

			j = get_dyn_data (mfile);	/* update time ptr */
			d = mfile->tick_beat + mfile->tick_sec
						* (mfile->tempo / 60.0);
			mfile->cur_time += (double) j * d;

			/* get next event */

			mfile->cur_event = get_int (mfile, 1);
			if (mfile->cur_event < 0x80) {	/* repeat old event */
				(mfile->pos)--;		/* type		    */
				mfile->cur_event = mfile->prv_event;
			}
			i = mfile->prv_event = mfile->cur_event;

			if (i < 0x80) err_msg ("invalid event type");

			if (i < 0xA0) {			/* note event */
				/* note number */
				j = get_int (mfile, 1) & 0x7F;
				/* velocity */
				k = get_int (mfile, 1) & 0x7F;
				if ((i >= 0x90) && k) {		/* note-on */
					i &= 0x0F;	/* channel */
					if (mfile->notes[i][j].note_on_time
					    >= 0.0) err_msg
						    ("invalid note-on event");
					/* store note-on parameters */
					mfile->notes[i][j].channel = i;
					mfile->notes[i][j].note_num = j;
					mfile->notes[i][j].note_on_time =
						mfile->cur_time;
					mfile->notes[i][j].note_on_velocity = k;
					continue;
				}				/* note-off */
				i &= 0x0F;		/* channel */
				if (mfile->notes[i][j].note_on_time < 0.0)
					err_msg ("invalid note-off event");
				/* store note-off parameters */
				mfile->notes[i][j].note_off_velocity = k;
				mfile->notes[i][j].note_off_time =
					mfile->cur_time;
				mfile->notes[i][j].duration =
					mfile->notes[i][j].note_off_time
					- mfile->notes[i][j].note_on_time;
				/* print note */
				print_note (mfile, scfile, i, j);
				/* clear note-on */
				mfile->notes[i][j].note_on_time = -1.0;
				continue;
			}

			/* these event types are ignored */

			if (i < 0xB0) {		/* polyphonic pressure	*/
				j = get_int (mfile, 2); continue;
			}

			if (i < 0xC0) {		/* control change	*/
				j = get_int (mfile, 2); continue;
			}

			if (i < 0xD0) {		/* program change	*/
				j = get_int (mfile, 1); continue;
			}

			if (i < 0xE0) {		/* channel pressure	*/
				j = get_int (mfile, 1); continue;
			}

			if (i < 0xF0) {		/* pitch bend		*/
				j = get_int (mfile, 2); continue;
			}

			/* exclusive message */

			if (i < 0xFF) {
				j = get_dyn_data (mfile);	/* length */
				get_int (mfile, j);	/* skip message */
				continue;
			}

			/* meta-events */

			i = get_int (mfile, 1);		/* event type */
			if (i == 0x51) {		/* tempo */
				if ((mfile->tick_sec != 0.0)
				    && (mfile->cur_time != 0.0)
				    && (mfile->nr_tracks > 1))
					err_msg ("invalid tempo change");
				j = get_dyn_data (mfile);	/* length (3) */
				k = get_int (mfile, 3);	/* usec / quarter */
				if ((j != 3) || (k == 0))
					err_msg ("invalid tempo change");
				mfile->tempo = 6.0e7 / (double) k;
				print_tempo (mfile, scfile);
				continue;
			} else if (i == 0x2F) {		/* end of track */
				j = get_dyn_data (mfile);	/* length (0) */
				if (j != 0)
					err_msg ("invalid end of track event");
				break;
			} else {		/* skip unknown meta-events */
				j = get_dyn_data (mfile);	/* length */
				get_int (mfile, j);
				continue;
			}
		} while (1);		/* next event */
		/* turn off all notes */
		for (i = 0; i < 16; i++) {
			for (j = 0; j < 127; j++) {
				if (mfile->notes[i][j].note_on_time < 0.0)
					continue;
				/* store note-off parameters */
				mfile->notes[i][j].note_off_velocity = 64;
				mfile->notes[i][j].note_off_time =
					mfile->cur_time;
				mfile->notes[i][j].duration =
					mfile->notes[i][j].note_off_time
					- mfile->notes[i][j].note_on_time;
				/* print note */
				print_note (mfile, scfile, i, j);
				/* clear note-on */
				mfile->notes[i][j].note_on_time = -1.0;
			}
		}
	}			/* next track */

	sc_printf (scfile, "\n");
}

