# alpha3

This release enables above-ground segment practice, supports PAL and JP treasures, and adds a host of new options for viewing and editing the game state.

Feature additions:
- The gz menu has been completely reorganized. Most options should be easier to find and require less menuing.
- It's now possible to warp from the world map
- All structure types now have associated edit menus and debug info. Presets also now have all structures set to appropriate states.
- GZ menu now shows available controls depending on which option you have selected
- Treasures and enemies above-ground will now respawn appropriately based on preset.
- New option: treasure region. You can change between US/PAL/JP treasures and it will take effect on the next load.
- New option: treasure editor.
    - You can move treasures around the map arbitrarily
    - While moving treasures, you can snap them to the nearest waypoint
- New option: enemy collision viewer
- New option: cave spawnpoint debug viewer
- You can now quick-retry sublevels by opening the gz menu
- Presets now set the correct day number and enter type
- Enemy debug info now shows in a more appropriate place for most enemies
- Most presets have had their cutscene flags updated to be more accurate
- 35 and 40 white presets have been added for PoD
- The squad editor now has pikmin images
- The preset menu now automatically scrolls to your currently selected preset when opened
- Reworked "boing mode" to be more useful
- Retrying sublevel now resets your poko count
- The crash landing cutscene is now skippable

Bug fixes:
- Fixed: poko editor now behaves as expected in caves
- Fixed: several small visual bugs while using the collision viewer. (The black triangles issue is still present.)
- Fixed: gates sometimes weren't receiving preset settings as expected
- Fixed: several menus could become out of sync with the actual game state
- Fixed: seed no longer shows while above-ground
- Fixed: sprays no longer un-unlock when reaching zero
- Fixed: dismiss position circles now account for pikmin working on tasks correctly
- Fixed: crashes when warping from an area with wild pikmin
- Fixed: captains no longer have 1hp when warping to the final floor of a cave from file select
- Fixed: you can no longer open the gz menu in spots where it would cause crashes or unexpected behavior to warp (loading screens, some cutscenes, etc.)
- Fixed: wild bulbmin no longer show up in presets after retrying sublevels
- Fixed: all objects now show up in freecam, not just ones that were visible initially
- Fixed: warping after retrying a sublevel with the same seed no longer continues to use the same seed

---

# alpha2

This release includes the following fixes:

- Fixed crash when enabling collision viewer while out of bounds in a sublevel with no void
- Fixed crashes related to retrying sublevels and warping
- Exploration Kit upgrades persist on retry
- Cutscene flags persist on retry

---

# alpha1

This is the first public release of P2GZ, the new Pikmin 2 practice ROM built by [@jonathonbrady](https://github.com/jonathonbrady), [@mayabyte](https://github.com/mayabyte), [@HeartPiece44](https://github.com/HeartPiece44), and [@amaritnez](https://github.com/amaritnez).

This release includes the following features:

- Warp anywhere in the game, including automatic squad presets for all current PoD splits
- Edit your pikmin squad, spray numbers, and pokos in real time
- Freecam mode with the ability to teleport anywhere in the current level
- Quick sublevel retry including options for retrying the same seed and starting from the beginning of the cave
- Edit the status of all gates in the current level, including setting their stage and remaining health
- Edit the time of day or freeze time completely for endless above-ground labbing
- Realtime collision viewer
- Realtime waypoint viewer that displays active carry paths
- Skippable save prompts between sublevels
- Skippable treasure cutscenes
- Debug info for the game's heap and enemy state
- Togglable in-game timer displayed on screen
- Realtime squad dismiss position viewer
- Exploration Kit editor
- Togglable cutscene flags

Open the GZ menu to access these features any time by double pressing Dpad Left while in game.

If you experience any crashes or unexpected behavior, please [create an issue](https://github.com/p2gz/p2gz/issues) so we can keep track of it and fix it.
