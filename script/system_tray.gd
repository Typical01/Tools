class_name SystemTrayNode extends StatusIndicator


var current_name: String
var current_arg_name: String
var old_item_name: String
var menu_item = false

func _ready() -> void:
	get_tree().set_auto_accept_quit(false)
	# 1. 获取窗口句柄（关键步骤）
	if DisplayServer.get_name() == "Windows":
		#print("SystemTray: Windows平台")
		Global.handle = DisplayServer.window_get_native_handle(DisplayServer.WINDOW_HANDLE, 0)
	if Global.handle != 0:
		print("SystemTray: 获取到窗口句柄: ", Global.handle)
		# 2. 调用 C++ 扩展
		if %TrayIcon:
			%TrayIcon.hide_window_from_taskbar(Global.handle, true)
	else:
		print("SystemTray: 无法获取窗口句柄")
	
	%TrayIcon.get_supported_resolutions()
	%General.size.x = \
		%SettingsWindow.size.x - %TabItem.custom_minimum_size.x - 20
	#print(%General.size.x)
	init_tray_menu(true)
	#%TrayIcon.hide_window_from_taskbar(Global.handle, false)
	%TabItem.select(1)
	%TabItem.item_selected.emit(1)
	init_settings_page()
	%PopupTips.call_deferred("update_position", %SettingsWindow)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _notification(what):
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		# 在这里处理关闭逻辑，例如弹出确认对话框
		print("SystemTray: 隐藏[设置]窗口.")
		%TrayIcon.hide_window_from_taskbar(Global.handle, true)
		# show_confirm_dialog()   # 根据你的 UI 触发确认
		# 如果要退出，则调用 get_tree().quit()

func auto_start_settings() -> void:
	#开机自启动
	%TrayIcon.set_self_starting("TypicalTools", OS.get_executable_path().replace("/", "\\"), "", Global.game_config.get("设置", {}).get("开机自启动", false))

func parse_shell_operate(option: String) -> int:
	var lower_str = option.to_lower()  # 不区分大小写
	if lower_str == "打开文件" or lower_str == "open":
		return 0
	elif lower_str == "打开文件夹" or lower_str == "explore":
		return 1
	elif lower_str == "管理员运行" or lower_str == "runas":
		return 2
	return -1  # 未知值
	
func shell_operate_to_string(code: int) -> String:
	match code:
		0:
			return "open"
		1:
			return "explore"
		2:
			return "runas"
		_:
			return "open"

func save_data() -> void:
	%PopupTips._on_show_tips(true, %SettingsWindow)
	data_manage.data_file = Global.game_config
	init_tray_menu()

func init_tray_menu(init: bool = false) -> void:
	if init:
		#启动项
		for key in Global.game_config.get("ShellExecuteStart", {}):
			var shell = ShellExecuteGD.new()
			shell.dict_to_shell_execute(key, Global.game_config.get("ShellExecuteStart", {}).get(key))
			if shell.is_empty():
				print("SystemTray: 启动项为空! [%s]" % key)
				shell.to_string()
				continue
			else:
				if !shell.disable:
					%TrayIcon.execute_analyze(shell)
	
	%TrayMenu.clear()
	#菜单项
	for key in Global.game_config.get("ShellExecuteMenu", {}):
		var shell = ShellExecuteGD.new()
		shell.dict_to_shell_execute(key, Global.game_config.get("ShellExecuteMenu", {}).get(key, {}))
		if shell.is_empty():
			print("SystemTray: 菜单项为空! [%s]" % key)
			shell.to_string()
			continue
		else:
			%TrayMenu.add_item(key, 3)
			#print("SystemTray: 添加菜单项: [%s]" % key)
	%TrayMenu.add_separator()
	%TrayMenu.add_check_item("修改分辨率", 2)
	%TrayMenu.add_separator()
	%TrayMenu.add_item("设置", 1)
	%TrayMenu.add_separator()
	%TrayMenu.add_item("退出", 0)

func init_settings_page() -> void:
	# SettingPage
	var temp_tab_item_list_node = %General.tab_item_list_node.get_children()
	var auto_starting_button = temp_tab_item_list_node.get(0).control_interface_node.get(0)
	auto_starting_button.toggled_switch.connect(_on_check_button_switch_toggled_switch)
	auto_starting_button.button_pressed = Global.game_config.get("设置", {}).get("开机自启动", false)
	auto_starting_button.text_toggled()
	auto_start_settings()
	
	var set_resolution_button = temp_tab_item_list_node.get(1).control_interface_node.get(0)
	set_resolution_button.init_option_list(%TrayIcon.get_resolutions_list())
	set_resolution_button.item_selected_index.connect(_on_option_button_set_resolutions_item_selected_index)
	set_resolution_button.selected = Global.game_config.get("设置", {}).get("修改分辨率", 0)
	
	var restore_resolution_button = temp_tab_item_list_node.get(2).control_interface_node.get(0)
	restore_resolution_button.init_option_list(%TrayIcon.get_resolutions_list())
	restore_resolution_button.item_selected_index.connect(_on_option_button_restore_resolutions_item_selected_index)
	restore_resolution_button.selected = Global.game_config.get("设置", {}).get("还原分辨率", 0)

