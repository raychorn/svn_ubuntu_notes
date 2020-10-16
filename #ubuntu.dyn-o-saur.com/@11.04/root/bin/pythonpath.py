import sys, os, re

_ignores_ = ['setuptools-0.6c11-py2.5.egg','pip-1.0.2-py2.5.egg']

def parse_exports(ex,target=None):
	_l_ = []
	for x in ex:
		try:
			toks = [[i.split(':') for i in f.split('=')] for f in x.split()]
			#print 'toks=%s' % (toks)
			#key1 = toks[0][0]
			#print 'key1=%s' % (key1)
			#key2 = toks[1][0]
			#print 'key2=%s' % (key2)
			_found = False
			for items in toks[1][1:]:
				for tok in list(set(items)):
					if (target) and (tok == target):
						#print '@@@'
						_found = True
						break
					#print 'tok=%s' % (tok)
			if (not _found):
				toks[1][-1].append(target)
			toks[1][-1] = ':'.join(list(set(toks[1][-1])))
			#print '(+) toks[-1]=%s (%s)' % (toks[-1],type(toks[-1]))
			toks[-1][0] = ''.join(toks[-1][0])
			toks[-1] = '='.join(toks[-1])
			toks[0] = toks[0][0][0]
			#print '(++) toks[0]=%s (%s) (%s)' % (toks[0],type(toks[0]))
			xx = ' '.join(toks)
			#print '(+++) toks=%s' % (toks)
			#print '(+++) xx=%s' % (xx)
			_l_.append(xx)
		except Exception, ex:
			print str(ex)
	return _l_

def readlinesFrom(fname):
	lines = []
	fin = open(fname)
	try:
		for aLine in fin.readlines():
			lines.append(aLine.strip())
	except Exception, ex:
		print str(ex)
	fin.close()
	return lines

def writelinesTo(fname,lines):
	fout = open(fname,'w')
	try:
		fout.writelines(lines)
	except Exception, ex:
		print str(ex)
	fout.flush()
	fout.close()

if (__name__ == '__main__'):
	_re = re.compile(r"(?P<name>.*/.*\.egg)")
	_process_ = lambda f,d:f.replace('.',d) if (f == '.') else f
	_source = '/root/bin/PYTHONPATH'
	_exports = readlinesFrom(_source)
	#parse_exports(_exports)
	print '_exports=%s' % (_exports)
	fpath = '/root/lib/python2.5/site-packages/easy-install.pth'
	fin = open(fpath)
	try:
		dname = os.path.dirname(fpath)
		for aLine in fin.readlines():
			if (_re.search(aLine)):
				fname = os.sep.join([_process_(f,dname) for f in aLine.strip().split(os.sep)])
				if (not any([fname.find(n) > -1 for n in _ignores_])):
					print '='*30
					print 'fname=%s' % (fname)
					#_ll_ = parse_exports(_exports,target=fname)
					#print '_ll_=%s' % (_ll_)
					_exports = parse_exports(_exports,target=fname)
					print '_exports=%s' % (_exports)
					print '='*30
					print
	except Exception, ex:
		print str(ex)
	fin.close()
	writelinesTo(_source,_exports)

