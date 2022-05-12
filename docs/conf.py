# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Reticula'
copyright = '2022'
author = 'Arash Badie-Modiri'

# The full version, including alpha/beta/rc tags
release = '0.0.3'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
  'sphinxcontrib.bibtex'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

primary_domain = None

nitpicky = True

rst_prolog = """
.. role:: py(code)
    :language: python
    :class: highlight

.. role:: cpp(code)
    :language: cpp
    :class: highlight
"""

bibtex_bibfiles = ['references.bib']
bibtex_default_style = 'unsrt'


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'furo'

pygments_style = "sphinx"
pygments_dark_style = "monokai"
html_title = "Reticula"

import os.path
def read_icon(path: str):
  with open(os.path.join(os.path.dirname(__file__), path), 'r') as f:
    return f.read()

html_theme_options = {
  "source_repository": "https://github.com/reticula-network/reticula-python",
  "source_branch": "main",
  "source_directory": "docs/",
  "footer_icons": [
    {
      "name": "GitHub",
      "url": "https://github.com/reticula-network",
      "html": read_icon("github.svg"),
      "class": "",
    }, {
      "name": "PyPi",
      "url": "https://pypi.org/project/reticula/",
      "html": read_icon("pypi.svg"),
      "class": "",
    },
  ],
}


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
