Name: Miguel Echeverria
Login: miguel.echeverria

Additional Notes: Extra credits implemented
(ASSIGNMENT 1)
- Normal mapping: Hold down N to see scene without normal mapping
- Loading of models through navigation: Models can be loaded by adding a component "ModelInstance" to a node.
ModelInstance components allow choosing which model to load (not sure if this meets the requirements)
- Object creation/deletion: You can right click any node in the scene graph, and click on "Add New Node" to
create a new node and add it as the child of the selected one.
- Transform editing with gizmo

(ASSIGNMENT 2)
- Cube Map: You can select a skybox in the renderer config gui window in the bottom-left.


Known Bugs:
- GLTf nodes without names appear in the scene hierarchy without a name (blank space), but they can still be
selected
- Ctrl + Shift + R to reload resources not implemented
- Be sure to load animation demo last, since going back to another demo is broken. Some of the animations
are not deleted properly for some reason.

Time of implementation: About 8-10 days (not at all times).
Time of testing: Not sure.

IMPORTANT USAGE NOTES:

DEMOS:
- Refer to the top-left of the gui to load the different demos prepared for assignment 2. IMPORTANT: Test the
animation demo last!

Gui usage:
- Top-left window: Scene hierarchy. Right click any node to display options (create child, change name, delete node).

- Top-right window: Component editor. Click add component to add a "ModelInstance" component. A model instance component
allows you to select a gltf file to load. If you select one, you can still change to another one (the previously generated
nodes will be deleted).
When a gltf model is selected in "ModelInstance", it will generate the corresponding nodes in the scene hierarchy, and those
that contain a mesh, will have a "MeshRenderable" component. Just beware that some models' nodes don't have names.
(Please note that no components can be added to the root node)

- When a model instance is selected, this will also automatically add Joint components, a SkeletonRoot component, and a SkinReference
component. This represent the joints of the skeleton, the skeleton root defined in gltf, and the node containing the mesh that referenced
the skin, respectively.

- When a model is loaded through the ModelInstance component, this node will also have an animation component, which allows you
to select different animations, and tweak certain values (if they have animations).

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