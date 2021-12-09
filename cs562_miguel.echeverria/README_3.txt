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


Known Bugs:
- Ctrl + Shift + R to reload resources not implemented
- Deleting objects and components manually is mostly broken and will probably crash, so be careful when adding points to a curve.

Time of implementation: About 5-6 days (without extra credit work).
Time of testing: Not sure, but extensive testing while implementing was done.

IMPORTANT USAGE NOTES:

DEMOS:
- NEW: Refer to the top-left of the gui to load the different demos prepared for assignment 2. PATH FOLLOWING is the name
of the demo for assignment 3.

Gui usage:
- Top-left window: Scene hierarchy. Right click any node to display options (create child, change name, delete node).

- Top-right window: Component editor. Click add component to add a "ModelInstance" component. A model instance component
allows you to select a gltf file to load. If you select one, you can still change to another one (the previously generated
nodes will be deleted).
When a gltf model is selected in "ModelInstance", it will generate the corresponding nodes in the scene hierarchy, and those
that contain for example a mesh, will have a "MeshRenderable" component with some info. Child nodes of a model can also be
added other informational components like Joint, SkinReference, or SkeletonRoot.
(Please note that no components can be added to the root node)

- When a model instance is selected, this will also automatically add Joint components, a SkeletonRoot component, and a SkinReference
component. This represent the joints of the skeleton, the skeleton root defined in gltf, and the node containing the mesh that referenced
the skin, respectively.

- When a model is loaded through the ModelInstance component, THIS node will also have an animation component, which allows you
to select different animations, and tweak certain values (if they have animations).

- NEW: You can also add a PiecewiseCurve component to any node. This component allows you to add points to form segments on the curve
(these will be added as children of the curve root node), just be careful because deleting them is broken at the moment. When a curve
node is added, if the curve type was hermite or bezier, there will also be tangents/control point nodes respectively added as children
of each curve point node.

- NEW: You can control all of the parametes for this assignment's demo in the gui of the PiecewiseCurve component (the component
in the root node of the curve). IMPORTANT: This is where you will need to generate the arc length table in order to see the
model moving. Every time you change a value like number of samples or initial tolerance, you will have to recompute the table.

- NEW: There is drop-down menu at the top left bar, which some options for debug drawing of bounding volumes, skeletons, or curves.
This is where you can select enabling drawing of the samples of the arc length table curve.

- NEW: The new mixamo model has been added as part of data, and it works, you can test it out if you want.

- Botom-left window: Camera controls and configuration.
- Bottom-right window: Light properties. Can also change the skybox or disable/enable it.

Camera controls:
- First person camera, as specified in the handout
- For looking around, you need to right click and drag
- There is an imgui window that allows you to tweak the speed of the camera
- V: Reset the camera

Other controls:
- Ctrl + R: Reset the scene (delete all nodes except root)
- Ctrl + Q: Quit the application

Note:
- If you want to add multiple models, you can create multiple objects in the scene with "ModelInstance" component.