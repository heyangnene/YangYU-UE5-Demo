# YangYu UE5 Demo

**YangYu** is a sci-fi cooperative survival demo made with **Unreal Engine 5**.

The project is currently a greybox-to-environment prototype set inside a sci-fi space station combat map. Players protect and repair a large reactor-style core while alien enemies attack it in waves. The demo supports a complete single-player combat loop and basic two-player Listen Server multiplayer synchronization.

The current prototype focuses on:

* Core defense and repair gameplay
* Enemy wave survival
* Player health and down state
* Basic cooperative multiplayer synchronization
* Sci-fi environment and character visual polish
* Shooting feedback and performance testing

---

## Current Milestone

**Demo 0.9 - Player Visual Polish, Shooting Feedback, and Replicated Down State**

This version improves the multiplayer presentation and combat feedback of YangYu.

The player placeholder body has been replaced with a sci-fi armored **Wraith** character mesh for multiplayer visibility. The local player still uses the first-person arms and weapon view, while other players are represented by the Wraith body mesh in the world.

Client projectile aiming has also been improved. Projectiles now use the local camera location and rotation when sending fire requests to the Server, allowing both Host and Client players to shoot correctly based on their own view angle.

Shooting feedback has been improved with glowing projectile visuals, light camera shake, and firing sound feedback.

The player down state now has clearer multiplayer feedback. When a player is down, the local player sees the PLAYER DOWN UI and dark screen overlay, while teammates can see the downed player’s Wraith body lying on the ground. This down visual state is synchronized through a replicated state variable with RepNotify.

---

## Current Gameplay Loop

The current loop is:

Enemy wave starts → Alien enemies move through the space station toward the core → Players shoot enemies → Players repair the core when needed → Repairing disables shooting → Players must choose between combat and repair → Clear all waves / Player down / Core destroyed

The current goal is to:

**Protect the core and clear all 3 enemy waves.**

---

## Current Features

* First-person player movement
* Projectile shooting
* Server-authoritative projectile spawning
* Client projectile aiming based on local camera location and rotation
* Shooting feedback with camera shake and firing sound
* Enemy health and death system
* Three-wave enemy encounter system
* Wave 1 has 5 enemies, Wave 2 has 7 enemies, and Wave 3 has 10 enemies
* Maximum alive enemy count limit to prevent too many enemies from appearing at once
* Enemies move toward and attack the core
* Enemies can damage the player when nearby
* Enemy base class refactored from Actor to Character for AI navigation support
* Enemy movement upgraded to AIController and NavMesh-based navigation
* Enemy AI targets the nearest alive player in multiplayer instead of only targeting Player 0
* Basic enemy aggro behavior: enemies chase nearby players before returning to the core
* Enemy attacks use a short wind-up before applying damage, making combat timing more readable
* Enemy death cleanup stops AI movement, disables collision, and prevents dead enemies from blocking gameplay
* Enemy placeholder mesh has been replaced with an alien predator skeletal mesh
* Enemy visual mesh uses the existing Character Capsule for collision to keep AI navigation stable
* Enemy skeletal mesh collision is disabled to prevent long limbs from blocking navigation or causing wall collisions
* Basic enemy animation state machine added
* Enemy animation switches between standing and movement states based on movement speed
* Enemy attack wind-up now has a clearer visual body motion before damage is applied
* Core health system
* Player health system
* Hold E to continuously repair the core
* Repairing the core disables shooting, creating a trade-off between defense and combat
* Invalid action hint appears when the player tries to shoot while repairing
* Player low-health warning UI
* Player damage screen flash feedback
* Player death state with PLAYER DOWN message
* Replicated player down visual state for teammates
* Downed player mesh animation is paused to prevent visual jitter
* Core destroyed state with CORE DESTROYED message
* Mission Success condition after clearing all enemy waves
* Mission Failed condition when the core is destroyed
* Mission end state locks player input and pauses gameplay after mission success or global mission failure
* In multiplayer, a downed player is locally locked while the other player can continue playing
* Refactored HUD update logic into separate Blueprint events for core, player, score, wave, and mission state
* Player third-person placeholder mesh has been replaced with a sci-fi armored Wraith character model for multiplayer visibility
* Local first-person arms are preserved for the owning player
* Remote players are displayed using the Wraith body mesh

