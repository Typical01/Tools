class_name TabListNode extends VBoxContainer



signal list_item_add(title_name)
signal list_item_select(item_name)
signal list_item_edit(item_name)
signal list_item_check(title_name, toggled_on)
signal list_item_change(tab_item_list_data, tab_item_list_name)
signal list_item_del(item_name)
signal list_item_move_up(item_name)
signal list_item_move_down(item_name)


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
var tab_item_list: Array[String] ## 保存节点名
var current_index: int = -1:
	set(index):
		current_index = index
@onready var tab_item_list_node = %TabItemList ##子选项卡节点
var scene_drag_ins
var button_group = ButtonGroup.new()




func _ready() -> void:
	pass

func _process(_delta: float) -> void:
	pass




func clear_item_list_node() -> void:	
	current_name = ""
	current_index = -1
	for child in tab_item_list_node.get_children():
		tab_item_list_node.remove_child(child)
		child.queue_free()
	tab_item_list.clear()

func get_item_list_index(item_name: String) -> int:
	return tab_item_list.find(item_name)

func get_item_list_data() -> Dictionary:
	var item_list_name: Dictionary
	for child in tab_item_list_node.get_children():
		item_list_name.set(child.item_name, child.disable)
	return item_list_name

func get_item_list_name() -> Array:
	return tab_item_list.duplicate()

func add_item(item_name: String, disable: bool) -> void:
	#print("TabList: add_item[%s: %s]" % [item_name, disable])
	var scene_ins = scene_template.instantiate()
	scene_ins.item_name = item_name
	scene_ins.disable = disable
	scene_ins.connect("button_up_call", _on_item_select)
	scene_ins.connect("button_move_up_call", _on_item_move_up)
	scene_ins.connect("button_move_down_call", _on_item_move_down)
	scene_ins.connect("button_double_clicked", _on_item_edit)
	scene_ins.connect("check_button_toggled_switch", _on_item_check)
	tab_item_list_node.add_child(scene_ins)
	scene_ins.button_node.button_group = button_group
	scene_ins.button_node.toggled.connect(_on_item_toggled.bind(scene_ins))
	tab_item_list.append(item_name)
	#print("add_item: ", tab_item_list)
	#current_index = tab_item_list_node.get_children().size() - 1
	#current_name = item_name

func remove_item(item_name: String) -> void:
	#print("TabList: remove_item[%s]" % [item_name])
	%ButtonDel.disabled = true
	var index = get_item_list_index(item_name)
	if index == -1:
		print("TabList: remove_item: 移除[%s]失败! 该节点已不存在!" % [item_name])
		return
	var item = tab_item_list_node.get_children()[index]
	tab_item_list_node.remove_child(item)
	tab_item_list.erase(item_name)
	#print("remove_item: ", tab_item_list)
	item.queue_free()
	tab_item_list_node.queue_sort()  # 触发重新布局
	tab_item_list_node.queue_redraw()  # 强制重绘
	current_name = ""
	current_index = -1

func select_item(item_name: String = "") -> void:
	#print("TabList: select[%s]" % [item_name])
	var index
	if item_name.is_empty():
		index = -1
		return
	index = get_item_list_index(item_name)
	if index == -1:
		print("TabList: select[%s] %s" % [item_name, "tab_item_list 找不到 [%s]!"])
	var current_node = tab_item_list_node.get_children()[index]
	if current_node:
		current_node.button_node.button_pressed = true
	%ButtonDel.disabled = false
	current_name = item_name
	current_index = index

func move_item(item_name: String, item_index: int):
	if item_index < tab_item_list.size() and item_index >= 0:
		#print("move_item: [%s -> %s]" % [item_name, item_index])
		#print("move_item: ", tab_item_list)
		var node = tab_item_list_node.get_children().get(get_item_list_index(item_name))
		if node:
			tab_item_list_node.move_child(node, item_index)
			tab_item_list.erase(item_name)
			tab_item_list.insert(item_index, item_name)
			#print("move_item: ", tab_item_list)
		else:
			print("move_item: [%s -> %s] %s" % [item_name, item_index, "没有该节点!"])

func _on_button_add_button_up() -> void:
	#print("TabList: add")
	list_item_add.emit(title_name)
	
func _on_button_del_button_up() -> void:
	if current_index == -1: return
	var item_name = tab_item_list[current_index]
	#print("TabList: del[%s]" % [child_node.item_name])
	#remove_item(child_node.item_name)
	list_item_del.emit(item_name)

func _on_item_select(item_name) -> void:
	select_item(item_name)
	list_item_select.emit(item_name)

func _on_item_edit() -> void:
	list_item_edit.emit(current_name)

func _on_item_check(item_name, toggled_on) -> void:
	list_item_check.emit(item_name, toggled_on)

func _on_item_move_up(item_name) -> void:
	var index = get_item_list_index(item_name)
	if index > 0:
		move_item(item_name, index - 1)
		current_index -= 1
		list_item_move_up.emit(item_name)

func _on_item_move_down(item_name) -> void:
	var index = get_item_list_index(item_name)
	if index < tab_item_list_node.get_children().size() - 1:
		move_item(item_name, index + 1)
		current_index += 1
		list_item_move_down.emit(item_name)

func _on_item_toggled(toggled_on: bool, item_node):
	item_node.show_move_button(toggled_on)
