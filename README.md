\# YangYu UE5 Demo



YangYu is a sci-fi survival demo made with Unreal Engine 5.



The current version is a single-player greybox prototype.

The player protects and repairs a core while enemies continuously attack it. The player can also be damaged by enemies and will enter a downed state when health reaches zero.



\## Current Features



\- First-person player movement

\- Projectile shooting

\- Enemy health and death system

\- Enemy wave spawning

\- Enemies move toward and attack the core

\- Enemies can damage the player when nearby

\- Core health system

\- Player health system

\- Core repair interaction with E key

\- Core health UI

\- Player health UI

\- Gameplay instruction HUD

\- Player damage screen flash feedback

\- Core damage feedback through red core glow

\- Basic enemy aggro behavior: enemies chase nearby players before returning to the core

\- Basic score and kill count system

\- Player death state with PLAYER DOWN message

\- Core destroyed state with CORE DESTROYED message

\- Death overlay when the player is down

\- Mission Failed condition when the core is destroyed

\- Mission Failed condition when the player is down

\- Mission Success condition after surviving 60 seconds



\## Controls



\- Left Mouse Button: Shoot

\- E: Repair Core



\## Current Goal



Protect the core and survive for 60 seconds.



\## Development Status



Current milestone:



\*\*Demo 0.2 - Damage Feedback and Player Death State\*\*



This version expands the core gameplay loop with player health, enemy damage to the player, separated damage feedback for the player and the core, and a clear player down state.



The current loop is:



Enemy spawning → Core attack → Player defense → Player damage → Core repair → Player down / Core destroyed / Survival success



\## Future Plans



\- Long-press repair interaction

\- Player damage sound effects

\- Core damage sound effects

\- Low-health player warning feedback

\- Enemy aggro and player-chasing behavior

\- More polished enemy AI with navigation

\- Wave-based mission structure

\- Beacon charging objective

\- Extraction objective

\- Multiplayer co-op support for 2–4 players

\- Role-based weapons: pulse weapon, gravity device, and guardian shield/support device

\- Environment, weapon, and enemy visual polish

\- Performance profiling and optimization

