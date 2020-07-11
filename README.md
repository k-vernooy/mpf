# music-filter
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
git clone 'https://github.com/k-vernooy/music-filter'
make
make install
```

To install via Homebrew:
```
brew tap k-vernooy/tap
brew install music-filter
```

This project is currently tested and compatible with Linux (Ubuntu) and macOS. Dependencies for the GUI and audio playing must be installed before compilation.


## Usage
The point of `music-filter` is to be a very simple and visual player. It is invoked from the command line, passed a list of files, and through CLI flags filters/orders them. Upon succesful completion of all songs in the passed list, it exits the gui app with code 1.

### Filtering
ID3v2 tags set within the input files are used to filter the music. `music-filter` does not contain tag editing functionality - for that, install a dedicated program such as `id3v2`. In `music-filter`, the `--filter` option is used to select subsets of your input files.

Any ID3v2 frame can be used to filter music. Basic comperators are: `==`, `>`, `<`, `<=`, `>=`.
Here's an example of playing all music in a directory by the composer Bach that was composed beyond 1730:
```
music-filter * --filter "COMPOSER==Bach;YEAR>1730"
```
Filters are performed sequentially; i.e. the above is NOT equal to
```
music-filter * --filter "YEAR>1730;COMPOSER==Bach"
```
String equality is case insensitive; string quantity comparison is alphabetical.

### Ordering
By default, `music-player` orders music by the order in which it is input. For other ordering operations, the `--order` flag may be invoked. It may either be passed "shuffle", or an ID3v2 frame and along with the order (ASCENDING [0] or DESCENDING [1]). Strings are ordered alphabetically. Example:
```
music-filter * --order "YEAR;1"
music-filter * --order shuffle
```

### Configuration
Configurations are handled in two ways. They can be passed direcly to music-filter with the `--config` option, or can be set by creating or modifying a `~/.music-filter/config` file.

If you listen to the same directory of music, or use the same filters a lot, defaults can be set. For these options, the properties `MUSIC_DIRECTORY`, `DEFAULT_FILTER`, and `DEFAULT_ORDER` may be set. To do so:
```
music-filter --config 'MUSIC_DIRECTORY=~/Music' \
                      'DEFAULT_FILTER="COMPOSER==BACH"'   # Must escape quotes for filter string
                      'DEFAULT_ORDER=shuffle'
```

or

```
~/.music-filter.config
```

For the GUI, colors and other variables can be set similarly. See the man page for the full list of settable variables.
