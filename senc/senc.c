#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// IDEA: Color results based on match level
// or even better, color matching characters

#define MAX_RESULTS 50
#define MAX_PATH_LENGTH 256

/**
 * Search match level
 * FUZZY example: search 'his ring' will match on 'this string' 
 */
enum {NONE, FUZZY, NOCASE, EXACT};

FILE * fp;
typedef struct {
	char * filename;
	int matchlvl;
} match;

int
search(char * search_string, char * filename) {
	int search_len = strlen(search_string);
	int filename_len = strlen(filename);
	if (search_len > filename_len) {
		return NONE;
	}
	// Exact matches
	int mchars = 0;
	for (int i = 0; i < filename_len; i++) {
		if (search_string[mchars] == filename[i]) {
			mchars++;
		} else {
			mchars = 0;
		}
		if (mchars == search_len) {
			return EXACT;
		}
	}
	// Case insensitive matches
	mchars = 0;
	for (int i = 0; i < filename_len; i++) {
		if (tolower(search_string[mchars]) == tolower(filename[i])) {
			mchars++;
		} else {
			mchars = 0;
		}
		if (mchars == search_len) {
			return NOCASE;
		}
	}
	// Weak matches
	mchars = 0;
	for (int i = 0; i < filename_len; i++) {
		if (tolower(search_string[mchars]) == tolower(filename[i])) {
			mchars++;
		}
		if (mchars == search_len) {
			return FUZZY;
		}
	}
	return NONE;
}


/**
 * Searches favorite paths and stores matches in given matches variable
 */
int
search_favs(match * matches, char * search_string) {
	char buf[LINE_MAX];
	int line_match;
	int read_chars;
	int match_i = 0;
	// TODO: Find out if getline is the preferred way
	while (fgets(buf, LINE_MAX, fp)) {
		line_match = search(search_string, buf);
		int len = strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
		}
		if (line_match == NONE) {
			continue;
		}
		// Have to copy string, because setting 
		// filename = buf, will just make all filenames point to
		// the same memory location and overwrite each other.
		strcpy(matches[match_i].filename, buf);
		matches[match_i].matchlvl = line_match;
		match_i++;
		if (match_i == MAX_RESULTS) {
			break;
		}
	}
	return match_i;
}

int
add_entry(char * new) {
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	/*
	const char * str = "Hello yy'aaalll\n";
	fwrite(str, sizeof(char), strlen(str), f);
	if (errno) {
		printf("errno = %d\n", errno);
	}
	*/
	return 0;
}

/**
 * Sort matches by matchlvl
 */
int
compare_matches(const void * e1, const void * e2) {
	match m1 = *((match*) e1);
	match m2 = *((match*) e2);
	return m2.matchlvl - m1.matchlvl;
}

char *
matchlvl_to_string(int match_level) {
	switch (match_level) {
		case FUZZY: return "fuzzy";
		case NOCASE: return "case ignored";
		case EXACT: return "exact";
	}
}

int
main(int nargs, char ** args) {
	char * favs_file;
	char * term;
	int i;
	if (nargs < 2) {
		printf(
			"Usage: senc [favorites file] <search term>\n"
		);
		exit(EXIT_FAILURE);
	} else if (nargs < 3) {
		favs_file = strcat(getenv("HOME"), "/.sencs");
		term = args[1];
	} else {
		favs_file = args[1];
		term = args[2];
	}
	if ((fp = fopen(favs_file, "a+")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	match matches[MAX_RESULTS];
	for (i = 0; i < MAX_RESULTS; i++) {
		matches[i].filename = malloc(LINE_MAX);
	}
	int match_i = search_favs(matches, term);
	qsort(matches, match_i, sizeof(match), compare_matches);
	for (i = 0; i < match_i; i++) {
		printf("%s\t \e[1m%s\e[0m \n", matches[i].filename,
				matchlvl_to_string(matches[i].matchlvl));
	}
	fclose(fp);
}
