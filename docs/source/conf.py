# -*- coding: utf-8 -*-
#
# GDK documentation build configuration file
SCANNING, DOCS, FUNC = 1, 2, 3

def get_doc_lines(l):
    if l.startswith('.. '):
        return ['   ' + l, '']
    return ['   ' + l]

def output_func(docs, func):
    # is_normal_ret = 'GDK_API int' in func
    is_normal_ret = True
    func = func[:-1].replace('GDK_API', '').strip()
    func = func.replace(',', ', ').replace('  ', ' ')
    ret = ['.. c:function:: ' + func, '']
    for l in docs:
        ret.extend(get_doc_lines(l))
    if is_normal_ret:
        ret.append('   :return: GA_OK or an error code.')  # FIXME: Link
        ret.append('   :rtype: int')
    ret.append('')
    ret.append('')
    return ret

def extract_docs(infile, outfile):

    lines = [l.strip() for l in open(infile).readlines()]
    title = infile.split('/')[-1][:-2].upper() + ' Functions'  # FIXME. we may need C headers to have a prefix
    title_markup = '=' * len(title)
    output, current, func, state = [title, title_markup, ''], [], '', SCANNING

    for l in lines:
        if state == SCANNING:
            if l.startswith('/**') and '*/' not in l:
                current, func, state = [l[3:]], '', DOCS
        elif state == DOCS:
            if l == '*/':
                state = FUNC
            else:
                assert l.startswith('*'), l
                if l.startswith('*|'):
                    current[-1] += ' ' + l[2:].strip()
                else:
                    current.append(l[1:].strip())
        else:  # FUNC
            func += l
            if ');' in func:
                output.extend(output_func(current, func))
                state = SCANNING
    with open(outfile, 'w') as f:
        f.write('\n'.join(output))

# Generate the documentation source files
# FIXME: elements
for m in [
    'gdk'
    ]:
    extract_docs('../../include/%s.h' % m, '%s.rst' % m)

# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['sphinx.ext.ifconfig',
    'sphinx.ext.githubpages']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

default_role = 'any'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'GDK'
copyright = u'2019, Blockstream Corporation'
author = u'Blockstream Corporation'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.

import re
version_re = "project\(gdk VERSION (.*) LANGUAGES"
with open('../../CMakeLists.txt', 'r') as cmake_file:
    for line in cmake_file:
        matched = re.match(version_re, line)
        if matched is not None:
            version = matched.group(1)
            break

# The full version, including alpha/beta/rc tags.
release = version

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = []

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'sphinx_rtd_theme'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


# -- Options for HTMLHelp output ------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = '%sdoc' % project

# Don't include the auto-generated source documents
html_copy_source = False
html_show_sourcelink = False

# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, '%s.tex' % project, u'%s Documentation' % project,
     author, 'manual'),
]


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, project, u'%s Documentation' % project,
     [author], 1)
]


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, project, u'%s Documentation' % project,
     author, project, 'The GreenAddress Development Kit.',
     'Miscellaneous'),
]