---

## Map and Environment

* Main combat loop has been moved into a sci-fi space station interior map
* A large reactor-style object is used as the visual core
* Original BP_CoreObjective gameplay logic is preserved while the visual core mesh is hidden
* Core collision has been resized to match the large reactor object
* Core hit feedback is shown through a red point-light flash when the core takes damage
* Player starts have been repositioned into a safe side corridor leading into the core room
* Enemy wave spawner has been repositioned to create a corridor-to-core attack route
* NavMesh has been rebuilt for the new combat map
* Hidden navigation helper ramp added to support enemy movement across stairs
* Enemy spawn positions are projected onto NavMesh for more stable spawning
* Enemy spawn spacing checks reduce overlapping enemies at the spawn point
* Added an exposed starfield view above the enemy attack route
* Removed selected ceiling panels from the enemy route hall to reveal outer space
* Starfield background added using the Starfield FREE asset from Fab

---

## Multiplayer Features

* Basic two-player Listen Server multiplayer support
* Host and Client can enter the same level
* Player movement is replicated between Host and Client
* Projectile spawning is handled by the Server and replicated to clients
* Projectile damage is handled on the Server to avoid inconsistent client-side health changes
* Client projectile firing sends camera location and rotation to the Server
* Projectile direction correctly follows the Client player's view angle
* Core health is replicated across Server and Client
* Client repair requests are sent to the Server before modifying core health
* Wave progress, enemy progress, team score, and team kills are replicated from the Server
* Server controls enemy spawning, enemy AI, wave progression, core health, and team score
* Remote player body visibility has been improved using the Wraith character mesh
* Local first-person arms are preserved for the owning player
* Other players see the Wraith body mesh in the world
* Individual player down state is supported in multiplayer
* A downed player is locally locked, while the other player can continue moving, shooting, and defending the core
* Player down visual state is replicated using RepNotify
* Teammates can see the downed player's body lying on the ground

---

## UI and Feedback

* Core health UI
* Player health UI
* Wave progress UI
* Enemy progress UI
* Team score and team kill count UI
* Gameplay instruction HUD
* Low-health warning text
* Player damage screen flash feedback
* Core damage feedback through red point-light flash
* Death overlay when the player is down or when the core is destroyed
* PLAYER DOWN message when the player reaches zero health
* CORE DESTROYED message when the core is destroyed
* MISSION SUCCESS message after all waves are cleared
* Invalid action hint when the player tries to shoot while repairing
* Shooting camera shake feedback
* Shooting sound feedback
* Glowing projectile visual feedback

---

## Controls

* **Left Mouse Button:** Shoot
* **Hold E:** Repair Core

---

## Current Wave Setup

* Wave 1: 5 enemies
* Wave 2: 7 enemies
* Wave 3: 10 enemies
* Maximum alive enemies at the same time: 5

---

## Current Multiplayer Design

* Host acts as the Listen Server
* Server controls enemy spawning, enemy AI, wave progression, core health, and team score
* Clients send shooting and repair requests to the Server
* Core health is shared between all players
* Wave progress, enemy progress, team score, and team kills are shared team-level values
* Individual player death does not immediately fail the mission in multiplayer
* Core destruction remains a global mission failure condition
* Downed players are prevented from moving, looking around, and shooting
* Teammates can visually identify a downed player through the replicated downed body state

---

## Technical Implementation Notes

### Health and Damage

The project uses a reusable health component for player, enemy, and core health logic. Damage and death handling are separated from visual feedback so that gameplay logic can remain stable while visual presentation is improved through Blueprint events.

### Projectile Networking

