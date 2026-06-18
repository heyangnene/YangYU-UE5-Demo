\# YangYu UE5 Demo



YangYu is a sci-fi cooperative survival demo made with Unreal Engine 5.



The current version is a greybox-to-environment prototype built inside a sci-fi space station combat map. The player protects and repairs a large reactor-style core while alien enemies attack it in waves. The demo supports a complete single-player combat loop and basic two-player listen-server multiplayer synchronization.



\## Current Features



\* First-person player movement

\* Projectile shooting

\* Enemy health and death system

\* Three-wave enemy encounter system

\* Wave 1 has 5 enemies, Wave 2 has 7 enemies, and Wave 3 has 10 enemies

\* Maximum alive enemy count limit to prevent too many enemies from appearing at once

\* Enemies move toward and attack the core

\* Enemies can damage the player when nearby

\* Enemy base class refactored from Actor to Character for AI navigation support

\* Enemy movement upgraded to AIController and NavMesh-based navigation

\* Enemy AI targets the nearest alive player in multiplayer instead of only targeting Player 0

\* Basic enemy aggro behavior: enemies chase nearby players before returning to the core

\* Enemy attacks use a short wind-up before applying damage, making combat timing more readable

\* Enemy death cleanup stops AI movement, disables collision, and prevents dead enemies from blocking gameplay

\* Enemy placeholder mesh has been replaced with an alien predator skeletal mesh

\* Enemy visual mesh uses the existing Character Capsule for collision to keep AI navigation stable

\* Enemy skeletal mesh collision is disabled to prevent long limbs from blocking navigation or causing wall collisions

\* Basic enemy animation state machine added

\* Enemy animation switches between standing and movement states based on movement speed

\* Enemy attack wind-up now has a clearer visual body motion before damage is applied

\* Core health system

\* Player health system

\* Hold E to continuously repair the core

\* Repairing the core disables shooting, creating a trade-off between defense and combat

\* Invalid action hint appears when the player tries to shoot while repairing

\* Player low-health warning UI

\* Player damage screen flash feedback

\* Player death state with PLAYER DOWN message

\* Core destroyed state with CORE DESTROYED message

\* Mission Success condition after clearing all enemy waves

\* Mission Failed condition when the core is destroyed

\* Mission end state locks player input and pauses gameplay after mission success or global mission failure

\* In multiplayer, a downed player is locally locked while the other player can continue playing

\* Refactored HUD update logic into separate Blueprint events for core, player, score, wave, and mission state



\## Map and Environment



\* Main combat loop has been moved into a sci-fi space station interior map

\* A large reactor-style object is used as the visual core

\* Original BP\_CoreObjective gameplay logic is preserved while the visual core mesh is hidden

\* Core collision has been resized to match the large reactor object

\* Core hit feedback is now shown through a red point-light flash when the core takes damage

\* Player starts have been repositioned into a safe side corridor leading into the core room

\* Enemy wave spawner has been repositioned to create a corridor-to-core attack route

\* NavMesh has been rebuilt for the new combat map

\* Hidden navigation helper ramp added to support enemy movement across stairs

\* Enemy spawn positions are projected onto NavMesh for more stable spawning

\* Enemy spawn spacing checks reduce overlapping enemies at the spawn point

\* Added an exposed starfield view above the enemy attack route

\* Removed selected ceiling panels from the enemy route hall to reveal outer space

\* Starfield background added using the Starfield FREE asset from Fab



\## Multiplayer Features



\* Basic two-player listen-server multiplayer support

\* Host and Client can enter the same level

\* Player movement is replicated between Host and Client

\* Projectile spawning is handled by the Server and replicated to clients

\* Projectile damage is handled on the Server to avoid inconsistent client-side health changes

\* Core health is replicated across Server and Client

\* Client repair requests are sent to the Server before modifying core health

\* Wave progress, enemy progress, team score, and team kills are replicated from the Server

\* Server controls enemy spawning, enemy AI, wave progression, core health, and team score



