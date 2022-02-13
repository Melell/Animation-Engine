Name: Miguel Echeverria
Login: miguel.echeverria

Additional Notes: Extra credits implemented
(ASSIGNMENT 1)
- Add model from file to child of a selected node
- Object creation/deletion
- Transform editing with gizmo
- Normal mapping: Hold down N to see scene without normal mapping

(ASSIGNMENT 2)
- Cube Map: You can select a skybox in the renderer config gui window in the bottom-left.
- Curve editing: Point picking was missing

(ASSIGNMENT 3)
- Bounding volume computation and drawing
- Picking
- Drag and drop

(ASSIGNMENT 4)
- Spherical Camera? (Only available with controller in BLENDING_1D demo)
- 1D Blending Editor: You can add/remove nodes, change the animations of the nodes, change the position of the nodes,
		      and move the blend parameter by clicking directly on it and dragging.
- 2D Blending Editor: Same controls as 1d editor.
- Triangulation with Delaunay

(ASSIGNMENT 5)
- Kinematic Chain Editing:
    - On the "CHAIN ROOT" node, you can push/pop joints to the chain through the components gui.
    - You can modify the lengths of the bones at run-time by selecting one of its joint in the scene graph, and moving
      it (thus making one end's bone larger, and the other end's bone smaller)
- Skeleton IK:
    - The last demo contains a Brain-Stem model, and an ik chain that goes from the left hip, until the left foot.
      By selecting the "TARGET" node in the scene graph, you can move the target that the end effector will try to
      follow.


Known Bugs:
- Ctrl + Shift + R to reload resources not implemented
- Deleting objects and components manually is mostly broken and will probably crash, so don't try any of that at the moment.
- CCD works weird, I don't know whether it is normal or I have something wrong.

Time of implementation: About 4-5 days.
Time of testing: Not sure.

IMPORTANT USAGE NOTES:

DEMOS:
- NEW, IMPORTANT: Top-left of the gui to load the demos. IK_... are the demos implemented for this assignment. On all the demos,
  you should look in the scene graph for the objects named "CHAIN ROOT", "END EFFECTOR", and "TARGET". "CHAIN ROOT" has the
  component with all the parameters you can tweak for the demos. "TARGET" is the object that you should move to verify the
  inverse kinematics work.

Gui usage:
- Top-left window: Scene hierarchy. Right click any node to display options (create child, change name, delete node).

- Top-right window: Component editor. Click add component to add a "ModelInstance" component. A model instance component
allows you to select a gltf file to load. If you select one, you can still change to another one (the previously generated
nodes will be deleted).
When a gltf model is selected in "ModelInstance", it will generate the corresponding nodes in the scene hierarchy, and those
that contain for example a mesh, will have a "MeshRenderable" component with some info. Child nodes of a model can also be
added other informational components like Joint, SkinReference, or SkeletonRoot.
(Please note that no components can be added to the root node of the scene)

- When a model instance is selected, this will also automatically add Joint components, a SkeletonRoot component, and a SkinReference
component. This represent the joints of the skeleton, the skeleton root defined in gltf, and the node containing the mesh that referenced
the skin, respectively.

- When a model is loaded through the ModelInstance component, THIS node will also have an animation component, which allows you
to select different animations, and tweak certain values (if they have animations).

- You can also add a PiecewiseCurve component to any node. This component allows you to add points to form segments on the curve
(these will be added as children of the curve root node), just be careful because deleting them is broken at the moment. When a curve
node is added, if the curve type was hermite or bezier, there will also be tangents/control point nodes respectively added as children
of each curve point node.

- You can control all of the parametes for this assignment's demo in the gui of the PiecewiseCurve component (the component
in the root node of the curve). This is where you will need to generate the arc length table in order to see the
model moving. Every time you change a value like number of samples or initial tolerance, you will have to recompute the table.

- There is drop-down menu at the top left bar, which some options for debug drawing of bounding volumes, skeletons, curves,
or even the grid of the level.
This is where you can select enabling drawing of the samples of the arc length table curve.

- Botom-left window: Camera controls and configuration.
- Bottom-right window: Light properties. Can also change the skybox or disable/enable it.

Editor Camera controls:
- First person camera, as specified in the handout
- For looking around, you need to right click and drag
- There is an imgui window that allows you to tweak the speed of the camera
- V: Reset the camera

Spherical Camera controls(only for Blending_1D demo):
- Right Stick: Move camera around player

Other controls:
- Ctrl + R: Reset the scene (delete all nodes except root)
- Ctrl + Q: Quit the application
- Left Stick: Move player (for Blending_1D and Blending_2D demos)
- Right Trigger: Sprint (for Blending_1D demo)

Note:
- NEW, IMPORTANT: The ik solver has the following status: Success(green), failure(red), processing(yellow/orange), and idle(purple).
  The ik chain will be displayed in the color corresponding to the solver's current status. However, since all the iterations are
  being done on the same frame, processing status will not be visible at the moment. Also, idle will only be visible when loading
  the demo, because the rest of the times it will always show the last status (success or failure).
- NEW, IMPORTANT: You can also change the maximum allowed error and the maximum allowed iterations of each solver method, in the
  "CHAIN ROOT", component gui.


- If you want to add multiple models, you can create multiple objects in the scene with "ModelInstance" component.
- The Blending_1D scene is a directional movement demo (controller required).
- The Blending_2D scene is a targeted movement demo (controller required, although camera is fixed).
- The Blending_1D_Editor scene is a similar demo as Blending_1D, but without the player controller,
  so that you can test out the 1d blend tree editor. (also uses the editor camera)
- The Blending_2D_Editor scene is a similar demo as Blending_2D, but without the player controller,
  so that you can test out the 2d blend tree editor. (also uses the editor camera)
- When loading one of the demos, always open the AnimationReference component gui of the model root
  node (named "X-BOT" in the demos), so that you can view all the options, or the parameter changing when the stick is
  moved.