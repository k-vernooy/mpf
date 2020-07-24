# mpf
A C++ based music player with audio tag filtering.

## Dependencies
`mpf` depends on [SDL2](https://www.libsdl.org/download-2.0.php) for audio reading, playing, and GUI capabilities. Specifically, it requires:
- SDL2 (`libsdl2-dev`)
- SDL2_image (`libsdl2-image-dev`)
- SDL2_mixer (`libsdl2-mixer-dev`)

On Ubuntu/Debian:
```bash
sudo apt install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev
```
MacOs (Homebrew):
```
brew install sdl2 sdl2_image sdl2_mixer
```
In addition, both Boost.Filesystem and TagLib must be installed.


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

Binary distributions will be available once a stable version is released. This project is currently tested and compatible with Linux (Ubuntu) and macOS. Dependencies for the GUI and audio playing must be installed before compilation.


## Usage
`mpf` is designed to be a simple visual player. It is invoked from the command line, passed a list of files, and through CLI flags filters/orders them. Upon succesful completion of all songs in the passed list, it exits the GUI app.

### Filtering
Metadata tags set within the input files are used to filter the music. In `mpf`, the `--filter` option is used to select subsets of your input files. *If `mpf` has trouble reading tags present in the audio, please submit an issue with the file attached along with the command used to run `mpf`*.

Specific tag names can be used to filter music. Basic comperators are: `==`, `>`, `<`, `<=`, `>=`.
Here's an example of playing all music in a directory by the composer Bach that was composed beyond 1730:
```
mpf * --filter "COMPOSER==Bach;YEAR>1730"
```
Each filter is separated by the `;` character, and are performed in sequence. String equality is case insensitive; string quantity comparison is alphabetical.

### Ordering
By default, `mpf` orders music by the order in which it is input. For other ordering operations, the `--order` flag may be invoked. It may either be passed "shuffle", or a tag name and the order (ASCENDING [0] or DESCENDING [1]). Strings are ordered alphabetically. Example:
```
mpf * --order "YEAR;ASCENDING"
mpf * --order shuffle
```
### Other Flags
- `--loop`: Used to loop the playlist. If `--shuffle` is set, the list is reshuffled.
- `--keep`: Within filtering, keep songs where the tag to be filtered by is not set.
- `--no-gui`: Run the cli version of the app
- `--theme <theme>`: Set the [GUI skin](https://example.com)

### Configuration
Configurations are handled in two ways. They can be passed direcly to `mpf` with the `--config` option, or can be set by creating or modifying a `~/.mpf.config` file.

If you listen to the same directory of music, or use the same filters a lot, defaults or presets can be set. For these options, the properties `MUSIC_DIRECTORY`, `DEFAULT_FILTER`, and `DEFAULT_ORDER` may be set. To do so:
```
mpf --config 'MUSIC_DIRECTORY=~/Music' \
             'DEFAULT_FILTER="COMPOSER==BACH"' \   # Must escape quotes for filter string
             'DEFAULT_ORDER=shuffle'
```

or

```
~/.mpf.config
----

MUSIC_DIRECTORY=~/Music
DEFAULT_FILTER=COMPOSER==BACH
```

For the GUI, colors and other variables can be set similarly. See the man page for the full list of configurable variables.
