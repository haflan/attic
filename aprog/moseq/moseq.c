#include "audio.h"
#include "gui.h"

int
main (int argc, char **argv)
{
    GtkApplication *app;
    int gtkstatus;

    // JACK --------------
    jack_setup();
    // --------------------

    // GTK GUI ------------
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    gtkstatus = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    // --------------------
    
    /* keep running (jack) until stopped by the user */
    //sleep (-1);
    //free(sample);

    /* this is never reached but if the program
       had some other way to exit besides being killed,
       they would be important to call.
    */

    jack_client_close (client);
    exit (0);

    return gtkstatus;
}
