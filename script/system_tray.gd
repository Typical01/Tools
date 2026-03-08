class_name SystemTrayNode extends StatusIndicator



var current_name: String
var current_arg_name: String
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
	
	%TrayIcon.get_supported_resolutions(0)
	%General.size.x = \
		%SettingsWindow.size.x - %TabItem.custom_minimum_size.x - 20
	#print(%General.size.x)
	init_tray_menu(true)
	%TabItem.select(0)
	%TabItem.item_selected.emit(0)
	init_settings_page()
	_shell_execute_init_list_item()
	%PopupTips.call_deferred("update_position", %SettingsWindow)
	
	%TrayIcon.hide_window_from_taskbar(Global.handle, true)

func _process(_delta: float) -> void:
	pass

func _notification(what):
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		print("SystemTray: 隐藏[设置]窗口.")
		%TrayIcon.hide_window_from_taskbar(Global.handle, true)





### ----------------------------------------------------
### ShellExecuteListItem





func _shell_execute_init_list_item() -> void:
	%ShellExecuteItem.clear_item_list_node()
	for key in Global.game_config.get(get_shell_execute_list_config(), []):
		var key_data = Global.game_config.get(get_shell_execute_config(), {}).get(key, {})
		#print("init_list_item: %s-%s" % [key, key_data])
		if key_data != null:
			var disable = key_data.get("disable", false)
			%ShellExecuteItem.add_item(key, disable)
		else:
			print("init_list_item: %s 为空!" % [key])

func _on_shell_execute_list_item_add(title_name: String) -> void:
	current_name = ""
	%EditDialog.title_name = "编辑[%s]" % [title_name]
	%EditDialog.item_name = "新项" + str(%ShellExecuteItem.tab_item_list.size())
	%EditDialog.popup_centered()
	%EditDialog.grab_focus()

func _on_shell_execute_list_item_check(_item_name: String, _toggled_on: Variant) -> void:
	Global.game_config.get(get_shell_execute_config(), {}).get(_item_name, {}).set("disable", !_toggled_on)
	save_data()

func _on_shell_execute_list_item_move(_item_name: String) -> void:
	Global.game_config.set(get_shell_execute_list_config(), %ShellExecuteItem.get_item_list_name())
	save_data()

func _on_shell_execute_list_item_del(item_name: String) -> void:
	%ShellExecuteItem.remove_item(item_name)
	current_name = ""
	%ItemEditPanel.visible = false
	print("SystemTray: shell_execute: %s: 移除[%s]!" % [get_shell_execute_config(), item_name])
	if !Global.game_config.get(get_shell_execute_config(), {}).erase(item_name):
		print("SystemTray: shell_execute: 移除[%s]失败!" % [item_name])
		return
	Global.game_config.get(get_shell_execute_list_config(), []).erase(item_name)
	save_data()

func _on_shell_execute_list_item_edit(item_name: String) -> void:
	%EditDialog.title_name = "编辑[%s]" % [get_title_name()]
	%EditDialog.item_name = item_name
	%EditDialog.popup_centered()
	%EditDialog.grab_focus()

func _on_shell_execute_list_item_select(item_name: String) -> void:
	if item_name == current_name: return
	#%ShellExecuteItem.select_item()
	current_name = item_name
	print("current_name[%s]: %s" % [get_shell_execute_config(), current_name])
	current_arg_name = ""
	%Arg.clear_item_list_node()
	var item = Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {})
	%File.text = item.get("file", "cmd.exe")
	%ShellOperate.selected = parse_shell_operate(item.get("shell_operate", "open"))
	%ProgramSwitch.text = item.get("program_switch", "/c")
	var args = item.get("arg", {})
	#print("args: ", args)
	var arg_list = item.get("arg_list", [])
	#print("arg_list: ", arg_list)
	for arg in arg_list:
		%Arg.add_item(arg, args.get(arg, false))
	%ItemEditPanel.visible = true

