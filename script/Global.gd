extends Node



var game_config = {}
var handle


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#data_manage.data_file = create_config()
	#data_manage.save_data_json()
	load_config()
	pass # Replace with function body.

func _exit_tree() -> void:
	save_data()
	print("Global: 程序关闭, 保存数据!")
	pass

func _notification(what):
	# 检查通知是否为“窗口管理器关闭请求”
	if what == NOTIFICATION_WM_CLOSE_REQUEST:
		# 2. 调用你的数据保存逻辑
		save_data()
		print("Global: 窗口关闭, 保存数据!")

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass

func save_data():
	data_manage.data_file = game_config
	data_manage.save_data_json()
	#data_manage.save_data_json("user://game_backup.json")

func load_config():
	if data_manage.data_file.is_empty():
		data_manage.data_file = create_config()
		data_manage.save_data_json()
		print("Global: 创建设置.")
	else:
		print("Global: 获取设置.")
	game_config = data_manage.data_file
	
	#初始化： 设置
	#auto_start_setting()

func create_config() -> Dictionary:
	var config = {}
	
	var setting = {}
	setting.set("开机自启动", false)
	setting.set("设置分辨率索引", 0)
	setting.set("还原分辨率索引", 0)
	
	var shell_menu_list = []
	var shell_menu = {}
	var note_book_key_on = ShellExecuteGD.new()
	note_book_key_on.operate_name = "笔记本键盘[开]"
	note_book_key_on.file = "cmd.exe"
	note_book_key_on.arg = { "sc config i8042prt start = auto": false }
	note_book_key_on.program_switch = "/k"
	note_book_key_on.shell_operate = "runas"
	note_book_key_on.window_show = true
	note_book_key_on.menu_button = true
	shell_menu_list.append(note_book_key_on.operate_name)
	shell_menu.set(note_book_key_on.operate_name, note_book_key_on.make_dict())
	
	var note_book_key_off = ShellExecuteGD.new()
	note_book_key_off.operate_name = "笔记本键盘[关]"
	note_book_key_off.file = "cmd.exe"
	note_book_key_off.arg = { "sc config i8042prt start = disabled": false }
	note_book_key_off.program_switch = "/k"
	note_book_key_off.shell_operate = "runas"
	note_book_key_off.window_show = true
	note_book_key_off.menu_button = true
	shell_menu_list.append(note_book_key_off.operate_name)
	shell_menu.set(note_book_key_off.operate_name, note_book_key_off.make_dict())
	
	var ping_baidu = ShellExecuteGD.new()
	ping_baidu.operate_name = "Ping[baidu]"
	ping_baidu.file = "cmd.exe"
	ping_baidu.arg = { "ping www.baidu.com": false }
	ping_baidu.program_switch = "/k"
	ping_baidu.shell_operate = "open"
	ping_baidu.window_show = true
	ping_baidu.menu_button = true
	shell_menu_list.append(ping_baidu.operate_name)
	shell_menu.set(ping_baidu.operate_name, ping_baidu.make_dict())
	
	config.set("ShellExecuteStart", {})
	config.set("ShellExecuteStartList", [])
	config.set("ShellExecuteMenu", shell_menu)
	config.set("ShellExecuteMenuList", shell_menu_list)
	config.set("设置", setting)
	return config

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
