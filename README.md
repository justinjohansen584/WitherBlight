# WitherBlight
Quick Overview:
- Wither Blight is a 3rd-person Rogue-Lite that will be heavily influenced by Boomer Shooter games' movement and combat mechanics.
- In run upgrades will come in two different ways, the first being with experience-based upgrades, and the other will be finding items scattered around the level. However, the player will not be able to select upgrades or get items until a level is complete. This will hopefully prevent the player's flow from being constantly interrupted by choices and make it easier to create builds.
- The world is going to be the Ammo system, slowly gather "blocks" around you, or destroy your surroundings for much more ammo.
- The Art style is going to be low-poly, where everything is going to be made up of simple shapes (Squares and Triangles). Fractule Computer Bismuth is what I have been calling it in my head.

 Code:
 This project was created from the 3rd-person Unreal example project.
 Examples of C++ 
 - WitherBlight\Source\WitherBlight\Variant_Combat
   - This contains the majority of the current files. Below are some files and what they do.
         - CombatCharacter is the main player character.
         - Gameplay\Component contains the components for Ammo or "Block" collection, part of the Inventory system, the Ability System Component, or ASC, and the Movement component
         - Gameplay\Environment contains the files for how Chest spawning and looting. and how some of the environment will work.
 - WitherBlight\Plugins\CAD_MenuSystem\Source\CAD_MenuSystem\CAD_MenuSystem.Build.cs
   - Plugin for creating a Main menu and an esc menu.
        - Most functionality is implemented in Blueprint for easier prototyping, accessibility, and implementation in future projects. but needed access to some native-only functionality.

Plugins/Frameworks used
- Gameplay Ability System or GAS
- Chaos and FieldSystem
- CommonUI

The current source control being used is Diversion because of its simplicity and stability for a solo developer in UE5.
The project is still currently in active development and has many WIP files and methods. 
