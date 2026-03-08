extends OptionButton


signal item_selected_index(index)


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass
	
func init_option_list(option_list: Array) -> void:
	for option in option_list:
		var resolution = option as Vector2i
		add_item("%sx%s" % [resolution.x, resolution.y])
	pass


func _on_item_selected(index: int) -> void:
	item_selected_index.emit(index)
	pass # Replace with function body.
