"""File-like objects that read from or write to a string buffer.

This implements (nearly) all stdio methods.

f = StringIO()      # ready for writing
f = StringIO(buf)   # ready for reading
f.close()           # explicitly release resources held
flag = f.isatty()   # always false
pos = f.tell()      # get current position
f.seek(pos)         # set current position
f.seek(pos, mode)   # mode 0: absolute; 1: relative; 2: relative to EOF
buf = f.read()      # read until EOF
buf = f.read(n)     # read up to n bytes
buf = f.readline()  # read until end of line ('\n') or EOF
list = f.readlines()# list of f.readline() results until EOF
f.write(buf)        # write at current position
f.writelines(list)  # for line in list: f.write(line)
f.getvalue()        # return whole file's contents as a string

Notes:
- Using a real file is often faster (but less convenient).
- There's also a much faster implementation in C, called cStringIO, but
  it's not subclassable.
- fileno() is left unimplemented so that code which uses it triggers
  an exception early.
- Seeking far beyond EOF and then writing will insert real null
  bytes that occupy space in the buffer.
- There's a simple test set (see end of this file).
"""

class StringIO:
	def __init__(self, buf = ''):
		self.buf = buf
		self.len = len(buf)
		self.buflist = []
		self.pos = 0
		self.closed = 0
		self.softspace = 0
	def close(self):
		if not self.closed:
			self.closed = 1
			del self.buf, self.pos
	def isatty(self):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		return 0
	def seek(self, pos, mode = 0):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		if self.buflist:
			self.buf = self.buf + ''.joinfields(self.buflist)
			self.buflist = []
		if mode == 1:
			pos = pos + self.pos
		elif mode == 2:
			pos = pos + self.len
		self.pos = max(0, pos)
	def tell(self):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		return self.pos
	def read(self, n = -1):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		if self.buflist:
			self.buf = self.buf + ''.joinfields(self.buflist)
			self.buflist = []
		if n < 0:
			newpos = self.len
		else:
			newpos = min(self.pos+n, self.len)
		r = self.buf[self.pos:newpos]
		self.pos = newpos
		return r
	def readline(self, length=None):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		if self.buflist:
			self.buf = self.buf + ''.joinfields(self.buflist)
			self.buflist = []
		i = self.buf.find('\n', self.pos)
		if i < 0:
			newpos = self.len
		else:
			newpos = i+1
		if length is not None:
			if self.pos + length < newpos:
				newpos = self.pos + length
		r = self.buf[self.pos:newpos]
		self.pos = newpos
		return r
	def readlines(self):
		lines = []
		line = self.readline()
		while line:
			lines.append(line)
			line = self.readline()
		return lines
	def write(self, s):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
		if not s: return
		if self.pos > self.len:
			self.buflist.append('\0'*(self.pos - self.len))
			self.len = self.pos
		newpos = self.pos + len(s)
		if self.pos < self.len:
			if self.buflist:
				self.buf = self.buf + ''.joinfields(self.buflist)
				self.buflist = []
			self.buflist = [self.buf[:self.pos], s, self.buf[newpos:]]
			self.buf = ''
		else:
			self.buflist.append(s)
			self.len = newpos
		self.pos = newpos
	def writelines(self, list):
		self.write(''.joinfields(list))
	def flush(self):
		if self.closed:
			raise ValueError, "I/O operation on closed file"
	def getvalue(self):
		if self.buflist:
			self.buf = self.buf + ''.joinfields(self.buflist)
			self.buflist = []
		return self.buf


