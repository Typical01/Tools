# res://script/log.gd
extends Node

class_name CustomLoggers

var log_impl

func _ready() -> void:
	# 创建 Logger 实例
	log_impl = CustomLoggersImpl.new()
	# 注册到引擎
	OS.add_logger(log_impl)
