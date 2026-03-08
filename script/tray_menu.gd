extends PopupMenu


signal set_window_from_taskbar

@export var tray_icon: TrayIcon

func _ready() -> void:
	DisplayServer.window_set_mode(DisplayServer.WINDOW_MODE_MINIMIZED)

func _process(_delta: float) -> void:
	pass

func _on_index_pressed(index: int) -> void:
	#print("index: %s" % index)
	var tray_item_name = get_item_text(index)
	
	if tray_item_name == "设置":
		set_window_from_taskbar.emit()
	
	if tray_item_name == "修改分辨率":
		set_item_checked(index, true)
		set_item_text(index, "还原分辨率")
		tray_icon.set_resolutions(tray_icon.get_resolutions_index())
	elif tray_item_name == "还原分辨率":
		set_item_checked(index, false)
		set_item_text(index, "修改分辨率")
		tray_icon.set_resolutions(tray_icon.get_restore_resolutions_index())
	print("TrayMenu: tray_item_name[%s]" % tray_item_name)
	
	#ShellExecute: id == 3
	if get_item_id(index) != 3:
		return
	var shell = ShellExecuteGD.new()
	shell.dict_to_shell_execute(tray_item_name, Global.game_config.get("ShellExecuteMenu", {}).get(tray_item_name))
	#shell.to_string()
	if !shell.is_empty():
		tray_icon.execute_analyze(shell, true)

func _on_id_pressed(id: int) -> void:
	#print("id: %s" % id)
	if id == 0:
		get_tree().quit()
