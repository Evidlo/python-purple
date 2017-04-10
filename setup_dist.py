#!/usr/bin/python3

from distutils.core import setup
from distutils.extension import Extension

from subprocess import Popen, PIPE

cflags = Popen(['pkg-config', '--cflags', 'purple'], stdout=PIPE).communicate()[0].decode()
ldflags = Popen(['pkg-config', '--libs', 'purple'], stdout=PIPE).communicate()[0].decode()

purplemodule = Extension('purple',
                         sources=['c_purple.c','purple.c'],
                         extra_compile_args=cflags.split(),
                         extra_link_args=ldflags.split())

long_description = "\
Python bindings for libpurple, a multi-protocol instant messaging library."

setup(name = 'python-purple',
      version = '0.1',
      author ='Bruno Abinader',
      author_email = 'bruno.abinader@openbossa.org',
      description = 'Python bindings for Purple',
      long_description = long_description,
      ext_modules = [purplemodule],
      )
