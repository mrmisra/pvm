# pvm - Simple Version Control Tool

## Overview
pvm is a minimal, C-based version control system inspired by Git. It supports basic repository operations and is designed to be portable across Linux (POSIX) and Windows systems.

## Features
- `init` — Initialize a new repository
- `save` — Stage files for commit (supports recursion and .pvmginore)
- `commit` — Commit staged files (with compression)
- `branch` — List or create branches
- `history` — Show commit history
- `index --git` — Import index, log, and branches from an existing Git repo

## Usage

### Initialize a repository
```
pvm init
```

### Stage files
```
pvm save file1.txt file2.c
pvm save .   # Recursively stage all files (respects .pvmginore)
```

### Commit staged files
```
pvm commit -m "Commit message"
```

### List or create branches
```
pvm branch           # List branches
pvm branch newbranch # Create a new branch
```

### Show commit history
```
pvm history
```

### Import from Git
```
pvm index --git
```

## .pvmginore
List files or directories to ignore (one per line). Example:
```
*.o
build/
secret.txt
```

## Building

### Linux (POSIX)
- Requires: gcc, make, zlib, git
- Build:
```
make
```

### Windows
- Requires: MinGW or MSVC, zlib, git
- Build with MinGW:
```
mingw32-make
```

## Notes
- Commits are compressed with zlib.
- `pvm` is not compatible with GitHub or the Git protocol.
- For Windows compatibility, some features may require further adaptation.

## License
MIT
