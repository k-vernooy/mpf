# mpf
A C++ based GUI interface client for MPD with ID3 tag filtering.

## Dependencies
As this is a client for the [MPD](musicpd.org) backend, it must first be installed. This can be done from [source](https://www.musicpd.org/doc/html/user.html#compiling-from-source) or in one of the below methods:
```
sudo apt install mpd
```
With Homebrew (MacOS)
```
brew install mpd
```

In addition to MPD, several C++ libraries must be installed.
- SDL2/SDL2main
- Boost.Filesystem


## Installation

To compile from source:
```
git clone 'https://github.com/k-vernooy/mpf'
make
make install
```

To install via Homebrew:
```
brew tap k-vernooy/tap
brew install mpf
```

This project is currently tested and compatible with Linux (Ubuntu) and macOS. Dependencies for the GUI and audio playing must be installed before compilation.


## Usage
`mpf` is designed to be a simple visual player. It is invoked from the command line, passed a list of files, and through CLI flags filters/orders them. Upon succesful completion of all songs in the passed list, it exits the GUI app.

### Filtering
ID3v2 tags set within the input files are used to filter the music. `mpf` does not contain tag editing functionality - for that, install a dedicated program such as `id3v2`. In `mpf`, the `--filter` option is used to select subsets of your input files.

Any ID3v2 frame can be used to filter music. Basic comperators are: `==`, `>`, `<`, `<=`, `>=`.
Here's an example of playing all music in a directory by the composer Bach that was composed beyond 1730:
```
mpf * --filter "COMPOSER==Bach;YEAR>1730"
```
Filters are performed sequentially; i.e. the above is NOT equal to
```
mpf * --filter "YEAR>1730;COMPOSER==Bach"
```
String equality is case insensitive; string quantity comparison is alphabetical.

### Ordering
By default, `mpf` orders music by the order in which it is input. For other ordering operations, the `--order` flag may be invoked. It may either be passed "shuffle", or an ID3v2 frame and along with the order (ASCENDING [0] or DESCENDING [1]). Strings are ordered alphabetically. Example:
```
mpf * --order "YEAR;1"
mpf * --order shuffle
```

### Configuration
Configurations are handled in two ways. They can be passed direcly to `mpf` with the `--config` option, or can be set by creating or modifying a `~/.mpf.config` file.

If you listen to the same directory of music, or use the same filters a lot, defaults can be set. For these options, the properties `MUSIC_DIRECTORY`, `DEFAULT_FILTER`, and `DEFAULT_ORDER` may be set. To do so:
```
mpf --config 'MUSIC_DIRECTORY=~/Music' \
                      'DEFAULT_FILTER="COMPOSER==BACH"'   # Must escape quotes for filter string
                      'DEFAULT_ORDER=shuffle'
```

or

```
~/.mpf.config
----

MUSIC_DIRECTORY=~/Music
DEFAULT_FILTER=COMPOSER==BACH
```

For the GUI, colors and other variables can be set similarly. See the man page for the full list of settable variables.