func _on_tray_menu_set_window_from_taskbar() -> void:
	%TrayIcon.hide_window_from_taskbar(Global.handle, false)

func _on_tab_item_item_selected(index: int) -> void:
	#print("SystemTray: 选择: %s" % [index])
	%TabItemPage.get_children()[index].visible = true
	for page in %TabItemPage.get_children():
		if page.get_index() != index:
			page.visible = false

func _on_check_button_switch_toggled_switch(toggled_on: Variant) -> void:
	Global.game_config.get("设置", {}).set("开机自启动", toggled_on)
	auto_start_settings()
	save_data()
	#print("SystemTray: 修改设置[开机自启动]: %s" % toggled_on)
	
func _on_option_button_restore_resolutions_item_selected_index(index: int) -> void:
	Global.game_config.get("设置", {}).set("还原分辨率", index)
	#print("SystemTray: 修改设置[还原分辨率]: %s" % index)
	%TrayIcon.set_restore_resolutions_index(index)
	save_data()
	
func _on_option_button_set_resolutions_item_selected_index(index: int) -> void:
	Global.game_config.get("设置", {}).set("修改分辨率", index)
	#print("SystemTray: 修改设置[修改分辨率]: %s" % index)
	%TrayIcon.set_resolutions_index(index)
	save_data()
	
func _on_menu_item_init_list_item() -> void:
	for key in Global.game_config.get(get_shell_execute_config(true), {}):
		var shell = ShellExecuteGD.new()
		shell.menu_button = true
		shell.dict_to_shell_execute(key, Global.game_config.get(get_shell_execute_config(true), {}).get(key, {}))
		get_shell_execute_list_node(true).add_item(shell.operate_name, shell.disable)

func _on_start_init_list_item() -> void:
	for key in Global.game_config.get(get_shell_execute_config(false), {}):
		var shell = ShellExecuteGD.new()
		shell.menu_button = false
		shell.dict_to_shell_execute(key, Global.game_config.get(get_shell_execute_config(false), {}).get(key, {}))
		get_shell_execute_list_node(false).add_item(shell.operate_name, shell.disable)

func _on_list_item_add(title_name: String) -> void:
	current_name = ""
	if title_name == "菜单项":
		menu_item = true
	else:
		menu_item = false
	%EditDialog.title_name = "编辑[%s]" % [title_name]
	%EditDialog.item_name = "新项" + str(get_shell_execute_list_node(menu_item).tab_item_list_node.get_children().size())
	%EditDialog.show()
	%EditDialog.grab_focus()

func _on_list_item_del(item_name: String) -> void:
	current_name = ""
	%ItemEditPanel.visible = false
	if !Global.game_config.get(get_shell_execute_config(menu_item), {}).erase(item_name):
		print("SystemTray: %s: 移除[%s]失败!" % [get_shell_execute_config(menu_item), item_name])
	save_data()
	
func _on_list_item_select(item_name: String) -> void:
	get_shell_execute_list_node(!menu_item).select_item()
	old_item_name = current_name
	current_name = item_name
	current_arg_name = ""
	%Arg.clear_item_list_node()
	get_shell_execute_list_node(menu_item).current_item = get_shell_execute_list_node(menu_item).tab_item_list.get(item_name, -1)
	%OperateName.text = item_name
	var item = Global.game_config.get(get_shell_execute_config(menu_item), {}).get(item_name, {})
	%File.text = item.get("file", "")
	%ShellOperate.selected = parse_shell_operate(item.get("shell_operate", "open"))
	%ProgramSwitch.text = item.get("program_switch", "/c")
	var args = item.get("arg", {})
	#print("args: ", args)
	for arg in args.keys():
		%Arg.add_item(arg, args.get(arg))
	%ItemEditPanel.visible = true
	
func _on_menu_list_item_select(item_name: String) -> void:
	menu_item = true
	_on_list_item_select(item_name)
	
func _on_start_list_item_select(item_name: String) -> void:
	menu_item = false
	_on_list_item_select(item_name)

func _on_item_list_item_check(item_name, toggled_on: Variant) -> void:
	Global.game_config.get(get_shell_execute_config(menu_item), {}).get(item_name, {}).set("disable", !toggled_on)
	save_data()

