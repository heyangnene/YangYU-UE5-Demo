\# YangYu UE5 Demo



YangYu is a sci-fi survival demo made with Unreal Engine 5.



The current version is a single-player greybox prototype.

The player protects and repairs a core while enemies attack it in waves. The player can also be damaged by enemies and will enter a downed state when health reaches zero.



\## Current Features



\- First-person player movement

\- Projectile shooting

\- Enemy health and death system

\- Three-wave enemy encounter system

\- Wave 1 has 5 enemies, Wave 2 has 7 enemies, and Wave 3 has 10 enemies

\- Maximum alive enemy count limit to prevent too many enemies from appearing at once

\- Enemies move toward and attack the core

\- Enemies can damage the player when nearby

\- Enemy base class refactored from Actor to Character for AI navigation support

\- Enemy movement upgraded to AIController and NavMesh-based navigation

\- Enemy death cleanup stops AI movement, disables collision, and prevents dead enemies from blocking gameplay

\- Basic enemy aggro behavior: enemies chase nearby players before returning to the core

\- Core health system

\- Player health system

\- Hold E to continuously repair the core

\- Repairing the core disables shooting, creating a trade-off between defense and combat

\- Invalid action hint appears when the player tries to shoot while repairing

\- Core health UI

\- Player health UI

\- Wave progress UI

\- Enemy progress UI

\- Score and kill count UI

\- Gameplay instruction HUD

\- Player damage screen flash feedback

\- Core damage feedback through red core glow

\- Player death state with PLAYER DOWN message

\- Core destroyed state with CORE DESTROYED message

\- Death overlay when the player is down or when the core is destroyed

\- Mission Failed condition when the core is destroyed

\- Mission Failed condition when the player is down

\- Mission Success condition after clearing all enemy waves

\- Refactored HUD update logic into separate Blueprint events for core, player, score, wave, and mission state



\## Controls



\- Left Mouse Button: Shoot

\- Hold E: Repair Core



\## Current Goal



Protect the core and clear all 3 enemy waves.



\## Development Status



Current milestone:



\*\*Demo 0.3 - Three-Wave Combat Loop\*\*



This version expands the prototype from a simple timed survival test into a structured wave-based combat loop. The player must protect the core, manage repair timing, stop enemies from overwhelming the objective, and clear all enemy waves to complete the mission.



The current loop is:



Enemy wave starts → Enemies attack the core → Player shoots enemies → Player repairs the core when needed → Repairing disables shooting → Player must choose between combat and repair → Clear all waves / Player down / Core destroyed



\## Current Wave Setup



\- Wave 1: 5 enemies

\- Wave 2: 7 enemies

\- Wave 3: 10 enemies

\- Maximum alive enemies at the same time: 5



\## Future Plans



\- Further enemy AI polish and behavior variation

\- Enemy attack behavior improvements

\- Enemy type variations

\- Player damage sound effects

\- Core damage sound effects

\- Low-health player warning feedback

\- Beacon charging objective

\- Extraction objective

\- Multiplayer co-op support for 2–4 players

\- Role-based weapons: pulse weapon, gravity device, and guardian shield/support device

\- 3D character models

\- Environment, weapon, and enemy visual polish

\- Background and lighting improvements

\- Sound effect and ambient audio improvements

\- Performance profiling and optimization

