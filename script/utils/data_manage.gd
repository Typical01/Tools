extends Node
class_name DataManage



var data_file : Dictionary = {}
var is_show_log := true
var save_path = "user://game.dat" :
	set(path):
		save_path = path
		if is_show_log: print("DataManage: 保存路径修改为[%s]!" % save_path)
	get():
		return save_path



func _ready() -> void:
	# 尝试加载；如果不存在就新建
	if not load_data():
		data_file = {}
		save_data()

# 保存
func save_data() -> void:
	if data_file.is_empty():
		data_file = {}
	var file = FileAccess.open(save_path, FileAccess.WRITE)
	if file == null:
		push_error("DataManage: 无法打开文件写入: %s" % save_path)
		return
	file.store_var(data_file)
	file.close()
	if is_show_log: 
		print("DataManage: 保存到文件[%s]!" % save_path)
		print("DataManage: 详细数据:")
		print(data_file)


# 读取
func load_data() -> bool:
	if not FileAccess.file_exists(save_path):
		if is_show_log: push_error("DataManage: 文件[%s]不存在!" % save_path)
		return false
	if is_show_log: print("DataManage: 打开文件[%s]!" % save_path)
	var file = FileAccess.open(save_path, FileAccess.READ)
	if file == null:
		push_error("DataManage: 打开文件失败: %s" % save_path)
		return false
	# get_var() 返回序列化的 Variant（这里通常是 Dictionary）
	var v = file.get_var()  # 若文件里是带对象序列化的，使用 file.get_var(true)
	file.close()
	if typeof(v) == TYPE_DICTIONARY:
		data_file = v
		if is_show_log: 
			print("DataManage: 详细数据:")
			print(data_file)
		return true
	else:
		push_error("DataManage: 读取到的存档不是 Dictionary，已忽略。")
		data_file = {}
		return false
