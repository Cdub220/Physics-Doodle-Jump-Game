# Game Design Document

## Section 0: Summary
This section should tell the reader what to expect in the future sections.  In particular, it should contain at least:
- a working title for your game
- a list of your team members (and their roles if decided)
- a "concept statement" of your game ("your game in a tweet")

Doodle Jump Clone
Team members: Dean Yao, Chase Williamson, Salviya Balami 
Concept Statement - "Launch your doodle hero on an endless climb! Bounce from platform to platform, dodge enemy fire, and see how high you can climb!

## Section 1: Gameplay
This section should address simple questions about how your game works:
- How does your game progress?
As the player jumps higher and higher, the screen keeps shifting and spawning more platforms.
- What are the win and loss conditions?
The game is infinite, just like real doodle jump. Loss conditions: The doodler doesn’t hit a platform and falls out of frame. Doodler is hit by enemies’ bullets. 
- Are there levels?
As score progresses, so does difficulty. After a certain threshold of score, an enemy shooting bullets from the top gets introduced. After a certain threshold of score, platform count decreases and more difficult platforms get introduced.
- Are there points?
Score will increase based on the how far the user goes.

This section should also address:
- **controls** (keyboard? mouse? tongue?)
Right/Left + space keys.
- **physics** (how does your game incorporate the physics engine?)
Gravity and collisions.
- **game flow** (what does the game look like from start to end for the player?)
Game starts and user starts on a platform. User plays game, progressing upwards from platform to platform. If the player loses, a game over screen will be shown.
- **graphics** (will you draw polygons? use sprites? make your own vector graphics?)
Sprites for the user and enemy. Polygons for platforms and bullets.

## Section 2: Feature Set
This section should reduce your game to a set of individual features (remember iterative development?).  Your team should
assign these features to individual group members and assign each one a priority from one to four (1 = game cannot work without this, 4 = cool but not necessary).

We have gathered together the following list of some example features you might choose to implement:
- make your own graphics or sprites
- add sound effects
- implement a scrolling environment
- implement a networked/multiplayer game
- implement 2D parallax (https://en.wikipedia.org/wiki/Parallax)
- implement rendering of text
- implement a mouse handler
- implement an AI for an enemy
- implement speed-independent friction
- implement more accurate integration (current implementation uses a trapezoid sum)
- implement music

**Rank 1: **

Doodler Initialize JOSH 
Platforms initialize DEAN 
Basic doodler movement CHASE 

At this stage, we should have a fixed screen with a constant number of platforms. The doodler should be able to move left and right as well as wrap around the screen.  

**Rank 2 + Rank 3: **

Doodler Bounce (2 features) - clip through bottom of platform ie no bounce + bounce on platforms CHASE + DEAN
Screen movement (2 features) - move screen down + keep doodler at constant y after screen moves down JOSH + DEAN
Rendering new platforms CHASE 

Platforms variation DEAN 
Bounce variation CHASE 
Increasing difficulty JOSH 

The doodler should be able to jump on the platforms according to physics. When the doodler reaches a certain y coordinate, the screen seems to “move” down. The first idea for how to do this is to have all bodies on the screen to move down as the doodler moves up. 

The game should now see the following: 
- The doodler bounces on the platforms appropriately. 
- When the doodler gets to a certain y, the screen shifts, and new platforms are rendered. 
- There are now different types of platforms, moving, constant, and breaking. 
- There is a score on the screen that increases with either time, y distance traveled, or platforms jumped on. 
- As the score increases above certain thresholds, the difficulty increases by spawning in the more challenging platforms at a higher rate. 

**Rank 4:  **
Music JOSH 
Game over condition DEAN 
Score CHASE 

This the creative imaginative part. There could be background music while we play the game. This is all stuff that would be cool to have but doesn’t affect the functionality of the game at all.  

## Section 3: Timeline
This section should assign every feature in the previous section to a particular group member and a particular week they will implement it.
We would aim to complete rank 1 in week 1, rank 2 in week 2, rank 3 in week 3, and as much of rank 4 features as we can throughout.

Week 1:
CHASE - Basic doodler movement
DEAN - Platforms initialize
JOSH - Doodler Initialize

Week 2:
Doodler Bounce (2 features) - clip through bottom of platform ie no bounce + bounce on platforms CHASE + DEAN
Screen movement (2 features) - move screen down + keep doodler at constant y after screen moves down JOSH + DEAN
Rendering new platforms CHASE

Week 3:
Platforms variation DEAN 
Bounce variation CHASE 
Increasing difficulty JOSH 
Music JOSH 
Game over condition DEAN 
Score CHASE 

## Section 4: Disaster Recovery
This section should describe how each member plans to get back on track if they fall behind.  Please take this section seriously.

Dean - if I fall behind, I will attend office hours every time I am available to catch up. I will dedicate a higher portion of my time to the project and grind to make sure I catch up. I will also use the TAs, Jedi, and Professor Blank as resources for help. To prevent me falling behind, I will work extremely diligently and always keep my groupmates updated on my progress.

Chase - If I fall behind I will attend OH use the time to ask questions about how to go about implementing certain aspects of the game. I will also talk with my groupmates to see if they have any tips to help me. Lastly I will spend more time on the project in order to catch up and not hold my groupmates down.

Josh - In the event that I fall behind on the timeline, I will devot time both inside of recitation and outside of class to meet with my groupmates in order to catch up on my portion of work. I will also communication with Profesor Blank, Jedi, and the rest of the TAs to verbalize any higher level questions that I run into to make up for the lost time.
