# Dungeon

Copyright © New Desire Studios 2022 - 2023

## 1. Project Introduction

Years ago, I quite like mobile game Soul Knight, and hoped to make a similar one. Recently, I happened to learn some basic design patterns, so I wish this could be my first practice. Before, I did make some games, but they were all small, and poor management can do the job. So this time, I'd also want to create a simple frame for game development, that is to say, a game engine, which could provide more flexibility and expansibility.

More information can be found in my personal website: [Tony's Studio - Dungeon](http://tonys-studio.top/project/dungeon.html)。

## 2. Building Dungeon

There are three projects in this Visual Studio solution: Dungeon, Dungine and TinyXML2. [TinyXML2](https://github.com/leethomason/tinyxml2) is a great XML parser so I use this to pack it into a static library. And Dungine is the so callled game engine. Just simply build the solution is OK. Dungeon is the start up project, and Release configuration will output the executable to `Publish\` directory, while Debug configuration will output to `Build\dist\Debug\`. All macros for debug are disabled.

### Build Environment

- Windows 11 Pro
- Visual Studio 2022 Community
- [EasyX 20220901](https://easyx.cn/)
- [FMOD 0.2.2.7](https://fmod.com)

Notice that in EasyX 20220901, prefix for message changed from `EM` to `EX`, which was not compatible with previous versions.

## 3. How Does Dungeon Work

The game contains the game engine Dungine (Dungeon Engine) and the game Dungeon itself. It is developed with [EasyX](https://easyx.cn/) for graphics and [FMOD](https://fmod.com) for audio. Also, tinyxml2 as mentioned before.

### 3.1 Dungine

This is the game engine, including basic classes, encapsulations and a simple UI library. It was build with ECS pattern. 

#### 3.1.1 Game Object

The core of a game frame is to manage all objects. For common objects in game, including characters, weapons, bullets, etc., I used Factory Pattern and Prototype Pattern to create them, and Component Pattern to add properties behaviors.

#### 3.1.2 Scene

Scene manages all game objects. It maintains a object pool, just a pool, no reuse things. One important thing is that adding or removing objects during scene update should be postponed till update finishes.

#### 3.1.3 Graphics and Audio

For graphics, the one and only thing to render is a `Symbol`, it includes position, layer, rotation angle, scale and alpha info, and is rendered in order of layer and position.

For audio, I used FMOD, and there are two types of sound. Effect, e.g. click and fire. The other is background music.

#### 3.1.4 Resource Management

Well, the resource management is not advanced, I made it up without reference.

When the program starts, it reads all reference of resource in a XML file, and only load it when required. Then, like smart pointer, it will release it if reference count is zero. There are image, audio, and motion resource. Motion resource is sprite sheet for animation.

#### 3.1.5 UI

In version 0.1.0, I was inspired by [EasyUI](https://codebus.cn/contributor/zhurong-easyui). Now, with more consideration, I refactored that, and added more fancy features. However, the basic thing is still, a widget contains drawer and trigger.

All UI widgets are preferred to be loaded from external XML file, including both absolute and relative positioning, and can also adapt to screen size automatically. Besides, there are some animation effects available, like translation, scaling and alpha change. In version 1.1.0, I introduced animation drawer to support animation widget.

There is one problem that widgets with animation drawer could not be assigned scaling animation, since animation drawer scales itself to adapt to screen size.

Interface is the carrier of all events, and is managed by Application, the largest class of all, and the entrance of the program.

#### 3.1.6 Trivia

Apart from primary objectives, it provides some other convenient tools, such as XML parse with TinyXML2, some vector calculations, [Quadruple Tree](https://codebus.cn/sunxiaoyu/quadtree-collision), etc. And some base class for design pattern. Here, Factory Pattern is combined with Prototype to be a Library, so, just get what you want directly from a library.

### 3.2 Dungeon

Well, this part is the actual game logic based on Dungine. The main stuff it does is to concrete game procedure, terrain, objects, and interfaces.

#### 3.2.1 Procedure

It is the whole process of the game, derived from `Scene`. It initialize resources, call terrain generation, and manage some special objects with special behaviors.

#### 3.2.2 Terrain

Terrain is one of the most complicated part of the game. It use random Prim algorithm to generate a maze in a 3 * 3 graph. Then it implements the real map based on this plain graph, turning nodes into Arenas, and edges into Bridges.

Besides, it provides A* algorithm for path finding, and a blank area finding algorihtm.

#### 3.2.3 Property and Behavior

All these are added by component. For example, there are `RigidBodyComponent`, `ColliderBoxComponent` for collision, `MoveComponent` for movement, and `AnimComponent` for rendering. For interaction and 'AI', there is `BehaviorComponent`, which really reduced dependency of objects.

#### 3.2.4 Interface

Interface is where every event takes place, and each of them is an specific class. Interface layout is provided by XML file, but events has to be hooked by hard coding.

#### 3.2.5 Flexibility

Most of the game data are loaded from external XML file, so you can easily modify the game without re-compile it. For example, change character properties, add new image or audio, even new character and weapon!

## 4. Epilogue

Almost all image were drawn by myself, some of them were collected from the internet, but made my own modification. In-game sound track comes from the game Undertale, the other were created by my mother.

It is, a reflection of my inner self, so it was never balanced, some weapons are extremely powerful, while others may be a little... weak.

Thanks for your watching! :smiley:

