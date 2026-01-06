class_name ShellExecute extends Resource

@export var operate_name: String #操作名
@export var shell_operate: String #Shell操作
@export var file: String #文件
@export var arg: Array[String] #参数
@export var window_show: bool #窗口显示
@export var menu_button: bool #菜单按键

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	
func _init(_operate_name: String, _shell_operate: String, _file: String,
	_arg: Array[String], _window_show: bool = true, _menu_button: bool = true) -> void:
	operate_name = _operate_name
	shell_operate = _shell_operate
	file = _file
	arg = _arg
	window_show = _window_show
	menu_button = _menu_button
