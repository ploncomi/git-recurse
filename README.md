# git-recurse
Tool for showing repositories recursively<p/>

It works similar to [vcs](https://github.com/dirk-thomas/vcstool), but git-recurse shows, for all the subfolders, all of the branches which are equivalent to the current ones. It can also fetch or pull the subfolders if requested.

Requires at least c++17 to compile.

In linux compile as:
`g++ git-recurse.cpp --std=c++17 -o git-recurse`

Use:
`./git-recurse [command]`

Command is optional and, if provided, it is executed in each subfolder containing a repository.
As an example, use this command for doing fetch in all subfolders:

`./git-recurse fetch`