Projectile spawning is handled through a Server RPC. The local player's camera location and rotation are passed to the Server when firing, allowing Client projectiles to follow the correct aiming direction.

Projectile damage is applied on the Server to avoid inconsistent client-side damage results.

### Player Down Visual State

The player down visual state uses a replicated Boolean variable with RepNotify. When the Server detects that a player's health has reached zero, it sets the replicated down visual state. Each machine then applies the downed body visual locally.

To prevent the Wraith body from jittering after being rotated into a downed pose, the mesh animation is paused when the down visual state is applied. The down pose is also continuously applied while the down state is active to keep the visual state stable during multiplayer testing.

### Enemy Navigation

Enemies use Character-based movement with AIController and NavMesh navigation. The skeletal mesh is used only for visual presentation, while collision and navigation are handled by the Character Capsule to avoid unstable movement caused by complex skeletal mesh collision.

---

## Performance Analysis

During multiplayer PIE testing, the project encountered rendering and video memory pressure after integrating the high-detail space station environment, starfield background, Wraith player mesh, and alien skeletal enemy model. Unreal Engine reported video memory exhaustion, and the frame rate dropped significantly during two-player Listen Server testing.

The issue was analyzed using Unreal Engine profiling commands such as:

* `stat fps`
* `stat unit`
* `stat gpu`

The profiling results showed that the main bottleneck was GPU rendering and video memory usage rather than enemy AI, wave logic, or multiplayer gameplay code.

Optimization steps included:

* Reducing scalability settings during multiplayer testing
* Keeping PIE test windows at a smaller resolution
* Increasing the temporary texture streaming pool using `r.Streaming.PoolSize 2000`
* Disabling unnecessary collision on visual-only meshes
* Keeping enemy navigation collision on the simple Character Capsule instead of the skeletal mesh
* Preserving gameplay logic while lowering rendering pressure for stable testing

After optimization, two-player PIE testing became stable again, with the frame rate returning to 60 FPS during the test scenario.

---

## Current Limitations

* Multiplayer currently focuses on two-player Listen Server testing
* Downed players cannot currently be revived by teammates
* The player down pose is a lightweight visual solution rather than a full death animation montage
* Shooting sound feedback is temporary and may be replaced with a more polished sci-fi weapon sound
* Enemy animation is still basic and does not yet include fully polished attack animation sequences
* The current demo focuses on the core defense loop rather than a full mission structure with extraction

---

## Future Plans

* Add teammate revive interaction for downed players
* Improve player down and recovery flow
* Add more polished sci-fi weapon sound effects
* Add player damage sound effects
* Add core damage sound effects
* Further enemy AI polish and behavior variation
* Further enemy animation polish
* Add more advanced enemy attack animations
* Add enemy type variations
* Add beacon charging objective
* Add extraction objective
* Expand multiplayer co-op support for 2-4 players
* Add role-based weapons or abilities, such as pulse weapon, gravity device, and guardian shield/support device
* Improve final UI presentation and mission result screen
* Add more ambient audio and environmental sound effects
* Further optimize the scene for smoother multiplayer demonstration

---

## Third-Party Assets

* Sci-fi space station environment asset from Fab
* Starfield background asset from Fab: Starfield FREE by Nebulosity
* Alien enemy character asset from Fab: TEUTHISAN | Alien Predator (FREE) by Aaron Sims Creative
* Player character asset: Paragon: Wraith by Epic Games

---

## Development Status

The project is currently in a playable prototype stage.

The current demo includes:

* A playable sci-fi space station combat map
* Core defense and repair gameplay
* Three enemy waves
* Alien enemy model and basic enemy animation
* Wraith player body model for multiplayer visibility
* Server-authoritative shooting and damage
* Basic two-player Listen Server support
* Replicated core health, wave progress, score, kills, and player down visual state
* Shooting feedback through camera shake and sound
* Performance profiling and optimization records

The next major development direction is to improve cooperative interaction, especially teammate revive gameplay and stronger mission objective structure.
