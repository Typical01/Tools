class_name TabItemNode extends Panel


@export var icon: Texture2D
@export var item_name: String = "Name":
	get():
		return %TabItemName.text
	set(new_text):
		%TabItemName.text = new_text
@export var item_info: String = "SimpleInfo"
@export var foldable_hight: int = 100
@export var control_interface_scene: Array[PackedScene]
@export var control_interface_node: Array[Node]
@onready var control_interface = %ControlInterface ##用于添加按钮/文本框节点
@onready var foldable_panel = %FoldableContainer ##用于添加下拉画布节点


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	%Icon.texture = icon
	%TabItemInfo.text = item_info
	%OptionButton.visible = false
	foldable_panel.position.y = size.y
	foldable_panel.custom_minimum_size.x = size.x
	foldable_panel.visible = false
	
	for control_scene in control_interface_scene:
		var control_ins = control_scene.instantiate()
		control_interface.add_child(control_ins)
		control_interface_node.append(control_ins)
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass


func _on_option_panel_folding_changed(is_folded: bool) -> void:
	if is_folded:
		foldable_panel.custom_minimum_size.y = 0
		custom_minimum_size.y -= foldable_hight
	else:
		custom_minimum_size.y += foldable_hight
		foldable_panel.custom_minimum_size.y = foldable_hight
	foldable_panel.visible = !is_folded
	pass # Replace with function body.
