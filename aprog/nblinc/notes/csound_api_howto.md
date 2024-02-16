Using the Csound API
====================
1. Install the library dev files:
    sudo apt-get install libcsound64-dev

2. csound.h should now be placed in `/usr/include/csound`, so include it like this:
    <csound/csound.h>

3. In the makefile, link to the csound library:
    -lcsound64
