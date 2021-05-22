[![Pharo-CI](https://github.com/ycorre/molecular/actions/workflows/molecular-pharo.yml/badge.svg)](https://github.com/ycorre/molecular/actions/workflows/molecular-pharo.yml)

**Molecular** is a small *R-Type* like game coded in C++ and in [Pharo](https://pharo.org).

## How to play

The game controls are the keyboard and the mouse: the mouse is used to move around and to shoot (left button), keyboard is used to move in the menus (arrows to move and space to validate) and, during the game, to change weapon (the keys are c, v and b). Escape is used to go back to the menu from the game and to quit the game if pressed from the main menu.

### Explanation of the HUD

![Molecular Screenshot](http://ycorre.github.io/molecular/images/screenshots/molecular_legend.png)

  1. Health bar
  2. Quarks bars. Quarks are the bonuses given by the enemies once destroyed. Quarks are used to activate weapons and determine their power. There are six different kinds of quarks: u, d, c, s, t, b (for up, down, charm, strange, top \& bottom).
  3. These are the activated weapons:
   1. the electrons (c), the basic weapon,
   2. the hadron (v) which can be loaded for more power (hold the mouse left-button down); the load level can be monitored with the VU meter,
   3. the baryon (b) which is a laser beam,
   4. and the plasma which is yet to be implemented.
  4. Power level indicator for electrons and baryon.
  5. VU meter showing the load level of the hadron.
  6. The score
  7. Remaining lives
  8. A bonus (Type-U quark)

## License

The *Molecular* C++ source code is under [CC0 Public Domain](https://creativecommons.org/publicdomain/zero/1.0/), except for the JSON parser library [JsonCpp](https://github.com/open-source-parsers/jsoncpp) which is under a dual Public Domain/MIT License. The Pharo source code is under [MIT license](https://opensource.org/licenses/MIT).

The *Molecular* artworks (graphics, sounds and music - everything in the /res and /sound folders and their subfolders) are under [CC BY-NC 4.0](http://creativecommons.org/licenses/by-nc/4.0/) and are the property of [Dimitrix](https://www.youtube.com/user/dimitrix9bit/videos).
