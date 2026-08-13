# KiraCheats GTASA ARM64

Experimental all-in-one AML menu for GTA San Andreas Android ARM64.

## What v1 tests

- AML native mod loading
- Embedded DEX injection through AML
- A floating **K** menu button
- Game cheat input via Android key events
- Player, wanted-level, weather, time, and traffic options
- Map reminder for the installed 100% save

## Build

Push these files to the `main` branch. GitHub Actions builds:

`KiraCheats-arm64-v1/libKiraCheats.so`

## Install

Put the built `.so` in:

`Android/data/com.rockstargames.gtasa/mods/libKiraCheats.so`

Then launch the game. A purple **K** button should appear.

## First test

Try **Health + armor + money**. If the game accepts the synthetic key sequence,
CJ's money/health should change. If the menu appears but the cheat has no effect,
the UI/DEX layer is working and only the input bridge needs a different hook.
