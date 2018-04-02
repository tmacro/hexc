import io
import os
import re
from glob import glob
from os.path import basename, dirname, join, relpath, splitext
from setuptools import Extension, find_packages, setup


def read(*names, **kwargs):
	return io.open(
		join(dirname(__file__), *names),
		encoding=kwargs.get('encoding', 'utf8')
	).read()

def translate_to_rst(text):
    pass
# Enable code coverage for C code: we can't use CFLAGS=-coverage in tox.ini, since that may mess with compiling
# dependencies (e.g. numpy). Therefore we set SETUPPY_CFLAGS=-coverage in tox.ini and copy it to CFLAGS here (after
# deps have been safely installed).
# if 'TOXENV' in os.environ and 'SETUPPY_CFLAGS' in os.environ:
#     os.environ['CFLAGS'] = os.environ['SETUPPY_CFLAGS']

hexc_ext = Extension(
	'hexc',
	libraries = ['Judy'],
	sources = glob('hexc/*.c'),
	include_dirs = ['hexc/', '/usr/local/include'],
	library_dirs = ['/usr/local/lib']
)

setup(
	name="hexc",
	version="0.1.2",
	url="https://github.com/tmacro/hexc",
	license='BSD 3-Clause',

	author="Tayor McKinnon",
	author_email="taylor@uncannypacket.com",

	description="A module for storing and manipulating hexagonal coordinates and grids.",
	# long_description='%s\n%s' % (
    #     open('README.rst').read(), # Extract the README removing to badges and appanding the changlog
	# 	re.compile('^.. start-badges.*^.. end-badges', re.M | re.S).sub('', read('README.rst')),
	# 	re.sub(':[a-z]+:`~?(.*?)`', r'``\1``', read('CHANGELOG.rst'))
    # ),
	packages=find_packages('hexc'),
	package_dir={'': 'hexc'},
	install_requires=[],

	classifiers=[
		'Development Status :: 2 - Pre-Alpha',
		'Programming Language :: Python',
		'Programming Language :: Python :: 3',
		'Programming Language :: Python :: 3.4',
		'Programming Language :: Python :: 3.5',
		'Programming Language :: Python :: 3.6',
	],
   
	py_modules=[splitext(basename(path))[0] for path in glob('hexc/*.py')],
	include_package_data=True,
	zip_safe=False,
	ext_modules=[hexc_ext],
)
