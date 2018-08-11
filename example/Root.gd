extends Node

var gdmir

func _ready():
	gdmir = GDMir.new()
	gdmir.start_mir_server()

func _process(delta):
	gdmir.add(delta * 1000)
	print(gdmir.get_total() / 1000.0)