func _on_operate_name_editing_toggled(toggled_on: bool) -> void:
	if toggled_on:
		pass
	else:
		if !current_name.is_empty():
			var shell = Global.game_config.get(get_shell_execute_config(menu_item), {})
			if shell.has(current_name):
				# 备份旧数据
				var old_data = shell[current_name]
				shell.erase(current_name)
				shell.set(%OperateName.text, old_data)
				Global.game_config.set(get_shell_execute_config(menu_item), shell)
				get_shell_execute_list_node(menu_item).add_item(%OperateName.text, old_data.get("disable", false))
				get_shell_execute_list_node(menu_item).remove_item(current_name)
				_on_list_item_select(%OperateName.text)
				save_data()
			else:
				print("SystemTrayNode: %s: 字典中没有: %s" % [get_shell_execute_config(menu_item), current_name])

func _on_file_editing_toggled(toggled_on: bool) -> void:
	if toggled_on:
		pass
	else:
		if !current_name.is_empty():
			Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("file", %File.text)
			save_data()

func _on_shell_operate_item_selected_index(index: int) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("shell_operate", shell_operate_to_string(index))
		save_data()
	
func _on_window_show_toggled_switch(index: int) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("window_show", %WindowShow.button_pressed)
		save_data()

func _on_program_switch_editing_toggled(toggled_on: bool) -> void:
	if toggled_on:
		pass
	else:
		if !current_name.is_empty():
			Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("program_switch", %ProgramSwitch.text)
			save_data()

func _on_arg_list_item_change(tab_item_dict: Dictionary) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("arg", tab_item_dict)
		#save_data()

func _on_arg_list_item_add(title_name: String) -> void:
	current_arg_name = ""
	%EditDialog.title_name = "编辑[%s]" % [title_name]
	%EditDialog.item_name = "新项" + str(%Arg.tab_item_list_node.get_children().size())
	%EditDialog.show()
	%EditDialog.grab_focus()
	
func _on_arg_list_item_del(item_name: Variant) -> void:
	current_arg_name = ""
	if !Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).get("arg", {}).erase(item_name):
		print("SystemTray: ArgList: 移除[%s]失败!" % [item_name])
	save_data()

func _on_arg_list_item_select(item_name: Variant) -> void:
	current_arg_name = item_name

func _on_arg_list_item_edit(item_name: Variant) -> void:
	%EditDialog.title_name = "编辑[参数]"
	%EditDialog.item_name = item_name
	%EditDialog.show()
	%EditDialog.grab_focus()
	
func _on_arg_list_item_check(item_name, toggled_on: Variant) -> void:
	Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).get("arg", {}).set(item_name, !toggled_on)
	save_data()

func get_shell_execute_config(menu: bool) -> String:
	if menu:
		return "ShellExecuteMenu"
	else:
		return "ShellExecuteStart"

func get_shell_execute_list_node(menu: bool) -> Node:
	if menu:
		return %MenuItem
	else:
		return %StartItem

func _on_edit_dialog_edit_accept(new_text: Variant) -> void:
	if %EditDialog.title_name != "编辑[参数]":
		var shell = ShellExecuteGD.new()
		shell.menu_button = menu_item
		shell.operate_name = new_text
		get_shell_execute_list_node(menu_item).add_item(new_text, shell.disable)
		get_shell_execute_list_node(menu_item).select_item(new_text)
		Global.game_config.get(get_shell_execute_config(menu_item), {}).set(new_text, shell.make_dict())
		_on_list_item_select(new_text)
	else:
		if !current_name.is_empty():
			if current_arg_name.is_empty():
				var arg = Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).get("arg", {})
				var arg_list = { new_text: false }
				arg.set("arg", arg_list)
				Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set("arg", arg)
				%Arg.add_item(new_text, false)
				%Arg.select_item(new_text)
			else:
				var arg = Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).get("arg", {})
				if arg.has(current_arg_name):
					# 备份旧数据
					var old_data = arg[current_arg_name]
					print("old_data: %s" % old_data)
					arg.erase(current_arg_name)
					arg.set(new_text, old_data)
					%Arg.remove_item(current_arg_name)
					Global.game_config.get(get_shell_execute_config(menu_item), {}).get(current_name, {}).set(new_text, arg)
					%Arg.add_item(new_text, old_data)
				else:
					print("SystemTrayNode: EditDialog: 字典中没有: %s" % [current_name])
	save_data()


func _on_select_file_button_up() -> void:
	%FileDialog.show()
	%EditDialog.grab_focus()
	
func _on_file_dialog_canceled() -> void:
	%FileDialog.hide()

func _on_file_dialog_dir_selected(dir: String) -> void:
	%File.text = dir

func _on_file_dialog_file_selected(path: String) -> void:
	%File.text = path

func _on_file_dialog_confirmed() -> void:
	_on_file_editing_toggled(false)

func _on_item_list_item_move(item_name: Variant) -> void:
	save_data()
