Name: Miguel Echeverria
Login: miguel.echeverria

Additional Notes: Extra credits implemented
- Normal mapping: Hold down N to see scene without normal mapping
- Loading of models through navigation: Models can be loaded by adding a component "ModelInstance" to a node.
ModelInstance components allow choosing which model to load (not sure if this meets the requirements)

Known Bugs:
- GLTf nodes without names appear in the scene hierarchy without a name (blank space)
- Ctrl + Shift + R to reload resources not implemented

Time of implementation: About 9-10 days (not at all times).
Time of testing: Hard to say, but at least a couple hours dedicated specifically for testing everything.

IMPORTANT USAGE NOTES:

Gui usage:
- Top-left window: Scene hierarchy. Right click any node to display options (create child, change name, delete node).

- Top-right window: Component editor. Click add component to add a "ModelInstance" component. A model instance component
allows you to select a gltf file to load. If you select one, you can still change to another one (the previously generated
nodes will be deleted).
When a gltf model is selected in "ModelInstance", it will generate the corresponding nodes in the scene hierarchy, and those
that contain a mesh, will have a "MeshRenderable" component. Just beware that some models' nodes don't have names.
(Please note that no components can be added to the root node)

- Botom-left window: Camera controls and configuration.
- Bottom-right window: Light properties.

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