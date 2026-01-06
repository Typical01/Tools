class_name TabBarRectNode extends VBoxContainer


@export var title: String
@export var tab_item_list: Array[TabItem]
@onready var tab_item_list_node = %TabItemList ##子选项卡节点


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	%TabName.text = title
	
	for item in tab_item_list:
		if item.only_scene: #非 TabItem只实例化场景
			var scene_ins = item.scene_template.instantiate()
			%TabItemList.add_child(scene_ins)
			return
			
		var scene_ins = item.scene_template.instantiate()
		scene_ins.custom_minimum_size.x = custom_minimum_size.x - 20
		size.y += scene_ins.size.y + 10
		#数据填充
		scene_ins.icon = item.icon
		scene_ins.item_name = item.item_name
		scene_ins.item_info = item.item_info
		scene_ins.foldable_hight = item.foldable_hight
		scene_ins.control_interface_scene = item.control_interface_scene
		
		%TabItemList.add_child(scene_ins)
		#var space = Control.new()
		#space.custom_minimum_size.y = 10
		#%TabItemList.add_child(space) #TabItem之间的间隔
		#print("TabBarRectNode: 添加节点[%s][%s]." % [item.item_name, scene_ins.get_script().get_global_name()])
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
