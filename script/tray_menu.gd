extends PopupMenu


signal set_window_from_taskbar

@export var tray_icon: TrayIcon

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_MINIMIZED)
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _on_index_pressed(index: int) -> void:
	#print("index: %s" % index)
	var name = get_item_text(index)
	
	if name == "设置":
		set_window_from_taskbar.emit()
	
	if name == "修改分辨率":
		set_item_checked(index, true)
		set_item_text(index, "还原分辨率")
		tray_icon.set_resolutions(tray_icon.get_resolutions_index())
	elif name == "还原分辨率":
		set_item_checked(index, false)
		set_item_text(index, "修改分辨率")
		tray_icon.set_resolutions(tray_icon.get_restore_resolutions_index())
	print("name: %s" % name)
	
	#ShellExecute: id == 3
	if get_item_id(index) != 3:
		return
	var shell = ShellExecuteGD.new()
	shell.dict_to_shell_execute(name, Global.game_config.get("ShellExecuteMenu", {}).get(name))
	if !shell.is_empty():
		tray_icon.execute_analyze(shell)
	pass # Replace with function body.

func _on_id_pressed(id: int) -> void:
	#print("id: %s" % id)
	if id == 0:
		get_tree().quit()
	pass # Replace with function body.
