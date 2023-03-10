"""An NNTP client class based on RFC 977: Network News Transfer Protocol.

Example:

>>> from nntplib import NNTP
>>> s = NNTP('news')
>>> resp, count, first, last, name = s.group('comp.lang.python')
>>> print 'Group', name, 'has', count, 'articles, range', first, 'to', last
Group comp.lang.python has 51 articles, range 5770 to 5821
>>> resp, subs = s.xhdr('subject', first + '-' + last)
>>> resp = s.quit()
>>>

Here 'resp' is the server response line.
Error responses are turned into exceptions.

To post an article from a file:
>>> f = open(filename, 'r') # file containing article, including header
>>> resp = s.post(f)
>>>

For descriptions of all methods, read the comments in the code below.
Note that all arguments and return values representing article numbers
are strings, not numbers, since they are rarely used for calculations.
"""

# RFC 977 by Brian Kantor and Phil Lapsley.
# xover, xgtitle, xpath, date methods by Kevan Heydon


# Imports
import re
import socket
import string



# Exceptions raised when an error or invalid response is received
class NNTPError(Exception):
	"""Base class for all nntplib exceptions"""
	def __init__(self, *args):
		apply(Exception.__init__, (self,)+args)
		try:
			self.response = args[0]
		except IndexError:
			self.response = 'No response given'

class NNTPReplyError(NNTPError):
	"""Unexpected [123]xx reply"""
	pass

class NNTPTemporaryError(NNTPError):
	"""4xx errors"""
	pass

class NNTPPermanentError(NNTPError):
	"""5xx errors"""
	pass

class NNTPProtocolError(NNTPError):
	"""Response does not begin with [1-5]"""
	pass

class NNTPDataError(NNTPError):
	"""Error in response data"""
	pass

# for backwards compatibility
error_reply = NNTPReplyError
error_temp = NNTPTemporaryError
error_perm = NNTPPermanentError
error_proto = NNTPProtocolError
error_data = NNTPDataError



# Standard port used by NNTP servers
NNTP_PORT = 119


# Response numbers that are followed by additional text (e.g. article)
LONGRESP = ['100', '215', '220', '221', '222', '224', '230', '231', '282']


# Line terminators (we always output CRLF, but accept any of CRLF, CR, LF)
CRLF = '\r\n'



