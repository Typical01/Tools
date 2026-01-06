extends Node



var game_config = {}
#var shell_execute_auto_start_config: Dictionary = {} ## ShellExecuteStart
#var shell_execute_menu_button_config: Dictionary = {} ## ShellExecuteMenu
var handle


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#data_manage.data_file = create_config()
	#data_manage.save_data()
	load_config()
	pass # Replace with function body.

func _exit_tree() -> void:
	data_manage.data_file = game_config
	data_manage.save_data()
	print("Global: 程序关闭, 保存数据!")
	pass

func _notification(what):
	# 检查通知是否为“窗口管理器关闭请求”
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		# 2. 调用你的数据保存逻辑
		data_manage.data_file = game_config
		data_manage.save_data()
		print("Global: 窗口关闭, 保存数据!")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func load_config():
	if data_manage.data_file.is_empty():
		data_manage.data_file = create_config()
		data_manage.save_data()
		print("Global: 创建设置.")
	else:
		print("Global: 获取设置.")
	game_config = data_manage.data_file
	
	#初始化： 设置
	#auto_start_setting()
	
	return
	#初始化： ShellExecuteStart
	var start: Dictionary = game_config.get("ShellExecuteStart", {})
	for key in start:
		var obj = ShellExecuteGD.new()
		obj.operate_name = key # key
		var data = start.get(key, {})
		if data.is_empty():
			print("ShellExecuteMenu: [%s] data为空!" % obj.operate_name)
		obj.file = data.get("file", "null")
		obj.arg = data.get("arg", {})
		obj.program_switch = data.get("program_switch", "null")
		obj.shell_operate = data.get("shell_operate", "null")
		obj.window_show = data.get("window_show", false)
		obj.menu_button = data.get("menu_button", false)
		obj.disable = data.get("disable", false)
	
	#初始化： ShellExecuteMenu
	var menu: Dictionary = game_config.get("ShellExecuteMenu", {})
	for key in menu:
		var obj = ShellExecuteGD.new()
		obj.operate_name = key # key
		var data = menu.get(key, {})
		if data.is_empty():
			print("ShellExecuteMenu: [%s] data为空!" % obj.operate_name)
		obj.file = data.get("file", "null")
		obj.arg = data.get("arg", {})
		obj.program_switch = data.get("program_switch", "null")
		obj.shell_operate = data.get("shell_operate", "null")
		obj.window_show = data.get("window_show", false)
		obj.menu_button = data.get("menu_button", false)
		obj.disable = data.get("disable", false)

func create_config() -> Dictionary:
	var config = {}
	config.set("ShellExecuteStart", {})
	config.set("ShellExecuteMenu", {})
	
	var setting = {}
	setting.set("开机自启动", false)
	setting.set("设置分辨率索引", 0)
	setting.set("还原分辨率索引", 0)
	
	var note_book_key_on = ShellExecuteGD.new()
	note_book_key_on.operate_name = "笔记本键盘[开]"
	note_book_key_on.file = "cmd.exe"
	note_book_key_on.arg = { "sc config i8042prt start = auto": false }
	note_book_key_on.program_switch = "/k"
	note_book_key_on.shell_operate = "runas"
	note_book_key_on.window_show = true
	note_book_key_on.menu_button = true
	config.get("ShellExecuteMenu", {}).set(
		note_book_key_on.operate_name, note_book_key_on.make_dict())
	
	var note_book_key_off = ShellExecuteGD.new()
	note_book_key_off.operate_name = "笔记本键盘[关]"
	note_book_key_off.file = "cmd.exe"
	note_book_key_off.arg = { "sc config i8042prt start = disabled": false }
	note_book_key_off.program_switch = "/k"
	note_book_key_off.shell_operate = "runas"
	note_book_key_off.window_show = true
	note_book_key_off.menu_button = true
	config.get("ShellExecuteMenu", {}).set(
		note_book_key_off.operate_name, note_book_key_off.make_dict())
	
	var ping_baidu = ShellExecuteGD.new()
	ping_baidu.operate_name = "Ping[baidu]"
	ping_baidu.file = "cmd.exe"
	ping_baidu.arg = { "ping www.baidu.com": false }
	ping_baidu.program_switch = "/k"
	ping_baidu.shell_operate = "open"
	ping_baidu.window_show = true
	ping_baidu.menu_button = true
	config.get("ShellExecuteMenu", {}).set(
		ping_baidu.operate_name, ping_baidu.make_dict())
	
	config.set("设置", setting)
	return config

func auto_start_setting() -> void:
	#%TrayIcon.set_self_starting()
	pass

func split_shell_commands(cmd: String, split: String = "&&") -> Array:
	var list: Array = []
	var parts = cmd.split(split, false)
	for p in parts:
		var trimmed = p.strip_edges()
		if trimmed != "":
			list.append(trimmed)
	return list

func join_shell_commands(list: Array, sep: String = " && ") -> String:
	var psa = PackedStringArray(list)
	return sep.join(psa)
