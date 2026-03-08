class_name TabListItemNode extends Panel


signal button_up_call(item_name)
signal button_move_up_call(item_name)
signal button_move_down_call(item_name)
signal button_double_clicked()
signal check_button_toggled_switch(item_name, toggled_on)

@export var item_name: String = "Name":
	get():
		return %ItemName.text
	set(new_text):
		%ItemName.text = new_text
@export var disable: bool:
	get():
		return !%CheckButton.button_pressed
	set(toggled_on):
		%CheckButton.button_pressed = !toggled_on
var last_click_time: int = 0
@onready var button_node = %Button
@onready var check_button_node = %CheckButton
@export var double_click_threshold: float = 0.3  # 双击时间阈值

func _ready() -> void:
	pass

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func set_transparency(toggled_on: bool):
	if toggled_on:
		%ItemName.visible = false
		%Icon.visible = false
		%CheckButton.visible = false
	else:
		%ItemName.visible = true
		%Icon.visible = true
		%CheckButton.visible = true

func _on_button_pressed() -> void:
	var current_time = Time.get_ticks_msec()
	var time_since_last_click = (current_time - last_click_time) / 1000.0  # 转换为秒
	if time_since_last_click < double_click_threshold:
		# 触发双击事件
		button_double_clicked.emit()
		last_click_time = 0  # 重置，防止连续触发
	else:
		last_click_time = current_time

func _on_button_up() -> void:
	button_up_call.emit(item_name)
	
func show_move_button(_button_pressed: bool) -> void:
	button_node.button_pressed = _button_pressed
	if button_node.button_pressed:
		%Up.visible = true
		%Down.visible = true
	else:
		%Up.visible = false
		%Down.visible = false

func _on_button_mouse_entered() -> void:
	pass

func _on_button_mouse_exited() -> void:
	pass

func _on_check_button_toggled_switch(toggled_on: Variant) -> void:
	check_button_toggled_switch.emit(item_name, toggled_on)


func _on_up_button_up() -> void:
	button_move_up_call.emit(item_name)

func _on_down_button_up() -> void:
	button_move_down_call.emit(item_name)
