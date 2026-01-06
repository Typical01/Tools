class_name CheckButtonSwitch extends CheckButton


signal toggled_switch(toggled_on)
@export var show_text: bool = true

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if !show_text:
		text = ""
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func text_toggled() -> void:
	if show_text:
		if button_pressed:
			text = "开"
		else:
			text = "关"

func _on_toggled(toggled_on: bool) -> void:
	if show_text:
		if toggled_on:
			text = "开"
		else:
			text = "关"
	toggled_switch.emit(toggled_on)
	pass # Replace with function body.
