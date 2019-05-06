# Animator For Raylib
A single C++ header file library for sprite-sheet animations in Raylib. 

## Prerequisites

Here is what you need to have set up to get started using AnimatorForRaylib.

1. Raylib source and library files
2. Visual Studio project that can run a Raylib application

### Steps
1. Download the latest release from the 'Releases' tab.
2. Extract the zip file and place the extracted folder in your Visual Studio Solution directory.
3. Open your VS Solution
4. Right click on your project, go to Properties.
5. Go to C/C++ > General, add `$(SolutionDir)AnimatorForRaylib\Include;` in 'Additional Include Directories'.
6. Go to Linker > General, add `$(SolutionDir)AnimatorForRaylib\Library\$(PlatformTarget)\$(Configuration)\;` in 'Additional Library Directories'.
7. Go to Linker > Input, add `AnimatorForRaylib.lib;` in 'Additional Dependencies'.
8. Click 'Apply' and click 'Ok'.

## How to Use

```cpp
#include <raylib.h>
#include "Animator.h"

Animator SpriteAnimator{"GiveItAName", Columns:5, Rows:5, Speed:10};
Texture2D Sprite;
Vector2 Location;

int main()
{
  const int ScreenWidth = 800;
  const int ScreenHeight = 450;

  InitWindow(ScreenWidth, ScreenHeight, "raylib [core] C++ example - basic window");

  SetTargetFPS(120);

  Sprite = LoadTexture("PathToSpritesheet/SpriteName.png");
  Location = {300.0f, 200.0f};

  SpriteAnimator.AssignSprite(Sprite);

  while (!WindowShouldClose())
  {
    // Update
    SpriteAnimator.Play();

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureRec(SpriteAnimator.GetSprite(), SpriteAnimator.GetFrameRec(), Location, WHITE);

    EndDrawing();
  }

  UnloadTexture(Sprite);
  
  CloseWindow();
  
  return 0;
}
```
Build and run the application.

## Result
![](AnimatorDemo.gif)
