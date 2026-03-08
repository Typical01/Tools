class_name EditDialogNode extends Window


signal edit_text_change(new_text)
signal edit_editing_toggled(toggled_on)
signal edit_accept(new_text)
signal edit_cancel()

var old_text
var title_name: String:
	get():
		return title
	set(new_text):
		title = new_text
var item_name: String:
	get():
		return %LineEdit.text
	set(new_text):
		%LineEdit.text = new_text

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	pass
	
func _on_line_edit_editing_toggled(toggled_on: bool) -> void:
	if toggled_on:
		#print("EditDialogNode: LineEdit 进入编辑.")
		old_text = item_name
	edit_editing_toggled.emit(toggled_on)

func _on_line_edit_text_changed(new_text: String) -> void:
	edit_text_change.emit(new_text)

func _on_button_accept_button_up() -> void:
	edit_accept.emit(item_name)
	hide()

func _on_button_cancel_button_up() -> void:
	edit_cancel.emit()
	hide()

func _on_close_requested() -> void:
	hide()


func _on_focus_entered() -> void:
	%LineEdit.grab_focus()
