#include <stdio.h>
#include <stdlib.h>

#include "matcher.h"

int main(int argc, char *argv[])
{
  int limit;
  int show_dotfiles = 1;
  char *abbrev;
  char *strings[20000];
  int num_strings = 0;

  if (argc < 2) {
    printf("Usage: matcher [options] <search>\n");
    return 1;
  }

  limit  = atoi(argv[1]);
  show_dotfiles = atoi(argv[2]);

  abbrev = argv[3];

  for (num_strings = 0; num_strings < 20000; num_strings++) {
    strings[num_strings] = malloc(1024 * sizeof(char));
    fgets(strings[num_strings], 1023, stdin);
    if (feof(stdin)) break;
  }

  score_list(abbrev, strings, num_strings, show_dotfiles, limit);
  return 0;
}

