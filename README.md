# Rotation

![](http://i.imgur.com/dwNGIPm.png "Demo picture of Rotation running on Cygwin64")

##### Abstract:

Rotate the board around and try to use gravity to get your character to the next level. Use the falling obstacles to help you progress!

##### Description:

This is a project made for an extra credit undergraduate course of IMGD 4000, made as an independent study group. 

The goal of the game is to rotate the board clockwise or counter-clockwise, so that the pieces in the board is affected by gravity and fall accordingly. You, as the '@' character, needs to escape through the levels by going to the '%' door to progress.

##### Controls:

The game uses the computer keyboard to control.

---

| Keys | Actions |
| --- | --- |
| Space | Confirm/Enter |
| Q | Rotate Left |
| W | Start Game |
| E | Rotate Right |
| R | Quit to Main Menu |
| A | Move Character Left | 
| S | Quit Game |
| D | Move Character Right |
| P | Pause Game |
| CTRL+C | Force Quit |

---

| Known Issues | Description |
| --- | --- |
| **The game segfaults after progressing through a few levels.** | So far, I have no clues or leads as to what causes the segfaults. I can only say that it requires more strenous debugging in the game source codes, as well as the engine source codes in order to find the culprit. |
| **Force quitting the game causes segfaults.** | This is the same issue that applies to the issue mentioned above. Again, either the game source codes or the engine source codes need to be strenously debugged for any bugs. |
| **Segfaults cannot be guaranteed to be resolved.** | Since I do not own the source code for the Dragonfly engine, I cannot guarantee the segfaults are going to be solved. |

---

This work is Apache Licensed. Dragonfly engine code is copyrighted by Mark Claypool.