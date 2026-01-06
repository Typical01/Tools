class_name TabListNode extends VBoxContainer


signal init_list_item
signal list_item_add(title_name)
signal list_item_select(item_name)
signal list_item_edit(item_name)
signal list_item_check(title_name, toggled_on)
signal list_item_change(tab_item_list)
signal list_item_del(item_name)
signal list_item_move_up(item_name)
signal list_item_move_down(item_name)
signal current_item_change(index)

@export var title_name: String:
	get():
		return %TabName.text
	set(text):
		%TabName.text = text
@export var title_info: String:
	get():
		return %TabInfo.text
	set(text):
		%TabInfo.text = text
var current_name: String
var item_node_max_size_x = 418.0
@export var scene_template: PackedScene
var tab_item_list: Dictionary ## 保存节点名和索引
var current_item: int = -1:
	set(index):
		current_item = index
		current_item_change.emit(index)
@onready var tab_item_list_node = %TabItemList ##子选项卡节点

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	call_deferred("_deferred_init")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _deferred_init() -> void:
	init_list_item.emit()

func clear_item_list_node() -> void:
	for child in tab_item_list_node.get_children():
		remove_item(child.item_name, false)

func update_index() -> void:
	for child in tab_item_list_node.get_children():
		tab_item_list[child.item_name] = child.get_index()

func get_item_list_dict() -> Dictionary:
	var item_list_name: Dictionary
	for child in tab_item_list_node.get_children():
		item_list_name.set(child.item_name, child.disable)
	return item_list_name

func add_item(item_name: String, disable: bool) -> void:
	#print("TabList: add_item[%s: %s]" % [item_name, disable])
	var scene_ins = scene_template.instantiate()
	scene_ins.item_name = item_name
	scene_ins.connect("button_up_call", _on_item_select)
	scene_ins.connect("button_move_up_call", _on_item_move_up)
	scene_ins.connect("button_move_down_call", _on_item_move_down)
	scene_ins.connect("button_double_clicked", _on_item_edit)
	scene_ins.connect("check_button_toggled_switch", _on_item_check)
	tab_item_list_node.add_child(scene_ins)
	tab_item_list.set(item_name, tab_item_list_node.get_children().size() - 1)
	scene_ins.disable = disable
	current_item = -1
	update_index()
	list_item_change.emit(get_item_list_dict())

func remove_item(item_name: String, save_data: bool = true) -> void:
	#print("TabList: remove_item[%s]" % [item_name])
	var index = tab_item_list.get(item_name, -1)
	if index == -1:
		print("TabList: remove_item: 移除[%s]失败! 该节点已不存在!" % [item_name])
		return
	var item = tab_item_list_node.get_children()[index]
	tab_item_list_node.remove_child(item)
	item.queue_free()
	tab_item_list_node.queue_sort()  # 触发重新布局
	tab_item_list_node.queue_redraw()  # 强制重绘
	tab_item_list.erase(item_name)
	current_name = ""
	update_index()
	if save_data: list_item_change.emit(get_item_list_dict())

func select_item(key: String = "") -> void:
	#print("TabList: select[%s]" % [key])
	update_index()
	current_name = key
	var index = tab_item_list.get(key, -1)
	if key.is_empty():
		index = -1
	current_item = index
	for page in tab_item_list_node.get_children():
		if page.get_index() != index:
			page.button_node.button_pressed = false
			page.reset_backgroud()
		else:
			page.button_node.button_pressed = true
			page.set_backgroud()
			#print("[%s]button_pressed = true" % index)

func _on_button_add_button_up() -> void:
	#print("TabList: add")
	list_item_add.emit(title_name)
	current_item = -1
	
func _on_button_del_button_up() -> void:
	if current_item == -1: return
	var child_node = tab_item_list_node.get_children()[current_item]
	var name = child_node.item_name
	#print("TabList: del[%s]" % [name])
	list_item_del.emit(name)
	remove_item(name)
	current_item = -1

func _on_item_select(item_name) -> void:
	select_item(item_name)
	list_item_select.emit(item_name)

func _on_item_edit() -> void:
	list_item_edit.emit(current_name)

func _on_item_check(item_name, toggled_on) -> void:
	list_item_check.emit(item_name, toggled_on)

func _on_item_move_up(item_name) -> void:
	var current_index = tab_item_list.get(item_name, -1)
	if current_index > 0:
		tab_item_list_node.move_child(tab_item_list_node.get_children().get(current_index), current_index - 1)
		update_index()
		list_item_move_up.emit(item_name)
	
func _on_item_move_down(item_name) -> void:
	var current_index = tab_item_list.get(item_name, -1)
	if current_index < tab_item_list_node.get_children().size() - 1:
		tab_item_list_node.move_child(tab_item_list_node.get_children().get(current_index), current_index + 1)
		update_index()
		list_item_move_down.emit(item_name)
