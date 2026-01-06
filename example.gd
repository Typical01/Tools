extends Node


@export var tray_menu : PackedScene

func _ready() -> void:
	var example := ExampleClass.new()
	example.print_type(example)


func _on_button_button_up() -> void:
	var tray = tray_menu.instantiate()
	add_child(tray)
	#tray.popup()
	pass # Replace with function body.
