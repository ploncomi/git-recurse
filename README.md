# git-recurse
Tool for showing repositories recursively<p/>

It works similar to [vcs](https://github.com/dirk-thomas/vcstool), but git-recurse shows, for all the subfolders, all of the branches which are equivalent to the current ones. It can also fetch the subfolders if requested.

Requires at least c++17 to compile.

In linux compile as:
`g++ git-recurse.cpp --std=c++17 -o git-recurse`