func _on_start_item_button_button_up() -> void:
	if !menu_item: return
	menu_item = false
	print("switch: StartItem")
	%ItemEditPanel.visible = false
	switch_shell_execute_item()

func _on_menu_item_button_button_up() -> void:
	if menu_item: return
	menu_item = true
	print("switch: MenuItem")
	%ItemEditPanel.visible = false
	switch_shell_execute_item()
	
func switch_shell_execute_item() -> void:
	%ShellExecuteItem.title_name = get_title_name()
	%ShellExecuteItem.title_info = get_title_info()
	current_arg_name = ""
	current_name = ""
	_shell_execute_init_list_item()





### --------------------------------------------------
### ArgsListItem





func _on_arg_list_item_add(title_name: String) -> void:
	current_arg_name = ""
	%EditDialog.title_name = "编辑[%s]" % [title_name]
	%EditDialog.item_name = "新项" + str(%Arg.tab_item_list_node.get_children().size())
	%EditDialog.popup_centered()
	%EditDialog.grab_focus()
	
func _on_arg_list_item_move(_item_name: Variant) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("arg_list", %Arg.get_item_list_name())
		print(Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).get("arg_list"))
		save_data()
	
func _on_arg_list_item_check(item_name: Variant, toggled_on: Variant) -> void:
	Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).get("arg", {}).set(item_name, !toggled_on)
	save_data()
	
func _on_arg_list_item_del(item_name: Variant) -> void:
	%Arg.remove_item(item_name)
	current_arg_name = ""
	var config = Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {})
	if !config.get("arg", {}).erase(item_name):
		print("SystemTray: arg: 移除[%s]失败!" % [item_name])
		return
	config.get("arg_list", {}).erase(item_name)
	save_data()

func _on_arg_list_item_edit(item_name: Variant) -> void:
	%EditDialog.title_name = "编辑[参数]"
	%EditDialog.item_name = item_name
	%EditDialog.popup_centered()
	%EditDialog.grab_focus()

func _on_arg_list_item_select(item_name: Variant) -> void:
	current_arg_name = item_name





### --------------------------------------------------
### 操作名/文件路径/显示窗口/程序开关/编辑对话框/文件对话框





func _on_edit_dialog_edit_accept(new_text: Variant) -> void:
	if %EditDialog.title_name != "编辑[参数]":
		if current_name.is_empty():
			var shell = ShellExecuteGD.new()
			shell.menu_button = menu_item
			shell.operate_name = new_text
			%ShellExecuteItem.add_item(new_text, shell.disable)
			%ShellExecuteItem.select_item(new_text)
			
			Global.game_config.get(get_shell_execute_config(), {}).set(new_text, shell.make_dict())
			Global.game_config.set(get_shell_execute_list_config(), %ShellExecuteItem.get_item_list_name())
			
			_on_shell_execute_list_item_select(new_text)
			print("SystemTrayNode: EditDialog: ShellExecute Add: %s" % [new_text])
		else:
			if Global.game_config.get(get_shell_execute_config(), {}).has(current_name):
				# 备份旧数据
				var old_data = Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {})
				var index = %ShellExecuteItem.get_item_list_index(current_name)
				%ShellExecuteItem.remove_item(current_name)
				%ShellExecuteItem.add_item(new_text, old_data.get("disable", false))
				%ShellExecuteItem.move_item(new_text, index)
				%ShellExecuteItem.select_item(new_text)
				
				Global.game_config.get(get_shell_execute_config(), {}).erase(current_name)
				Global.game_config.get(get_shell_execute_config(), {}).set(new_text, old_data)
				Global.game_config.set(get_shell_execute_list_config(), %ShellExecuteItem.get_item_list_name())
				_on_shell_execute_list_item_select(new_text)
				print("SystemTrayNode: EditDialog: ShellExecute Set: %s" % [new_text])
			else:
				print("SystemTrayNode: EditDialog: ShellExecute Set: %s: 字典中没有: %s" % [get_shell_execute_config(), current_name])
	else:
		if !current_name.is_empty():
			if current_arg_name.is_empty():
				%Arg.add_item(new_text, false)
				%Arg.select_item(new_text)
				
				var arg = Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).get("arg", {})
				arg.set(new_text, false)
				Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("arg", arg)
				Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("arg_list", %Arg.get_item_list_name())
				
				_on_arg_list_item_select(new_text)
				print("SystemTrayNode: EditDialog: Arg Add: %s" % [new_text])
			else:
				var arg = Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).get("arg", {})
				if arg.has(current_arg_name):
					# 备份旧数据
					var old_data = arg[current_arg_name]
					print("SystemTrayNode: EditDialog: Arg Add: arg old_data: %s - %s" % [current_arg_name, old_data])
					var index = %Arg.get_item_list_index(current_arg_name)
					%Arg.remove_item(current_arg_name)
					%Arg.add_item(new_text, old_data)
					%Arg.move_item(new_text, index)
					%Arg.select_item(new_text)
					
					arg.erase(current_arg_name)
					arg.set(new_text, old_data)
					Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("arg", arg)
					Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("arg_list", %Arg.get_item_list_name())
					
					_on_arg_list_item_select(new_text)
					print("EditDialog: EditDialog: Arg Set: %s" % [new_text])
				else:
					print("SystemTrayNode: EditDialog: 字典中没有: %s" % [current_name])
	save_data()

