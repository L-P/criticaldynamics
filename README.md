# Critical Dynamics for Half-Life
Critical Dynamics is a modification of the Half-Life game and its GoldSrc engine.  
Is is a short single-player campaign designed to be as vanilla as possible
while still profiting from 25 years of Moore-abiding hardware.  

It served as a personal testbed for learning the [TrenchBroom][1] editor and
experimenting with [WASM][2].

**The Half-Life 25th anniversary version is required to play this mod.**

## Building on Linux
1. Clone this repository (requires LFS support) and `$ git submodule update --init`
2. Build or install [SDHLT][3] and put its binaries in your `$PATH`  
   (Any VHLT fork _should_ work but you'll have to edit [`bin/buildmap`](./bin/buildmap))
3. Build or install [Goldutil][4] and put it in your `$PATH`
4. `$ sudo apt install tup jq gdb build-essential`
5. Install [clang ≥ 18][5], this is necessary to build and link for WASM
7. `$ make`
8. If you cloned the repository inside your `steamapps/common/Half-Life`
   directory you should now be able to run `bin/run` to launch the game inside
   GDB or without a debugger attached through Steam.

## Building on Windows/Mac
1. Install a Debian-based distribution
2. See the [Building on linux](#building-on-linux) section

## Acknowledgments
- [The Whole Half-Life][6] community for being an inclusive and welcoming bunch
  and its Wiki for being the best GoldSrc resource around
- [TrenchBroom][1] for being a _relief_ to work with when you've had the
  displeasure to work with Worldcraft/Hammer and their derivatives
- [Klemss][7] for playtesting
- Not Valve, where's the promised 25th anniversary SDK Valve? Huh? Just
  open-source the engine and be done with it. I'm not asking for HL3 here.

### Development OST
- [Live on KEXP][8], notably:
  - [Gaye Su Akyol][14]
  - [Afrika Baby Bam][15]
  - [Subatomic Sound System][16]
  - [Las Áñez][17]
- [Tsumi's mixes][9] and out of them:
  - [Eric Sharp - Love the Rain][12]
  - [Crackazat - Coffee time][13]
- [The Comedy Lottery][10]
- [Tinzo's and Jojo's mixes][11]

[1]: https://github.com/TrenchBroom/TrenchBroom
[2]: https://github.com/l-p/halflife-wasm
[3]: https://github.com/seedee/SDHLT
[4]: https://github.com/l-p/goldutil
[5]: https://releases.llvm.org
[6]: https://twhl.info/
[7]: https://github.com/klemss
[8]: https://www.youtube.com/playlist?list=PLUh4W61bt_K6tflBpjWgnXLpyuu6EbNTW
[9]: https://soundcloud.com/tsumi_chan
[10]: https://www.youtube.com/@TheComedyLottery
[11]: https://www.youtube.com/@bookclubradio
[12]: https://soundcloud.com/kitsunemusique/eric-sharp-love-the-rain-kitsune-musique
[13]: https://soundcloud.com/houseofeureka/crackazat-coffee-time
[14]: https://www.youtube.com/watch?v=BT27ZQdEEjk
[15]: https://www.youtube.com/watch?v=KsL8nXfIQLg
[16]: https://www.youtube.com/watch?v=65i1PM8xRqo
[17]: https://www.youtube.com/watch?v=Mz5cLumVK-Y
