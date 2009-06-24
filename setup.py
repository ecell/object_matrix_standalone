import os
import re
from glob import glob
from distutils.ccompiler import new_compiler
from distutils.sysconfig import customize_compiler
from numpy.distutils.core import setup, Extension
from distutils.errors import CompileError


class AutoconfEmulator(object):
    def __init__(self, lang_ext="c"):
        ccompiler = new_compiler()
        customize_compiler(ccompiler)
        self.ccompiler = ccompiler
        self.macros = []
        self.include_dirs = []
        self.default_lang_ext = lang_ext
        self.header_availability = {}
        self.type_availability = {}
        self.decl_availability = {}

    def check_headers(self, headers, includes=[], lang_ext=None):
        if lang_ext == None:
            lang_ext = self.default_lang_ext
        src = 'conftest.' + lang_ext
        for header in headers:
            try:
                open(src, 'w').write(self.generate_includes(includes + [header]))
                try:
                    self.ccompiler.preprocess(source=src,
                        output_file=os.devnull,
                        macros=self.macros, include_dirs=self.include_dirs)
                    available = True
                except CompileError:
                    available = False
                self.header_availability[header] = available
            finally:
                os.unlink(src)

    def check_types(self, types, includes=[], lang_ext=None):
        if lang_ext == None:
            lang_ext = self.default_lang_ext
        src = 'conftest.' + lang_ext
        for type in types:
            try:
                open(src, 'w').write(
                    self.generate_includes(includes) +
                    '\nint main() { sizeof(%s); }' % type)
                try:
                    self.ccompiler.compile(sources=[src],
                        output_dir='.',
                        macros=self.macros, include_dirs=self.include_dirs)
                    available = True
                except CompileError:
                    available = False
                self.type_availability[type] = available
            finally:
                for file in glob('conftest.*'):
                    os.unlink(file)

    def check_decls(self, decls, includes=[], lang_ext=None):
        if lang_ext == None:
            lang_ext = self.default_lang_ext
        src = 'conftest.' + lang_ext
        for decl in decls:
            try:
                open(src, 'w').write(
                    self.generate_includes(includes) +
                    '\nint main() { %s; }' % decl)
                try:
                    self.ccompiler.compile(sources=[src],
                        output_dir='.',
                        macros=self.macros, include_dirs=self.include_dirs)
                    available = True
                except CompileError:
                    available = False
                self.decl_availability[decl] = available
            finally:
                for file in glob('conftest.*'):
                    os.unlink(file)


    @staticmethod
    def generate_includes(headers):
        return "\n".join('#include <%s>' % header for header in headers)

    @staticmethod
    def macro_name(val):
        return re.sub(r'::|[./-]', '_', re.sub(r'<.*', '', val)).upper()

    def generate(self, file):
        f = open(file, 'w')
        for items in self.header_availability.iteritems(), self.type_availability.iteritems():
            for key, value in items:
                if value:
                    f.write('#define HAVE_%s 1\n' % self.macro_name(key))
                else:
                    f.write('#undef HAVE_%s\n' % self.macro_name(key))
        for key, value in self.decl_availability.iteritems():
            if value:
                f.write('#define HAVE_DECL_%s 1\n' % self.macro_name(key))
            else:
                f.write('#undef HAVE_DECL_%s\n' % self.macro_name(key))

if not os.path.exists('config.h'):
    ac = AutoconfEmulator(lang_ext="cpp")
    ac.check_headers(
        [
            'tr1/functional',
            'tr1/unordered_map',
            'unordered_map',
            'boost/functional/hash.hpp'
            ],
        lang_ext="cpp")
    ac.check_types(['std::hash<int>'], includes=['functional'])
    ac.check_types(['std::tr1::hash<int>'], includes=['tr1/functional'])
    ac.generate('config.h')

setup(
    name = 'object_matrix',
    version = '0.0.0',
    description = 'object_matrix standalone',
    packages = [ '' ],
    ext_modules = [
        Extension(
            'object_matrix',
            [ 'object_matrix_module.cpp', 'src/Model.cpp', 'src/NetworkRules.cpp', 'src/BasicNetworkRulesImpl.cpp', 'src/utils.cpp' ],
            include_dirs = [ 'src', '.' ],
            libraries = [ 'boost_python-mt', 'gsl', 'gslcblas' ],
            define_macros = [ ('USE_NUMPY', '1'), ('HAVE_CONFIG_H', '1') ],
            language = 'c++',
            )
        ]
    )
