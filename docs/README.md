# How to assemble your Quad Creek kit/PCB

![PCB_Step_0](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_0.jpg)

Welcome! Here I'll walk you through the basic steps of how to solder a Quad Creek. This is NOT a beginner project, you don't have to be a seasoned veteran, but if this is your first time soldering, stop. Here I'll go over my general way of soldering, you don't have to do it exactly how I do, but this way is most efficient and easy for me. Solder with care. If you just got the PCB and not the DIY kit, refer to the BOM at the bottom of this page.

## Resistors & Diodes
![PCB_Step_1](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_1.jpg)
Solder these from the top. Make sure the diode band is on the same side as the silkscreen image, and make sure you put the BAT46s on the diodes with the S curve, and the 1N914s on the diodes with the straight band. If you're unsure of capacitor color codes, look it up. Lastly, make sure everything sits flush, you'll thank yourself later.

## Capacitors
![PCB_Step_2](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_2.jpg)
Also solder these from the top. Make sure the numbers match.

## IC Sockets, Transistors, & More
![PCB_Step_3](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_3.jpg)
To solder the IC sockets, insert, and then fold two opposite corner pins to secure the socket in place, and solder from the bottom. Ensure the notches are aligned, so no ICs are unintentionally placed upside-down. I solder the transistors from the bottom. You can do them from the top, but it's harder that way. Do not solder the voltage regulator backwards; very bad, high-temperature things happen when you do. Don't forget to do the jumpers either, you can use scrap leads to wire them as pictured.

## Power, Aux Jacks, & Other Bottom Components
![PCB_Step_4](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_4.jpg)
To solder the stereo aux jacks, I solder one pin to begin, then gently push the body of the jack to be flush, while the initial solder joint is being heated. Repeat for a second pin to anchor it, then solder the rest normally. This process is also used for the 2.1mm power jack. For the nano board, I socket the male pins into the female sockets, insert them into the PCB, place the nano board on top, solder the male pins to it, and then solder the female sockets to the board. This ensures the board can easily be inserted and removed from the board. Also if you bought a DIY kit the nano will come pre programmed, so you don't need to do anything there, though feel free to reprogram it if you'd like.

## Potentiometers
![PCB_Step_5](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_5.jpg)
Soldering the poteniometers is a similar process to the aux. What I do is insert the potentiometers, solder just one pin on each pot, then when they've all had one joint soldered, I flip the board over and solder all 96 potentiometer pins from the bottom side. Do not solder the anchoring legs on the side until you're sure they're aligned enough to fit on the panel. You also don't need to solder them at all if you plan on screwing every potentiometer nut into the front panel.

## Final Steps


![PCB_Step_6](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_6.jpg)
![PCB_Step_7](https://raw.github.com/uvknhn/quad-creek/main/docs/img/PCB_Step_7.jpg)

Once all your soldering is complete, test your quad creek. Turn the 5 mixer channels all the way counterclockwise, turn the 4 knobs at the tops of the creeks up the same, set the ones directly below those to about halfway, and see if there is sound coming out of the three outputs. If that works, I'd screw in all the banana jacks, the nylon standoffs, and do further testing with some banana plugs. Then once you have tested/feel confident it works, install the panel and enjoy!
A note on installing the banana jacks: Be wary not to overtighten. When tightening, hold the jack in place by applying downward pressure to the top of the jack, rather than holding from the sides. This prevents the plastic part at the top from being threaded and stripped out.

## BILL OF MATERIALS

### RESISTORS

| Value | Qt. |
| :-- | --: |
| 1M | 6 |
| 100k | 56 |
| 10k | 35 |
| 1k | 18 |
| 2.2M | 1 |
| 220k | 17 |
| 22k | 12 |
| 2.2k | 2 |
| 47k | 6 |
| 4.7k | 1 |

### CAPACITORS

| Value | Code | Type | Qt. |
| :-- | :--: | :--: | --: |
| 220uF | 227 | Electr. | 1 |
| 10uF | 106 | MLCC | 7 |
| 4.7uF | 475 | MLCC | 2 |
| 1uF | 105 | MLCC | 16 |
| 100nF | 104 | MLCC | 32 |
| 10nF | 103 | MLCC | 10 |
| 1nF | 102 | MLCC | 12 |
| 100pF | 101 | MLCC | 1 |

### ICs

| Chip | Qt. |
| :-- | --: |
| TL082 | 6 |
| LM358 | 7 |
| CD4013 | 2 |
| L7809CV | 1 |
| ATMEGA328P Nano Board | 1 |

### TRANSISTORS

| Transistor | Qt. |
| :-- | --: |
| 2N3904 | 10 |
| BC548 | 1 |

### DIODES

| Diode | Qt. |
| :-- | --: |
| 1N914 | 25 |
| BAT46 | 16 |
| 1N4001 | 1 |

### I/O

| Part | Code | Qt. |
| :-- | :--: | --: |
| 4MM Banana | [Generic/Unknown](https://www.aliexpress.us/item/3256807503214936.html) | 32 |
| Potentiometer | RD901F-40-15K-B100K | 32 |
| Stereo Jack | PJ-324 | 5 |
| 2.1mm Jack | A-4118 | 1 |
| DIP Switch | DI-R02 | 2 |

### SOCKETS

| Type | Qt. |
| :-- | --: |
| 15 Pin Socket | 2 |
| 14 Pin DIP Socket | 2 |
| 8 Pin DIP Socket | 13 |

### Knobs
| Part | Code | Qt. |
| :-- | :--: | --: |
| Black Knob | [A-6351](https://www.taydaelectronics.com/black-control-knob-9-8x11mm-shaft-hole-6x18t.html)/["Stubby"](https://www.thonk.co.uk/shop/stubby-knobs-t18/) | 9 |
| Blue Knob | [A-6352](https://www.taydaelectronics.com/blue-control-knob-9-8x11mm-shaft-hole-6x18t.html) | 7 |
| Green Knob | [A-6353](https://www.taydaelectronics.com/green-control-knob-9-8-x-11mm-shaft-hole-6x18t.html) | 9 |
| Purple Knob | [A-5666](https://www.taydaelectronics.com/purple-control-knob-9-8-x-11mm-shaft-hole-6x18t-5666.html) | 7 |

### Mechanical
| Type | Qt. |
| :-- | --: |
| M3×15mm Nylon Standoff | 19 |
| M3×4mm Screw | 19 |
| M3×10mm Screw | 19 |
