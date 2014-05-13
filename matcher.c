/*
 * Most of this is borrowed from Command-T:
 *   https://github.com/wincent/Command-T/blob/4b2da2fb/ruby/command-t/match.c
 * The rest is also under standard 2-clause BSD license, 2012 Burke Libbey
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// use a struct to make passing params during recursion easier
typedef struct
{
  char    *str_p;                 // pointer to string to be searched
  long    str_len;                // length of same
  char    *abbrev_p;              // pointer to search string (abbreviation)
  long    abbrev_len;             // length of same
  double  max_score_per_char;
  int     dot_file;               // boolean: true if str is a dot-file
  int     always_show_dot_files;  // boolean
  int     never_show_dot_files;   // boolean
} matchinfo_t;

#ifdef _WIN32
#define IS_PATHSEP(c) (c == '\\' || c == '/')
#else
#define IS_PATHSEP(c) (c == '/')
#endif

double recursive_match(matchinfo_t *m,  // sharable meta-data
                       long str_idx,    // where in the path string to start
                       long abbrev_idx, // where in the search string to start
                       long last_idx,   // location of last matched character
                       double score)    // cumulative score so far
{
  double seen_score = 0;      // remember best score seen via recursion
  int dot_file_match = 0;     // true if abbrev matches a dot-file
  int dot_search = 0;         // true if searching for a dot

  long i, j;

  for (i = abbrev_idx; i < m->abbrev_len; i++) {
    char c = m->abbrev_p[i];
    if (c == '.')
      dot_search = 1;
    int found = 0;
    for (j = str_idx; j < m->str_len; j++, str_idx++) {
      char d = m->str_p[j];
      if (d == '.') {
        if (j == 0 || IS_PATHSEP(m->str_p[j - 1])) {
          m->dot_file = 1;        // this is a dot-file
          if (dot_search)         // and we are searching for a dot
            dot_file_match = 1; // so this must be a match
        }
      }
      else if (d >= 'A' && d <= 'Z')
        d += 'a' - 'A'; // add 32 to downcase
      if (c == d) {
        found = 1;
        dot_search = 0;

        // calculate score
        double score_for_char = m->max_score_per_char;
        long distance = j - last_idx;
        if (distance > 1) {
          double factor = 1.0;
          char last = m->str_p[j - 1];
          char curr = m->str_p[j]; // case matters, so get again
          if (IS_PATHSEP(last))
            factor = 0.9;
          else if (last == '-' ||
                  last == '_' ||
                  last == ' ' ||
                  (last >= '0' && last <= '9'))
            factor = 0.8;
          else if (last >= 'a' && last <= 'z' && curr >= 'A' && curr <= 'Z')
            factor = 0.8;
          else if (last == '.')
            factor = 0.7;
          else
            // if no "special" chars behind char, factor diminishes
            // as distance from last matched char increases
            factor = (1.0 / distance) * 0.75;
          score_for_char *= factor;
        }

        if (++j < m->str_len) {
          // bump cursor one char to the right and
          // use recursion to try and find a better match
          double sub_score = recursive_match(m, j, i, last_idx, score);
          if (sub_score > seen_score)
            seen_score = sub_score;
        }

        score += score_for_char;
        last_idx = str_idx++;
        break;
      }
    }
    if (!found)
      return 0.0;
  }
  if (m->dot_file) {
    if (m->never_show_dot_files || (!dot_file_match && !m->always_show_dot_files))
      return 0.0;
  }
  return (score > seen_score) ? score : seen_score;
}

double score(char *abbrev,      // user input string to search for
             char *str,         // proposed potential match to calculate score against
             int show_dotfiles) // bool
{
  long i;

  matchinfo_t m;
  m.str_p                 = str;
  m.str_len               = strlen(str);
  m.abbrev_p              = abbrev;
  m.abbrev_len            = strlen(abbrev);
  m.max_score_per_char    = (1.0 / m.str_len + 1.0 / m.abbrev_len) / 2;
  m.dot_file              = 0;
  m.always_show_dot_files = show_dotfiles == 1;
  m.never_show_dot_files  = show_dotfiles == 0;

  // calculate score
  double score = 1.0;
  if (m.abbrev_len == 0) { // special case for zero-length search string
    // filter out dot files
    if (!m.always_show_dot_files) {
      for (i = 0; i < m.str_len; i++) {
        char c = m.str_p[i];
        if (c == '.' && (i == 0 || IS_PATHSEP(m.str_p[i - 1]))) {
          score = 0.0;
          break;
        }
      }
    }
  } else { // normal case
    score = recursive_match(&m, 0, 0, 0, 0.0);
  }

  return score;
}

typedef struct
{
  char *ptr;
  double score;
} item_t;

int compare_items(const void *a, const void *b)
{
  item_t *at = (item_t *)a;
  item_t *bt = (item_t *)b;
  if (at->score > bt->score)
    return -1;
  else if (at->score < bt->score)
    return 1;
  else
    return 0;
}

void score_list(char *abbrev,
                char **strs,
                int num_strs,
                int show_dotfiles,
                int limit)
{
  long i;
  long abbrev_len;

  abbrev_len = strlen(abbrev);
  item_t items[num_strs];

  for (i = 0; i < num_strs; i++) {
    items[i].ptr   = strs[i];
    items[i].score = score(abbrev, strs[i], show_dotfiles);
  }

  qsort(items, num_strs, sizeof(item_t), compare_items);

  if (num_strs < limit) limit = num_strs;
  for (i = 0; i < limit; i++) {
    // Filter out 0 score items unless no search term has been passed in
    if (items[i].score > 0 || abbrev_len == 0) {
      printf("%s", items[i].ptr);
    }
  }

}

