
# --------------------------- Makefile for mid2sco ----------------------------

CC ?= gcc
STRIP = strip
CP = cp -af
RM = rm -f
CHOWN = chown
CHMOD = chmod
CFLAGS ?= -Wall -O2		# default compiler flags
CFLAGS += -W -ansi -pedantic	# some more warning options

PGM = mid2sco
INSTDIR = /usr/local/bin
USER = 0:0
PERM = 555

# -----------------------------------------------------------------------------

OBJS = fileio.o midi2score.o main.o
LIBS = -lm

all: $(PGM)

$(PGM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $+ $(LIBS)

%.o: %.c main.h
	$(CC) $(CFLAGS) -c $< -o $@

uninstall:
	$(RM) "$(INSTDIR)/$(PGM)"

install: uninstall $(PGM)
	$(CP) "./$(PGM)" "$(INSTDIR)"/
	-$(CHOWN) "$(USER)" "$(INSTDIR)/$(PGM)"
	-$(CHMOD) "$(PERM)" "$(INSTDIR)/$(PGM)"

install-strip: uninstall $(PGM)
	$(CP) "./$(PGM)" "$(INSTDIR)"/
	$(STRIP) "$(INSTDIR)/$(PGM)"
	-$(CHOWN) "$(USER)" "$(INSTDIR)/$(PGM)"
	-$(CHMOD) "$(PERM)" "$(INSTDIR)/$(PGM)"

clean:
	$(RM) $(OBJS)

clean-all:
	$(RM) $(OBJS) $(PGM)

# -----------------------------------------------------------------------------

