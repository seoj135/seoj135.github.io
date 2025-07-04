import bpy

# Iterate through all selected objects
for obj in bpy.context.selected_objects:
    if obj.type == 'MESH':  # Check if the object is a mesh
        # Switch to Object Mode to apply modifier
        bpy.context.view_layer.objects.active = obj
        bpy.ops.object.mode_set(mode='OBJECT')
        
        # Add a Decimate modifier
        decimate_modifier = obj.modifiers.new(name="Decimate", type='DECIMATE')
        
        # Set the decimate ratio (0.1 means keeping 10% of the original faces)
        decimate_modifier.ratio = 0.3  # You can adjust this value as needed (e.g., 0.5 for 50%)
        
        # Apply the modifier
        bpy.ops.object.modifier_apply(modifier=decimate_modifier.name)
        
        # Optionally, you can switch back to Edit Mode if necessary
        # bpy.ops.object.mode_set(mode='EDIT')

print("Decimation applied to all selected mesh objects.")
