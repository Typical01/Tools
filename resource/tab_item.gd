class_name TabItem extends Resource


@export var scene_template: PackedScene
@export var only_scene: bool = false
@export var icon: Texture2D
@export var item_name: String
@export var item_info: String
@export var foldable_hight: int = 100
@export var control_interface_scene: Array[PackedScene]


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass
