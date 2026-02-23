# AudioScraping

**Audio content scraping from open sources**  
A tool for automatically downloading audio files (music, podcasts, sound effects, speech recordings, etc.) from various websites.

**Project status**: very early stage (initial commit)

## Purpose of the project

- Collecting training datasets for AI (voice, music, sound effects)
- Creating personal archives of podcasts / lectures / audiobooks
- Automating bulk downloading of sound effects / loops / samples
- Researching / monitoring audio content across the internet

## Current capabilities (as of February 2026)

- Only basic project structure exists so far
- Written in C++ (Visual Studio solution)
- Planning support for multiple audio sources

Planned / possible sources (feel free to add your own):

- Free Music Archive / Creative Commons websites
- YouTube → audio track extraction (youtube-dl / yt-dlp style)
- SoundCloud (public tracks)
- Freesound.org
- Podcast RSS feeds
- Niche forums and old audio archives

## Technologies

- **Language**: C++ (C++17 / C++20)
- **Build environment**: Visual Studio (`.sln` file)
- **Planned libraries**:
  - libcurl or cpp-httplib — HTTP requests
  - nlohmann/json — API handling
  - taglib — reading/writing audio metadata
  - possibly ffmpeg (via API or process call) — format conversion / audio extraction

## Build & Setup

No proper instructions yet (repository just created).

Rough plan:

```bash
# 1. Clone
git clone https://github.com/MaximusPro/AudioScraping.git
cd AudioScraping
```
# 2. Open in Visual Studio
AudioScraping.sln

# 3. Build (Release / Debug × x64)
(CMake support for cross-platform builds is planned later)
## Usage (future documentation)
```bash
# Desired future interface examples (not implemented yet)
audioscrape -client_id 2266286247012 -link "https://soundcloud.com/eatbrain"
```
## TODO
- Connect at least one working source (e.g. Freesound API)
- Implement basic CLI interface (argc/argv or library)
- Add proper error handling and logging
- Save metadata (title, artist, tags, etc.)
- Support output folder and format selection
- Consider multithreading + rate limiting
- Create proper .gitignore for Visual Studio + C++

# License
MIT (for now — feel free to change)
# Author
MaximusPro
Contributions, ideas, feature requests — welcome in issues / pull requests!
