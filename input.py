from distutils.core import setup, Extension

module1 = Extension(
	'matrix_norms', # название модуля внутри Python
	 sources = ['matrix_norms.c'] # Исходные файлы модуля
)

setup(
	name = 'matrix_norms',              # название модуля (my_plus.__name__)
	version = '1.1',               # версия модуля
	description = 'matrix_norms', # описание модуля
	ext_modules= [module1]         # объекты типа Extension (мы объявили его выше)
)
