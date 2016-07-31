#this is the logger!!!!!
import os
from time import gmtime, strftime

class logger():
	def __init__(self, file_path, file_name):
		self.file_path = file_path
		self.file_name = file_name
		self.time = None
		self.file = os.path.join(self.file_path, self.file_name)

"""
	DESCRIPTION- write to log file what the we send to the def

	PARAM:
		status- get ERR or LOG.
		system- get system ot host name
		data- more details about the bug
	
	RETURN- None 
"""
	def write_to_exist_file(self, status = None, system = None, data = None):
		if os.path.isfile(self.file):
			file = open(self.file,"a")
		else:
			file = open(self.file,"w")
		self.get_time()
		#this is how the log raw will look like
		line_interface = "\n{time}\t{status}:\t{system}\t{data}\n"
		line_body = line_interface.format(time = self.time,status = status,system = system,data = data)
		print line_body
		file.write(line_body)
		file.close()

"""
	RETURN- the current time.
"""
	def get_time(self):
		self.time = strftime("%Y-%m-%d %H:%M:%S", gmtime())

"""
	SET- set a hedder in a log file if we want to
"""
	def add_header_to_file(self,st = None):
		if(os.path.isfile(self.file)):
			file = open(self.file,"a")
			file.write("{}\n===================================================\n".format(st))
		else:
			file = open(self.file,"w")
			file.write("{}\n===================================================\n".format(st))
		file.close()