# The class itself
class NNTP:
	def __init__(self, host, port=NNTP_PORT, user=None, password=None,
		     readermode=None):
		"""Initialize an instance.  Arguments:
		- host: hostname to connect to
		- port: port to connect to (default the standard NNTP port)
		- user: username to authenticate with
		- password: password to use with username
		- readermode: if true, send 'mode reader' command after
		              connecting.

	        readermode is sometimes necessary if you are connecting to an
	        NNTP server on the local machine and intend to call
	        reader-specific comamnds, such as `group'.  If you get
	        unexpected NNTPPermanentErrors, you might need to set
	        readermode.
		"""
		self.host = host
		self.port = port
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect(self.host, self.port)
		self.file = self.sock.makefile('rb')
		self.debugging = 0
		self.welcome = self.getresp()
		if readermode:
			try:
				self.welcome = self.shortcmd('mode reader')
			except NNTPPermanentError:
				# error 500, probably 'not implemented'
				pass
		if user:
			resp = self.shortcmd('authinfo user '+user)
			if resp[:3] == '381':
				if not password:
					raise NNTPReplyError(resp)
				else:
					resp = self.shortcmd(
						'authinfo pass '+password)
					if resp[:3] != '281':
						raise NNTPPermanentError(resp)

	# Get the welcome message from the server
	# (this is read and squirreled away by __init__()).
	# If the response code is 200, posting is allowed;
	# if it 201, posting is not allowed

	def getwelcome(self):
		"""Get the welcome message from the server
		(this is read and squirreled away by __init__()).
		If the response code is 200, posting is allowed;
		if it 201, posting is not allowed."""

		if self.debugging: print '*welcome*', `self.welcome`
		return self.welcome

	def set_debuglevel(self, level):
		"""Set the debugging level.  Argument 'level' means:
		0: no debugging output (default)
		1: print commands and responses but not body text etc.
		2: also print raw lines read and sent before stripping CR/LF"""

		self.debugging = level
	debug = set_debuglevel

	def putline(self, line):
		"""Internal: send one line to the server, appending CRLF."""
		line = line + CRLF
		if self.debugging > 1: print '*put*', `line`
		self.sock.send(line)

	def putcmd(self, line):
		"""Internal: send one command to the server (through putline())."""
		if self.debugging: print '*cmd*', `line`
		self.putline(line)

	def getline(self):
		"""Internal: return one line from the server, stripping CRLF.
		Raise EOFError if the connection is closed."""
		line = self.file.readline()
		if self.debugging > 1:
			print '*get*', `line`
		if not line: raise EOFError
		if line[-2:] == CRLF: line = line[:-2]
		elif line[-1:] in CRLF: line = line[:-1]
		return line

	def getresp(self):
		"""Internal: get a response from the server.
		Raise various errors if the response indicates an error."""
		resp = self.getline()
		if self.debugging: print '*resp*', `resp`
		c = resp[:1]
		if c == '4':
			raise NNTPTemporaryError(resp)
		if c == '5':
			raise NNTPPermanentError(resp)
		if c not in '123':
			raise NNTPProtocolError(resp)
		return resp

	def getlongresp(self):
		"""Internal: get a response plus following text from the server.
		Raise various errors if the response indicates an error."""
		resp = self.getresp()
		if resp[:3] not in LONGRESP:
			raise NNTPReplyError(resp)
		list = []
		while 1:
			line = self.getline()
			if line == '.':
				break
			if line[:2] == '..':
				line = line[1:]
			list.append(line)
		return resp, list

	def shortcmd(self, line):
		"""Internal: send a command and get the response."""
		self.putcmd(line)
		return self.getresp()

	def longcmd(self, line):
		"""Internal: send a command and get the response plus following text."""
		self.putcmd(line)
		return self.getlongresp()

	def newgroups(self, date, time):
		"""Process a NEWGROUPS command.  Arguments:
		- date: string 'yymmdd' indicating the date
		- time: string 'hhmmss' indicating the time
		Return:
		- resp: server response if succesful
		- list: list of newsgroup names"""

		return self.longcmd('NEWGROUPS ' + date + ' ' + time)

	def newnews(self, group, date, time):
		"""Process a NEWNEWS command.  Arguments:
		- group: group name or '*'
		- date: string 'yymmdd' indicating the date
		- time: string 'hhmmss' indicating the time
		Return:
		- resp: server response if succesful
		- list: list of article ids"""

		cmd = 'NEWNEWS ' + group + ' ' + date + ' ' + time
		return self.longcmd(cmd)

	def list(self):
		"""Process a LIST command.  Return:
		- resp: server response if succesful
		- list: list of (group, last, first, flag) (strings)"""

		resp, list = self.longcmd('LIST')
		for i in range(len(list)):
			# Parse lines into "group last first flag"
			list[i] = tuple(string.split(list[i]))
		return resp, list

	def group(self, name):
		"""Process a GROUP command.  Argument:
		- group: the group name
		Returns:
		- resp: server response if succesful
		- count: number of articles (string)
		- first: first article number (string)
		- last: last article number (string)
		- name: the group name"""

		resp = self.shortcmd('GROUP ' + name)
		if resp[:3] <> '211':
			raise NNTPReplyError(resp)
		words = string.split(resp)
		count = first = last = 0
		n = len(words)
		if n > 1:
			count = words[1]
			if n > 2:
				first = words[2]
				if n > 3:
					last = words[3]
					if n > 4:
						name = string.lower(words[4])
		return resp, count, first, last, name

	def help(self):
		"""Process a HELP command.  Returns:
		- resp: server response if succesful
		- list: list of strings"""

		return self.longcmd('HELP')

	def statparse(self, resp):
		"""Internal: parse the response of a STAT, NEXT or LAST command."""
		if resp[:2] <> '22':
			raise NNTPReplyError(resp)
		words = string.split(resp)
		nr = 0
		id = ''
		n = len(words)
		if n > 1:
			nr = words[1]
			if n > 2:
				id = words[2]
		return resp, nr, id

	def statcmd(self, line):
		"""Internal: process a STAT, NEXT or LAST command."""
		resp = self.shortcmd(line)
		return self.statparse(resp)

	def stat(self, id):
		"""Process a STAT command.  Argument:
		- id: article number or message id
		Returns:
		- resp: server response if succesful
		- nr:   the article number
		- id:   the article id"""

		return self.statcmd('STAT ' + id)

	def next(self):
		"""Process a NEXT command.  No arguments.  Return as for STAT."""
		return self.statcmd('NEXT')

	def last(self):
		"""Process a LAST command.  No arguments.  Return as for STAT."""
		return self.statcmd('LAST')

	def artcmd(self, line):
		"""Internal: process a HEAD, BODY or ARTICLE command."""
		resp, list = self.longcmd(line)
		resp, nr, id = self.statparse(resp)
		return resp, nr, id, list

	def head(self, id):
		"""Process a HEAD command.  Argument:
		- id: article number or message id
		Returns:
		- resp: server response if succesful
		- nr: article number
		- id: message id
		- list: the lines of the article's header"""

		return self.artcmd('HEAD ' + id)

	def body(self, id):
		"""Process a BODY command.  Argument:
		- id: article number or message id
		Returns:
		- resp: server response if succesful
		- nr: article number
		- id: message id
		- list: the lines of the article's body"""

		return self.artcmd('BODY ' + id)

	def article(self, id):
		"""Process an ARTICLE command.  Argument:
		- id: article number or message id
		Returns:
		- resp: server response if succesful
		- nr: article number
		- id: message id
		- list: the lines of the article"""

		return self.artcmd('ARTICLE ' + id)

	def slave(self):
		"""Process a SLAVE command.  Returns:
		- resp: server response if succesful"""

		return self.shortcmd('SLAVE')

	def xhdr(self, hdr, str):
		"""Process an XHDR command (optional server extension).  Arguments:
		- hdr: the header type (e.g. 'subject')
		- str: an article nr, a message id, or a range nr1-nr2
		Returns:
		- resp: server response if succesful
		- list: list of (nr, value) strings"""

		pat = re.compile('^([0-9]+) ?(.*)\n?')
		resp, lines = self.longcmd('XHDR ' + hdr + ' ' + str)
		for i in range(len(lines)):
			line = lines[i]
			m = pat.match(line)
			if m:
				lines[i] = m.group(1, 2)
		return resp, lines

	def xover(self,start,end):
		"""Process an XOVER command (optional server extension) Arguments:
		- start: start of range
		- end: end of range
		Returns:
		- resp: server response if succesful
		- list: list of (art-nr, subject, poster, date,
		                 id, references, size, lines)"""

		resp, lines = self.longcmd('XOVER ' + start + '-' + end)
		xover_lines = []
		for line in lines:
			elem = string.splitfields(line,"\t")
			try:
				xover_lines.append((elem[0],
						    elem[1],
						    elem[2],
						    elem[3],
						    elem[4],
						    string.split(elem[5]),
						    elem[6],
						    elem[7]))
			except IndexError:
				raise NNTPDataError(line)
		return resp,xover_lines

	def xgtitle(self, group):
		"""Process an XGTITLE command (optional server extension) Arguments:
		- group: group name wildcard (i.e. news.*)
		Returns:
		- resp: server response if succesful
		- list: list of (name,title) strings"""

		line_pat = re.compile("^([^ \t]+)[ \t]+(.*)$")
		resp, raw_lines = self.longcmd('XGTITLE ' + group)
		lines = []
		for raw_line in raw_lines:
			match = line_pat.search(string.strip(raw_line))
			if match:
				lines.append(match.group(1, 2))
		return resp, lines

	def xpath(self,id):
		"""Process an XPATH command (optional server extension) Arguments:
		- id: Message id of article
		Returns:
		resp: server response if succesful
		path: directory path to article"""

		resp = self.shortcmd("XPATH " + id)
		if resp[:3] <> '223':
			raise NNTPReplyError(resp)
		try:
			[resp_num, path] = string.split(resp)
		except ValueError:
			raise NNTPReplyError(resp)
		else:
			return resp, path

	def date (self):
		"""Process the DATE command. Arguments:
		None
		Returns:
		resp: server response if succesful
		date: Date suitable for newnews/newgroups commands etc.
		time: Time suitable for newnews/newgroups commands etc."""

		resp = self.shortcmd("DATE")
		if resp[:3] <> '111':
			raise NNTPReplyError(resp)
		elem = string.split(resp)
		if len(elem) != 2:
			raise NNTPDataError(resp)
		date = elem[1][2:8]
		time = elem[1][-6:]
		if len(date) != 6 or len(time) != 6:
			raise NNTPDataError(resp)
		return resp, date, time


	def post(self, f):
		"""Process a POST command.  Arguments:
		- f: file containing the article
		Returns:
		- resp: server response if succesful"""

		resp = self.shortcmd('POST')
		# Raises error_??? if posting is not allowed
		if resp[0] <> '3':
			raise NNTPReplyError(resp)
		while 1:
			line = f.readline()
			if not line:
				break
			if line[-1] == '\n':
				line = line[:-1]
			if line[:1] == '.':
				line = '.' + line
			self.putline(line)
		self.putline('.')
		return self.getresp()

	def ihave(self, id, f):
		"""Process an IHAVE command.  Arguments:
		- id: message-id of the article
		- f:  file containing the article
		Returns:
		- resp: server response if succesful
		Note that if the server refuses the article an exception is raised."""

		resp = self.shortcmd('IHAVE ' + id)
		# Raises error_??? if the server already has it
		if resp[0] <> '3':
			raise NNTPReplyError(resp)
		while 1:
			line = f.readline()
			if not line:
				break
			if line[-1] == '\n':
				line = line[:-1]
			if line[:1] == '.':
				line = '.' + line
			self.putline(line)
		self.putline('.')
		return self.getresp()

	def quit(self):
		"""Process a QUIT command and close the socket.  Returns:
		- resp: server response if succesful"""

		resp = self.shortcmd('QUIT')
		self.file.close()
		self.sock.close()
		del self.file, self.sock
		return resp


def _test():
	"""Minimal test function."""
	s = NNTP('news', readermode='reader')
	resp, count, first, last, name = s.group('comp.lang.python')
	print resp
	print 'Group', name, 'has', count, 'articles, range', first, 'to', last
	resp, subs = s.xhdr('subject', first + '-' + last)
	print resp
	for item in subs:
		print "%7s %s" % item
	resp = s.quit()
	print resp


# Run the test when run as a script
if __name__ == '__main__':
	_test()