\## UI and Feedback



\* Core health UI

\* Player health UI

\* Wave progress UI

\* Enemy progress UI

\* Team score and team kill count UI

\* Gameplay instruction HUD

\* Low-health warning text

\* Player damage screen flash feedback

\* Core damage feedback through red point-light flash

\* Death overlay when the player is down or when the core is destroyed

\* PLAYER DOWN message when the player reaches zero health

\* CORE DESTROYED message when the core is destroyed

\* MISSION SUCCESS message after all waves are cleared



\## Controls



\* Left Mouse Button: Shoot

\* Hold E: Repair Core



\## Current Goal



Protect the core and clear all 3 enemy waves.



\## Current Wave Setup



\* Wave 1: 5 enemies

\* Wave 2: 7 enemies

\* Wave 3: 10 enemies

\* Maximum alive enemies at the same time: 5



\## Current Multiplayer Design



\* Host acts as the listen server

\* Server controls enemy spawning, enemy AI, wave progression, core health, and team score

\* Clients send shooting and repair requests to the Server

\* Core health is shared between all players

\* Wave progress, enemy progress, team score, and team kills are shared team-level values

\* Individual player death does not immediately fail the mission in multiplayer

\* Core destruction remains a global mission failure condition



\## Development Status



Current milestone:



\*\*Demo 0.8 - Alien Enemy Visual and Animation Polish\*\*



This version replaces the original placeholder enemy block with an alien predator skeletal mesh and adds a basic animation state machine for enemy movement. The original enemy AI, health, attack wind-up, wave spawning, multiplayer synchronization, and death cleanup logic were preserved while the enemy visual presentation was improved. The enemy still uses a simple Character Capsule for collision and navigation, while the skeletal mesh is used only for visual appearance.



The current loop is:



Enemy wave starts → Alien enemies move through the space station toward the core → Players shoot enemies → Players repair the core when needed → Repairing disables shooting → Players must choose between combat and repair → Clear all waves / Player down / Core destroyed



\## Performance Analysis



During multiplayer PIE testing, the project encountered rendering and video memory pressure after integrating the high-detail space station environment, starfield background, and alien skeletal enemy model. Unreal Engine reported video memory exhaustion, and the frame rate dropped significantly during two-player Listen Server testing.



The issue was analyzed using Unreal Engine profiling commands such as `stat fps`, `stat unit`, and `stat gpu`. The profiling results showed that the main bottleneck was GPU rendering and video memory usage rather than enemy AI, wave logic, or multiplayer gameplay code.



Optimization steps included:



\* Reducing scalability settings during multiplayer testing

\* Keeping PIE test windows at a smaller resolution

\* Increasing the temporary texture streaming pool using `r.Streaming.PoolSize 2000`

\* Disabling unnecessary collision on visual-only meshes

\* Keeping enemy navigation collision on the simple Character Capsule instead of the skeletal mesh

\* Preserving gameplay logic while lowering rendering pressure for stable testing



After optimization, two-player PIE testing became stable again, with the frame rate returning to 60 FPS during the test scenario.



\## Future Plans



\* Further enemy AI polish and behavior variation

\* Further enemy animation polish

\* More advanced enemy attack animations

\* Enemy type variations

\* Player damage sound effects

\* Core damage sound effects

\* Beacon charging objective

\* Extraction objective

\* Expanded multiplayer co-op support for 2–4 players

\* Role-based weapons: pulse weapon, gravity device, and guardian shield/support device

\* 3D character models

\* Player full-body model for multiplayer visibility

\* Environment, weapon, and enemy visual polish

\* Background and lighting improvements

\* Sound effect and ambient audio improvements

\* Performance profiling and optimization



\## Third-Party Assets



\* Sci-fi space station environment asset from Fab

\* Starfield background asset from Fab: Starfield FREE by Nebulosity

\* Alien enemy character asset from Fab: TEUTHISAN | Alien Predator (FREE) by Aaron Sims Creative



