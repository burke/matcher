# Matcher

This is a standalone library that does the same fuzzy-find matching as Command-T.vim.

# Installation

```
$ make
# move `matcher` somewhere useful
```

# Usage

Matcher takes three arguments, then a list of strings to match against via stdin.

The arguments are:

1. limit: The number of matches to return
2. show_dotfiles: 1 if you want dotfiles included, 0 otherwise
3. search: the actual string the user wants to search for in the editor or wherever.

```
$ matcher <limit :: Int> <show_dotfiles :: {1 | 0}> <search :: String>
# eg.
$ find . | matcher 10 0 customer.rb
```

# Using with CtrlP.vim

```vimscript
let g:ctrlp_dotfiles = 0
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
  let cmd = "/Users/burke/matcher/matcher " . a:limit . " " . g:ctrlp_dotfiles . " " . a:str
  let input = join(a:items, "\n")
  return split(system(cmd, input))

endfunction
let g:ctrlp_match_func = { 'match': 'g:GoodMatch' }
```

# Bugs

* Probably

# Contributing

* Fork branch commit push pullrequest
