import palm_imputil
imputil = palm_imputil
import palmdm
import sys
if not hasattr(sys,'path'):
    sys.path = []

import_manager = None

class MemoDBImporter(imputil.Importer):
    def __init__(self, name='MemoDB', category = 'Python'):
        self.category = category
        self.name = name
        self.cache = {}
        self._check_db()
        self._populate_cache()
        
    def _check_db(self):
        "Raise an exception if the database/category is invalid"
        db = palmdm.open(self.name, 'r')
        cats = db.listcategories()
        my_cat = filter(lambda x, self=self: x == self.category, cats)
        if not my_cat:
            raise palmdm.error, "Can't find category %s" % self.category
        db.close()
        
    def _populate_cache(self):
        db = palmdm.open(self.name,'r')
        recs = db.listrecords(self.category)
        for i in recs:
            rec = db.queryrecord(i)
            buf = rec.getbuffer()
            title = _get_module_name(_get_title(buf))
            rec.release()
            if title:
                self.cache[title] = i
        db.close()

    def _load_module(self, module_name):
        if not self.cache.has_key(module_name):        
            self._populate_cache()
        if not self.cache.has_key(module_name):
            raise ImportError, "Module not found"

        db = palmdm.open(self.name,'r')
        rec = db.queryrecord(self.cache[module_name])
        buf = rec.getbuffer()

        # need to modify compile to allow
        code = compile(buf[:-1], "MemoDB: module_name", "exec")
        rec.release()
        db.close()
        return code
    def get_code(self, parent, modname, fqname):
        if parent:  # assume no parent initially
            return None
        co = self._load_module(modname)
        return 0, co, {}

def _get_title(buf):
    # return the title found in the MemoDB buffer - should move
    # this to the palmapps module
    i = 0
    size = len(buf)
    while(i < len(buf)):
        if buf[i] == '\n':
            size = i  # not i+1, since '\n' is not included in the title
            break
        i = i + 1
    return buffer(buf, 0, size)


_skip = ('\n',' ','\t','\r', '#')
def _get_module_name(buf):
    # given the one-line title, search for a module name
    s = str(buf)
    s = s.strip()
    if s[0] <> "#":
        return ''
    fin = s.find('.py')
    if fin == -1:
        return ''
    i = 0
    while (i < fin):
        if not s[i] in _skip:
            break
        i = i + 1
    return s[i:fin]

def install():
    global import_manager
    if not import_manager:
        import_manager = imputil.ImportManager()
        import_manager.install()
        sys.path.append(imputil.BuiltinImporter())
        sys.path.append(MemoDBImporter())

def uninstall():
    global import_manager
    if import_manager:
        import_manager.uninstall()
        import_manager = None
        
