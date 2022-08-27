# Dungeon
A roguelike game as a practice of design patterns.
## Overview
Dungeon is a roguelike game which provides various ranom elements. You can act as two characteristic heroes to go through the dagerous dungeon full of monsters. There are two chapters and each chapter includes five levels. Each chapter has its distinctive style and unique monsters, and a Boss at the end of it. You may start off weak, but as you move on, you could find powerful weapons in the misterious crystals.
## Build Environment
It is developed with [EasyX](https://easyx.cn/) and [FMOD](https://fmod.com/). To compile it, EasyX library is required. Just build the entire solution, and run.
- Windows 11 Pro
- Visual Studio 2022 Community
- EasyX 20220610
- FMOD 0.2.2.7
## Dungine
For this game, I created a clumsy 2D game engine - Dungine (Dungeon Engine). It provides the basic classes needed in a game, such as GameObject, Scene, resource manager and so on. Besides, it also realised a tiny UI library which support external XML style and simple dynamic effects. Although it is specific to this game, it is still general enough for other games.
## Details
In this project, I practiced some commonly used design patterns. Most importantly, I used prototype pattern and component pattern for all game objects. Everything in the game (UI not included) use components to update, and can clone another object instantly. Quite like that in Unity, there are some basic components like ColliderBoxComponent, RigidBodyComponent, BehaviorComponent. For behaviors, each behavior is a independent class, which allows more flexibility.
