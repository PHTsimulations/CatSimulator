# Microsoft Cat Simulator — NumWorks NWA Alpha

Native EADK external app prototype (C++), for NumWorks Epsilon calculators. It uses the public EADK API, 320x240 screen and key scanning.

## Build through GitHub Actions (no local toolchain)
1. Create a GitHub repository (e.g. `microsoft-cat-simulator-nwa`).
2. Upload all files/folders from this project, including `.github/workflows/build.yml` (GitHub may hide dotfiles in upload dialogs; create workflow file via Add file if needed).
3. Commit. Open **Actions** and wait for `Build NumWorks NWA` to finish green.
4. Open the workflow run, scroll to **Artifacts**, download `Microsoft-Cat-Simulator-NWA.zip`, extract `CatSimulator.nwa`.
5. Use NumWorks official app uploader (`my.numworks.com/apps`) and install the downloaded NWA.

## Controls
- Arrow keys: move around the tile map
- OK / EXE: interact / confirm
- Back: pause menu / return
- Pause menu: arrows to choose, OK to select

## Alpha prototype systems
- tile neighborhood map, player cat sprite, movement
- cat status panel: energy, hunger, fun, comfort, bond
- nap, play, exploration/area change, weather cycling, photo-mode stub
- status values tick while exploring

This is a small calculator-native game; it intentionally uses low-resolution shapes and text rather than external assets.

## Important
The GitHub build is a compilation workflow; hardware testing is still required. External apps are hidden in exam mode and are removed by calculator reset, according to NumWorks Epsilon documentation.
