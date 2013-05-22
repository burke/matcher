#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include "matcher.h"

struct globalArgs_t {
  int dotfiles; // -d
  int limit; // -l
  char *manifest; // -m
  char *search;
} globalArgs;

static const char *optString = "dl:m:h?";

static const struct option longOpts[] = {
  { "no-dotfiles", no_argument, NULL, 'd' },
  { "limit", required_argument, NULL, 'l' },
  { "manifest", required_argument, NULL, 'm' },
  { "help", no_argument, NULL, 'h' },
  { NULL, no_argument, NULL, 0 }
};

/* Display program usage, and exit. */
void display_usage(void)
{
  puts("Usage: matcher [--no-dotfiles] [--limit num] [--manifest filename] <query>\n");
  exit( EXIT_FAILURE );
}

void parse_arguments(int argc, char *argv[])
{
  int opt = 0;
  int longIndex = 0;

  globalArgs.dotfiles = 1;
  globalArgs.limit = 10;
  globalArgs.manifest = NULL;
  globalArgs.search = NULL;

  opt = getopt_long(argc, argv, optString, longOpts, &longIndex);

  while (opt != -1) {
    switch (opt) {
      case 'l':
        globalArgs.limit = atoi(optarg);
        break;
      case 'd':
        globalArgs.dotfiles = 0;
        break;
      case 'm':
        globalArgs.manifest = optarg;
        break;
      case 'h':
      case '?':
        display_usage();
        break;
      default:
        break;
    }
    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
  }

  globalArgs.search = (optind < argc) ? argv[optind] : "";

	int i = 0;
	while(globalArgs.search[i] != '\0'){
		globalArgs.search[i] = tolower(globalArgs.search[i]);
		i++;
	}
}

int main(int argc, char *argv[])
{
  parse_arguments(argc, argv);

  char *strings[20000];
  int num_strings = 0;

  FILE *fp = stdin;
  if (globalArgs.manifest) {
    fp = fopen(globalArgs.manifest, "r");
  }

  for (num_strings = 0; num_strings < 20000; num_strings++) {
    strings[num_strings] = malloc(1024 * sizeof(char));
    fgets(strings[num_strings], 1023, fp);
    if (feof(fp)) break;
  }

  fclose(fp);

  score_list(globalArgs.search,
             strings,
             num_strings,
             globalArgs.dotfiles,
             globalArgs.limit);
  return 0;
}

