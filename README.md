# Matcher

This is a standalone library that does the same fuzzy-find matching as Command-T.vim.

# Installation

```shell
$ make
# move `matcher` somewhere useful
$ make install
# make install will install it to /usr/local/bin.
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
let g:path_to_matcher = "/path/to/matcher"

let g:ctrlp_user_command = ['.git/', 'cd %s && git ls-files . -co --exclude-standard']

let g:ctrlp_match_func = { 'match': 'GoodMatch' }

function! GoodMatch(items, str, limit, mmode, ispath, crfile, regex)

  " Create a cache file if not yet exists
  let cachefile = ctrlp#utils#cachedir().'/matcher.cache'
  if !( filereadable(cachefile) && a:items == readfile(cachefile) )
    call writefile(a:items, cachefile)
  endif
  if !filereadable(cachefile)
    return []
  endif

  " a:mmode is currently ignored. In the future, we should probably do
  " something about that. the matcher behaves like "full-line".
  let cmd = g:path_to_matcher.' --limit '.a:limit.' --manifest '.cachefile.' '
  if !( exists('g:ctrlp_dotfiles') && g:ctrlp_dotfiles )
    let cmd = cmd.'--no-dotfiles '
  endif
  let cmd = cmd.a:str

  return split(system(cmd), "\n")

endfunction
```


# Bugs

* Probably

# Contributing

* Fork branch commit push pullrequest
* I'm bad at github notifications. Send me an email too at burke@burkelibbey.org
