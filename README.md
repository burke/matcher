# Matcher

This is a standalone library that does the same fuzzy-find matching as Command-T.vim.

# Installation

```
$ make
# move `matcher` somewhere useful
```

# Usage

Matcher searches for a string in a list of filenames, and returns the
ones it thinks you are most likely referring to. It works exactly like
fuzzy-finder, Command-T, and so on.

### Usage:

```shell
$ matcher [options] <search>
```

#### Options:

* `--limit`: The number of matches to return (default 10)
* `--no-dotfiles`: Dotfiles will never be returned (by default, they may
  be)
* `--manifest`: Specify a file containing the list of files to scan. If
  none given, matcher will read the list from stdin.

### Examples

```shell
$ matcher --limit 20 --no-dotfiles --manifest filelist.txt customer.rb
$ find . | matcher order
```

# Using with CtrlP.vim

```viml
function! g:GoodMatch(items, str, limit, mmode, ispath, crfile, regex)
  " the Command-T matcher doesn't do regex. Return now if that was requested.
  if a:regex == 1
    let [lines, id] = [[], 0]
    for item in a:items
      let id += 1
      try | if !( a:ispath && item == a:crfile ) && (match(item, a:str) >= 0)
        cal add(lines, item)
      en | cat | brea | endt
    endfo
    return lines
  end

  " a:mmode is currently ignored. In the future, we should probably do
  " something about that. the matcher behaves like "full-line".
  let cmd = "/Users/burke/matcher/matcher --limit " . a:limit . " --manifest " . ctrlp#utils#cachefile() . " "
  if ! g:ctrlp_dotfiles
    let cmd = cmd . "--no-dotfiles "
  endif
  let cmd = cmd . a:str
  return split(system(cmd))

endfunction
let g:ctrlp_match_func = { 'match': 'g:GoodMatch' }
```

# Bugs

* Probably

# Contributing

* Fork branch commit push pullrequest
