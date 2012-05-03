# Matcher

This is a standalone library that does the same fuzzy-find matching as Command-T.vim.

# Installation

    $ make
    # move `matcher` somewhere useful

# Usage

Matcher takes three arguments, then a list of strings to match against via stdin.

The arguments are:

1. limit: The number of matches to return
2. show_dotfiles: 1 if you want dotfiles included, 0 otherwise
3. search: the actual string the user wants to search for in the editor or wherever.

    $ matcher <limit :: Int> <show_dotfiles :: {1 | 0}> <search :: String>
    # eg.
    $ find . | matcher 10 0 customer.rb

# Bugs

* Probably

# Contributing

* Fork branch commit push pullrequest
