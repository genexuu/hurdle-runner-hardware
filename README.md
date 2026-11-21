# Hurdle Runner

A breadboard arcade game built for ECE 233, Module 3. Push the joystick left or right to move the runner, then press the thumbstick to jump a hurdle. A collision ends the run and the 16 × 2 LCD shows the score.

![Blender animation showing the jumper wires, LCD, joystick, buzzer and controller lifting off the breadboard and returning to their positions](media/preview.gif)

[Watch the 18-second animation in 720p](media/assembly.mp4) · [Download the editable Blender scene](hurdle-runner.blend) · [View the exploded assembly](media/exploded.jpg)

## Assembly sequence

| Time | What to look for |
| --- | --- |
| 0–3.7 s | The camera turns toward the assembled board as the jumper wires lift clear. |
| 3.7–8 s | The LCD tilts up and the joystick cap lifts to reveal its shaft. |
| 8–11.6 s | The buzzer, resistor and controller separate from the breadboard assembly. |
| 11.6–18 s | The camera orbits the exploded assembly. The modules return to position and the jumpers reconnect. |

To explore the model, open the scene in Blender 4.5.3 LTS and press **Space**. The timeline markers identify each step across frames 1–360 at 20 fps. In the Outliner, expand a numbered collection and select its parent object named **motion** to reposition the module. Select an individual part to edit its geometry or material. **Numpad 0** toggles the camera view. The exported video adds English captions and labels that track moving parts.

## Hardware

![Original breadboard prototype with the buzzer on the left, the LCD on the right and the controller behind them](media/prototype.jpg)

The course report and demonstration footage provided the reference for the Blender model. Board geometry, jumper routing and separation distances are simplified for visibility. Use the original circuit diagram and board documentation when wiring the hardware.

| Component | Quantity | Function and connections |
| --- | --- | --- |
| TI microcontroller development board | 1 | Reads the controls and drives the peripherals. The prototype photo shows the Texas Instruments logo. The exact board model is unconfirmed. |
| Analog joystick with push switch | 1 | The sketch reads the horizontal axis on `A8`. The report labels that connection `P4_4`. Pressing the switch on `P4_7` triggers a falling-edge interrupt using the internal pull-up. |
| 1602A character LCD | 1 | Displays the runner and hurdles as custom glyphs. The two rows represent the ground and the space above it. |
| Buzzer | 1 | The standalone music sketch plays *Jingle Bells* using `tone()` on `P3_5`. |
| Solderless breadboard | 1 | Holds the peripherals and provides connection points. |
| Fixed resistor | At least 1 | One is visible beside the buzzer. Its resistance is unconfirmed. |
| LCD contrast potentiometer | 1 in the schematic | Adjusts LCD contrast. Its presence on the assembled board and its resistance are unconfirmed. |
| Jumper wires | Multiple | Connect the modules. The photos do not establish an exact count. |

The LCD sketch passes these six signals to the `LiquidCrystal` constructor, in this order.

| Signal | RS | E | D4 | D5 | D6 | D7 |
| --- | --- | --- | --- | --- | --- | --- |
| Controller pin | P6_7 | P2_3 | P2_6 | P2_4 | P5_6 | P6_6 |

## Firmware

**Development environment:** Energia IDE for TI microcontrollers. The exact development board model remains unconfirmed. [TI Energia overview](https://www.ti.com/tool/ENERGIA)

| Original sketch | Behavior |
| --- | --- |
| [PlayerAction.ino](firmware/PlayerAction/PlayerAction.ino) | Reads the joystick with `analogRead(A8)`. Values below 250 request a step back and values above 800 request a step forward. A button interrupt requests a jump. A 250 ms task advances the state machine and updates the runner's row and column. |
| [RefreshScreen.ino](firmware/RefreshScreen/RefreshScreen.ino) | Draws the runner and two hurdles. The hurdles start in different columns, move left and wrap around. Each collision-free update adds a point. A collision displays `Game Over!` and the score. |
| [Music_JingleBells.ino](firmware/Music_JingleBells/Music_JingleBells.ino) | Pairs each note with its duration and plays the melody using `tone()`, a delay and `noTone()`. It has its own `setup()` and `loop()`. |

The repository preserves the three original sketches. The player and display modules share game state and use `OneMsTaskTimer`. The display also uses `LiquidCrystal`. Building the game requires a board environment that defines the TI pin aliases above. The supplied files contain separate entry points for the display and music sketches, with no common entry point that schedules all three modules. An integrated firmware build and hardware test remain unverified.
