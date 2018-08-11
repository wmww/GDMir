extends Node

var gdmir

func _ready():
	gdmir = GDMir.new()

func _process(delta):
	gdmir.add(delta * 1000)
	print(gdmir.get_total() / 1000.0)
