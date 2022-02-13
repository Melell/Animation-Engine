--- This is a shortened version with just the info relevant to the cloth assignment ---
(Refer to README_GENERAL for general engine instructions from previous assignments)

• Name/Student ID: Miguel Echeverria / 540000918

• Login: miguel.echeverria

• Additional Notes: Extra credits implemented
(CLOTH ASSIGNMENT)
	- Wind Effect

• Known Bugs:
	- Ctrl + Shift + R to reload resources not implemented
	- Deleting objects and components manually is mostly broken and will probably crash, so don't try any of that at the moment.

(CLOTH ASSIGNMENT)
	- Collision with sphere gives artifacts (individual particles are constrained to be outside the sphere, but the triangles
	  that are rendered overlap the sphere), as the cloth is not very high resolution.
	- Being able to select a particle and move it, as well as change its state (static or dynamic) through editor not implemented.

• Time of implementation: About a day and a half only doing this assignment.

• Time of testing: Not sure, but probably not as much as the previous assignments.

• Comments:
	- Top-left of the gui: Load scene menu. CLOTH SIMULATION and CLOTH COLLISION are the demos implemented for this assignment.
	- CLOTH_SIMULATION: Select the scene node named "CLOTH". You can then change the wind direction and scale that affects the
	  cloth in the cloth component gui.
	- CLOTH_COLLISION: Same as CLOTH_SIMULATION, but here you can also select the sphere, and move it around to see how the cloth
	  interacts with it. However, please always scale the sphere uniformly (if you want to scale it).
	- Both demos: Please note that in both demos, moving the actual cloth scene node won't move the cloth, as the particles' initial
	  positions and updated positions don't take this into account.
	- Constraints Rendering: Top-left of the gui -> Debug Drawing -> CLOTH. Here you can alter between debug drawing of the
	  cloth's constraints, and the cloth being rendered with a normal mapped texture.