func _on_file_text_changed() -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("file", %File.text)
		save_data()

func _on_shell_operate_item_selected_index(index: int) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("shell_operate", shell_operate_to_string(index))
		save_data()
	
func _on_window_show_toggled_switch(_index: int) -> void:
	if !current_name.is_empty():
		Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("window_show", %WindowShow.button_pressed)
		save_data()

func _on_program_switch_editing_toggled(toggled_on: bool) -> void:
	if toggled_on:
		pass
	else:
		if !current_name.is_empty():
			Global.game_config.get(get_shell_execute_config(), {}).get(current_name, {}).set("program_switch", %ProgramSwitch.text)
			save_data()

func _on_select_file_button_up() -> void:
	%FileDialog.popup_centered()
	%EditDialog.grab_focus()
	
func _on_file_dialog_canceled() -> void:
	%FileDialog.hide()

func _on_file_dialog_dir_selected(dir: String) -> void:
	%File.text = dir

func _on_file_dialog_file_selected(path: String) -> void:
	%File.text = path

func _on_file_dialog_confirmed() -> void:
	_on_file_text_changed()





### --------------------------------------------------
### Setting





func auto_start_settings() -> void:
	#开机自启动
	var task_name = "AutoStart_TypicalTools"
	var auto_start = Global.game_config.get("设置", {}).get("开机自启动", false)
	var exe_path = OS.get_executable_path().replace("/", "\\")
	var tips = "设置: 开机自启动[%s]" % [auto_start]
	var task
	if auto_start:
		task = "schtasks /create /tn \"%s\" /tr \"%s\" /sc onlogon /f" % [task_name, exe_path]
	else:
		task = "schtasks /delete /tn \"%s\" /f" % [task_name]
	
	var auto_start_shell = ShellExecuteGD.new()
	auto_start_shell.operate_name = tips
	auto_start_shell.file = "cmd.exe"
	auto_start_shell.arg = { task: true }
	auto_start_shell.program_switch = "/c"
	auto_start_shell.shell_operate = "runas"
	auto_start_shell.window_show = false
	auto_start_shell.menu_button = false
	
	if !%TrayIcon.execute_analyze(auto_start_shell, false):
		var dialog := AcceptDialog.new()
		dialog.title = "警告"
		dialog.dialog_text = tips + " 失败!"
		get_tree().root.add_child.call_deferred(dialog)
		await dialog.tree_entered
		dialog.popup_centered()
		dialog.close_requested.connect(queue_free)

func save_data() -> void:
	%PopupTips._on_show_tips(true, %SettingsWindow)
	data_manage.data_file = Global.game_config
	Global.save_data()
	init_tray_menu()

