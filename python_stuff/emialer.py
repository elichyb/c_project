from smtplib import SMTP

class email(object):
	def __init__(self, recipients = None, data = None, subject = None, from_add=None, timeout = 60, smtp_server = "127.0.0.1"):
		self.recipients = recipients
		self.data = data
		self.subject = subject
		self.from_add = from_add
		self.timeout = timeout
		self.smtp_server = smtp_server

	def set_recipients(self,recipients):
		self.recipients = recipients

	def set_data(self, data):
		self.data = data

	def set_subject(self, subject):
		self.subject = subject

	def set_from_add(self, from_addm):
		self.from_add = from_add

	def set_timeout(self, timeout):
		self.timeout = timeout

	def send_email(self):
		if self.from_add is None:
			from_address = ["elichayb@il.ibm.com"]
		new_recipients = [self.recipients]
		#todo check valid recepients
		return SMTP(self.smtp_server,timeout=self.timeout).sendmail(from_address,
		 															new_recipients,
		  															self.data)