func init_tray_menu(init: bool = false) -> void:
	if init:
		#启动项
		for key in Global.game_config.get("ShellExecuteStartList", []):
			var shell = ShellExecuteGD.new()
			shell.dict_to_shell_execute(key, Global.game_config.get("ShellExecuteStart", {}).get(key))
			shell.to_string()
			if shell.is_empty():
				print("SystemTray: 启动项为空! [%s]" % key)
				shell.to_string()
				continue
			else:
				if !shell.disable:
					%TrayIcon.execute_analyze(shell, true)
	
	%TrayMenu.clear()
	#菜单项
	for key in Global.game_config.get("ShellExecuteMenuList", []):
		#print("ShellExecuteMenu Add: ", key)
		var shell = ShellExecuteGD.new()
		var data = Global.game_config.get("ShellExecuteMenu", {}).get(key, {})
		if data != null:
			shell.dict_to_shell_execute(key, data)
		else:
			shell.dict_to_shell_execute(key, {})
		#shell.to_string()
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
	auto_starting_button.button_pressed = Global.game_config.get("设置", {}).get("开机自启动", false)
	auto_starting_button.text_toggled()
	auto_starting_button.toggled_switch.connect(_on_check_button_switch_toggled_switch)
	
	var set_resolution_button = temp_tab_item_list_node.get(1).control_interface_node.get(0)
	set_resolution_button.init_option_list(%TrayIcon.get_resolutions_list())
	set_resolution_button.selected = Global.game_config.get("设置", {}).get("修改分辨率", 0)
	set_resolution_button.item_selected_index.connect(_on_option_button_set_resolutions_item_selected_index)
	
	var restore_resolution_button = temp_tab_item_list_node.get(2).control_interface_node.get(0)
	restore_resolution_button.init_option_list(%TrayIcon.get_resolutions_list())
	restore_resolution_button.selected = Global.game_config.get("设置", {}).get("还原分辨率", 0)
	restore_resolution_button.item_selected_index.connect(_on_option_button_restore_resolutions_item_selected_index)
	
func _on_tray_menu_set_window_from_taskbar() -> void:
	%TrayIcon.hide_window_from_taskbar(Global.handle, false)

func _on_tab_item_item_selected(index: int) -> void:
	#print("SystemTray: 选择: %s" % [index])
	%TabItemPage.get_children()[index].visible = true
	for page in %TabItemPage.get_children():
		if page.get_index() != index:
			page.visible = false

func _on_option_button_set_resolutions_item_selected_index(index: int) -> void:
	Global.game_config.get("设置", {}).set("修改分辨率", index)
	#print("SystemTray: 修改设置[修改分辨率]: %s" % index)
	%TrayIcon.set_resolutions_index(index)
	save_data()

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

func parse_shell_operate(option: String) -> int:
	var lower_str = option.to_lower()  # 不区分大小写
	if lower_str == "打开文件" or lower_str == "open":
		return 0
	elif lower_str == "打开文件夹" or lower_str == "explore":
		return 1
	elif lower_str == "管理员运行" or lower_str == "runas":
		return 2
	return -1  # 未知值
	
### "open"/"explore"/"runas"
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

### "ShellExecuteMenu"/"ShellExecuteStart"
func get_shell_execute_config() -> String:
	if menu_item:
		return "ShellExecuteMenu"
	else:
		return "ShellExecuteStart"

### "ShellExecuteMenuList"/"ShellExecuteStartList"
func get_shell_execute_list_config() -> String:
	if menu_item:
		return "ShellExecuteMenuList"
	else:
		return "ShellExecuteStartList"

### "菜单项"/"启动项"
func get_title_name() -> String:
	if menu_item:
		return "菜单项"
	else:
		return "启动项"
		
### "添加到托盘菜单作为选项"/"程序启动时运行"
func get_title_info() -> String:
	if menu_item:
		return "添加到托盘菜单作为选项"
	else:
		return "程序启动时运行"
