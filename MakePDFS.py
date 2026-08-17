#!/usr/bin/env python3
"""
Build a printable, ICPC-style team reference notebook from the .cpp templates
in this repository.

Layout is two-column / small-font (KACTL style) so the whole notebook fits in a
reasonable page count instead of the ~140 pages a single-column build produces.

Usage:
    python3 MakePDFS.py                 # generate the .tex only
    python3 MakePDFS.py --build         # generate .tex and run pdflatex twice
    python3 MakePDFS.py --build --font tiny --no-numbers   # squeeze harder

Everything is written into a scratch directory (.texbuild/) so the source tree
stays clean; the finished PDF is copied back to the repository root.
"""

import argparse
import os
import re
import shutil
import subprocess
import sys

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(ROOT_DIR, '.texbuild')
EXCLUDED_DIRS = {'.git', '.vscode', '__pycache__', '.texbuild', 'PDFs', 'Notes'}
NOTES_DIR = 'Notes'          # raw .tex chapters (cheat sheets etc.), included verbatim
SOURCE_EXTS = ('.cpp', '.h', '.hpp', '.cc')

# Full-page cover image, used as page 1 if present. pdfTeX can embed png/jpg/pdf.
COVER_CANDIDATES = ('cover.png', 'cover.jpg', 'cover.jpeg', 'cover.pdf')
COVER_ASSET = 'cover-image'  # basename inside .texbuild/ (extension appended)

DEFAULT_TITLE = 'Hamza'
DEFAULT_AUTHOR = 'Hamza'
DEFAULT_OUTPUT = 'Hamza'     # .tex / .pdf basename


# --------------------------------------------------------------------------
# LaTeX escaping
# --------------------------------------------------------------------------

# Single-pass table: escaping character-by-character means a replacement can
# never be re-processed by a later rule. (The previous version escaped '#{}'
# into '\#' and then rewrote every '\' into '\textbackslash', corrupting them.)
_TEX_ESCAPES = {
    '\\': r'\textbackslash{}',
    '&': r'\&',
    '%': r'\%',
    '$': r'\$',
    '#': r'\#',
    '_': r'\_',
    '{': r'\{',
    '}': r'\}',
    '~': r'\textasciitilde{}',
    '^': r'\textasciicircum{}',
    '\u2013': '-',   # en dash
    '\u2014': '-',   # em dash
    '\u2018': "'",
    '\u2019': "'",
    '\u201c': "''",
    '\u201d': "''",
}


def tex_escape(text):
    """Escape a string for use in ordinary LaTeX text."""
    return ''.join(_TEX_ESCAPES.get(ch, ch) for ch in text)


def safe_asset_name(filename):
    """
    Filename used inside the build directory.

    \\lstinputlisting takes an unescaped path, so the copy must avoid the
    characters TeX treats specially in a filename argument.
    """
    stem, ext = os.path.splitext(filename)
    stem = stem.replace('\u2013', '-').replace('\u2014', '-')
    stem = re.sub(r'[^A-Za-z0-9._+-]', '-', stem)
    stem = re.sub(r'-{2,}', '-', stem).strip('-')
    return (stem or 'file') + ext


def display_title(filename):
    """Human-readable heading for a source file."""
    stem = os.path.splitext(filename)[0]
    return tex_escape(stem)


# --------------------------------------------------------------------------
# Document generation
# --------------------------------------------------------------------------

FONT_SIZES = {
    'footnotesize': (r'\footnotesize', 0.55),
    'scriptsize': (r'\scriptsize', 0.50),
    'tiny': (r'\tiny', 0.48),
}


def build_preamble(args):
    font_cmd, basewidth = FONT_SIZES[args.font]
    numbers = 'left' if not args.no_numbers else 'none'

    return r"""\documentclass[10pt,a4paper,twocolumn]{article}

\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage{lmodern}
\usepackage{geometry}
\usepackage{listings}
\usepackage{xcolor}
\usepackage{graphicx}
\usepackage{fancyhdr}
\usepackage[hidelinks]{hyperref}

% Tight page: the notebook is meant to be printed and flipped through, not read
% like prose, so margins are minimal and the two columns sit close together.
\geometry{a4paper, top=1.1cm, bottom=1.1cm, left=1cm, right=1cm,
          headsep=0.25cm, columnsep=0.55cm}

\setlength{\parindent}{0pt}
\setlength{\columnseprule}{0.4pt}

\pagestyle{fancy}
\fancyhf{}
\fancyhead[L]{\small\itshape\nouppercase{\leftmark}}
\fancyhead[R]{\small\thepage}
\renewcommand{\headrulewidth}{0.4pt}

\hypersetup{
    pdftitle={""" + tex_escape(args.title) + r"""},
    pdfauthor={""" + tex_escape(args.author) + r"""},
    bookmarksnumbered=true,
}

% Compact headings -- default article spacing wastes a lot of vertical room.
% Done with base \@startsection rather than titlesec, which lives in the much
% larger texlive-latex-extra package.
\makeatletter
\renewcommand\section{\@startsection{section}{1}{\z@}%
  {-1.4ex \@plus -.3ex \@minus -.2ex}{0.7ex \@plus .1ex}%
  {\normalfont\large\bfseries}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
  {-1.1ex \@plus -.2ex \@minus -.2ex}{0.35ex \@plus .1ex}%
  {\normalfont\normalsize\bfseries}}
\makeatother

\definecolor{codegreen}{rgb}{0,0.5,0}
\definecolor{codegray}{rgb}{0.55,0.55,0.55}
\definecolor{codepurple}{rgb}{0.55,0,0.75}
\definecolor{codeblue}{rgb}{0,0,0.7}
\definecolor{backcolour}{rgb}{0.975,0.975,0.975}

\lstdefinestyle{cpstyle}{
    backgroundcolor=\color{backcolour},
    commentstyle=\color{codegreen},
    keywordstyle=\color{codeblue}\bfseries,
    stringstyle=\color{codepurple},
    numberstyle=\tiny\color{codegray},
    basicstyle=""" + font_cmd + r"""\ttfamily,
    columns=fullflexible,
    basewidth=""" + f'{basewidth}' + r"""em,
    keepspaces=true,
    breaklines=true,
    breakatwhitespace=false,
    breakindent=1em,
    postbreak=\mbox{\textcolor{codegray}{$\hookrightarrow$}\space},
    numbers=""" + numbers + r""",
    numbersep=4pt,
    xleftmargin=""" + ('1.6em' if numbers == 'left' else '0.4em') + r""",
    framexleftmargin=0pt,
    frame=none,
    aboveskip=0.5ex,
    belowskip=0.9ex,
    showspaces=false,
    showstringspaces=false,
    showtabs=false,
    tabsize=2,
    inputencoding=utf8,
    extendedchars=true,
    literate={á}{{\'a}}1 {é}{{\'e}}1 {í}{{\'i}}1 {ó}{{\'o}}1 {ú}{{\'u}}1
             {ñ}{{\~n}}1 {°}{{\textdegree}}1 {≤}{{$\leq$}}1 {≥}{{$\geq$}}1
             {→}{{$\rightarrow$}}1 {∞}{{$\infty$}}1 {–}{{-}}1 {—}{{-}}1
             {’}{{'}}1 {‘}{{'}}1 {“}{{''}}1 {”}{{''}}1,
}
\lstset{style=cpstyle, language=C++}

% Listings must be allowed to split across columns/pages, otherwise a long
% template leaves half a column blank.
\lstset{breaklines=true}

"""


def build_cover(cover_asset, fit):
    """
    Full-bleed cover as page 1.

    The image is emitted as a *zero-metric overlay*: \\raisebox forces height and
    depth to 0pt and \\makebox forces width to 0pt, so as far as the page builder
    is concerned the page is empty. That matters because two independent
    problems bite otherwise:

      * an image exactly \\paperheight tall overflows the text block by a hair
        and spills a second, near-blank page;
      * anything with real metrics gets typeset *into a column*, and vertical
        fill glue shoves it into column two.

    With zero metrics it is anchored at the top-left of the paper and ignores
    the column layout entirely, so no \\onecolumn is needed -- which is lucky,
    because \\newgeometry recomputes \\columnwidth from the *class option* and
    silently undoes \\onecolumn anyway.
    """
    size = (r'width=\paperwidth,height=\paperheight' if fit == 'stretch'
            else r'width=\paperwidth,height=\paperheight,keepaspectratio')

    return r"""% ---- cover page -------------------------------------------------------
\begingroup
\setlength{\columnseprule}{0pt}
\newgeometry{margin=0pt}
\thispagestyle{empty}
\noindent\makebox[0pt][l]{\raisebox{-\paperheight}[0pt][0pt]{%
  \parbox[b][\paperheight][c]{\paperwidth}{\centering%
  \includegraphics[""" + size + r"""]{""" + cover_asset + r"""}}}}%
\clearpage
\restoregeometry
\endgroup
% -----------------------------------------------------------------------

"""


def build_front_matter(args, cover_asset=None):
    out = r"""\begin{document}

"""
    if cover_asset:
        # The image is the title page, so the text title band is dropped and the
        # notebook goes straight from cover to contents. No \twocolumn here --
        # the class option already provides it and calling it would \clearpage
        # an extra blank page in.
        out += build_cover(cover_asset, args.cover_fit)
    else:
        out += build_title_band(args)

    out += r"""
\setcounter{tocdepth}{2}
{\small\tableofcontents}
\vspace{1em}

"""
    return out


def build_title_band(args):
    return r"""% Full-width title band above the two columns. Note: no \\ line breaks here --
% inside \twocolumn[...] they blow up \@icentercr; use \par + \vspace instead.
\twocolumn[
  \begin{@twocolumnfalse}
  \centering
  {\LARGE\bfseries """ + tex_escape(args.title) + r"""}\par
  \vspace{0.3em}
  {\normalsize """ + tex_escape(args.author) + r"""}\par
  \vspace{0.2em}
  {\small\today}\par
  \vspace{0.6em}
  \hrule
  \vspace{0.8em}
  \end{@twocolumnfalse}
]
"""


def collect_chapters():
    """Return [(chapter_name, [filenames])] for every non-excluded folder."""
    chapters = []
    for name in sorted(os.listdir(ROOT_DIR)):
        path = os.path.join(ROOT_DIR, name)
        if not os.path.isdir(path) or name in EXCLUDED_DIRS or name.startswith('.'):
            continue
        files = sorted(f for f in os.listdir(path) if f.endswith(SOURCE_EXTS))
        if files:
            chapters.append((name, files))
    return chapters


def collect_notes():
    """Raw .tex chapters (cheat sheets, formula pages) from Notes/."""
    path = os.path.join(ROOT_DIR, NOTES_DIR)
    if not os.path.isdir(path):
        return []
    return sorted(f for f in os.listdir(path) if f.endswith('.tex'))


def find_cover(explicit):
    """Locate the cover image: an explicit --cover path, else cover.* in root."""
    if explicit:
        path = explicit if os.path.isabs(explicit) else os.path.join(ROOT_DIR, explicit)
        if not os.path.isfile(path):
            raise SystemExit(f'cover not found: {explicit}')
        return path
    for name in COVER_CANDIDATES:
        path = os.path.join(ROOT_DIR, name)
        if os.path.isfile(path):
            return path
    return None


def stage_cover(cover_path):
    """
    Copy the cover into the build dir, returning the name to \\includegraphics.

    pdfTeX can only embed *baseline* JPEGs -- a progressive JPEG makes it fail
    with "Unsupported JPEG" -- so progressive files are re-encoded on the way in.
    """
    ext = os.path.splitext(cover_path)[1].lower()
    if ext == '.jpeg':
        ext = '.jpg'
    dest = os.path.join(BUILD_DIR, COVER_ASSET + ext)

    if ext == '.jpg':
        try:
            from PIL import Image
            with Image.open(cover_path) as im:
                if im.info.get('progressive') or im.info.get('progression'):
                    print(f'  cover: re-encoding progressive JPEG as baseline '
                          f'({im.width}x{im.height})')
                    im.convert('RGB').save(dest, 'JPEG', quality=95,
                                           progressive=False, optimize=True)
                    return COVER_ASSET + ext
        except ImportError:
            print('  cover: Pillow not installed; if pdflatex reports an '
                  'unsupported JPEG, convert it to baseline or PNG first')

    shutil.copyfile(cover_path, dest)
    return COVER_ASSET + ext


def generate_body(chapters, notes):
    out = []

    # Notes first: a formula sheet is worth more at the front of the notebook
    # than buried after 60 pages of code.
    for note in notes:
        out.append(f'\\input{{{NOTES_DIR}/{safe_asset_name(note)}}}\n\n')

    for chapter, files in chapters:
        title = tex_escape(chapter.replace('_', ' ').replace('-', ' '))
        out.append(f'\\section{{{title}}}\n')
        out.append(f'\\markboth{{{title}}}{{{title}}}\n')
        for filename in files:
            out.append(f'\\subsection{{{display_title(filename)}}}\n')
            out.append(
                f'\\lstinputlisting{{{chapter}/{safe_asset_name(filename)}}}\n\n')
    return ''.join(out)


# --------------------------------------------------------------------------
# Build directory
# --------------------------------------------------------------------------

def stage_sources(chapters, notes):
    """
    Copy every source file into .texbuild/ under a TeX-safe name.

    Copying (rather than symlinking inside the repo, as the previous version
    did) keeps the working tree clean and means the build directory can be
    deleted without touching anything tracked.
    """
    if os.path.isdir(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)
    os.makedirs(BUILD_DIR)

    for chapter, files in chapters:
        dest_dir = os.path.join(BUILD_DIR, chapter)
        os.makedirs(dest_dir, exist_ok=True)
        for filename in files:
            shutil.copyfile(os.path.join(ROOT_DIR, chapter, filename),
                            os.path.join(dest_dir, safe_asset_name(filename)))

    if notes:
        dest_dir = os.path.join(BUILD_DIR, NOTES_DIR)
        os.makedirs(dest_dir, exist_ok=True)
        for note in notes:
            shutil.copyfile(os.path.join(ROOT_DIR, NOTES_DIR, note),
                            os.path.join(dest_dir, safe_asset_name(note)))


def run_pdflatex(tex_name, output_pdf):
    if shutil.which('pdflatex') is None:
        print('\npdflatex not found. Install the minimal LaTeX set with:\n'
              '  sudo apt-get install --no-install-recommends \\\n'
              '      texlive-latex-base texlive-latex-recommended \\\n'
              '      texlive-fonts-recommended\n'
              '(texlive-full is ~5.5GB and is not needed for this document.)',
              file=sys.stderr)
        return False

    # Twice: first pass writes the .toc, second pass typesets it.
    for run in (1, 2):
        print(f'  pdflatex pass {run}/2 ...')
        proc = subprocess.run(
            ['pdflatex', '-interaction=nonstopmode', '-halt-on-error', tex_name],
            cwd=BUILD_DIR, capture_output=True, text=True)
        if proc.returncode != 0:
            print('\npdflatex failed:\n', file=sys.stderr)
            print('\n'.join(proc.stdout.splitlines()[-40:]), file=sys.stderr)
            return False

    built = os.path.join(BUILD_DIR, os.path.splitext(tex_name)[0] + '.pdf')
    shutil.copyfile(built, output_pdf)
    return True


# --------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description='Build an ICPC-style two-column reference notebook.')
    parser.add_argument('--title', default=DEFAULT_TITLE)
    parser.add_argument('--author', default=DEFAULT_AUTHOR)
    parser.add_argument('--out', default=DEFAULT_OUTPUT,
                        help='output basename (default: %(default)s)')
    parser.add_argument('--font', choices=FONT_SIZES.keys(), default='scriptsize',
                        help='listing font size (default: %(default)s)')
    parser.add_argument('--no-numbers', action='store_true',
                        help='drop line numbers to save horizontal space')
    parser.add_argument('--cover', metavar='PATH',
                        help='cover image for page 1 (default: auto-detect '
                             'cover.png/.jpg/.jpeg/.pdf in the repo root)')
    parser.add_argument('--no-cover', action='store_true',
                        help='ignore any cover image and use the text title page')
    parser.add_argument('--cover-fit', choices=('fit', 'stretch'), default='fit',
                        help='"fit" preserves aspect ratio, "stretch" fills the '
                             'page exactly (default: %(default)s)')
    parser.add_argument('--build', action='store_true',
                        help='run pdflatex after generating the .tex')
    args = parser.parse_args()

    chapters = collect_chapters()
    notes = collect_notes()
    if not chapters and not notes:
        print('No source files found.', file=sys.stderr)
        return 1

    total = sum(len(f) for _, f in chapters)
    for chapter, files in chapters:
        print(f'{chapter}: {len(files)} file(s)')
    if notes:
        print(f'{NOTES_DIR}: {len(notes)} note(s)')
    print(f'-> {total} source files across {len(chapters)} chapters')

    stage_sources(chapters, notes)

    cover_asset = None
    cover_path = None if args.no_cover else find_cover(args.cover)
    if cover_path:
        print(f'cover: {os.path.relpath(cover_path, ROOT_DIR)}')
        cover_asset = stage_cover(cover_path)

    tex = (build_preamble(args) + build_front_matter(args, cover_asset)
           + generate_body(chapters, notes) + '\n\\end{document}\n')

    tex_name = args.out + '.tex'
    with open(os.path.join(BUILD_DIR, tex_name), 'w', encoding='utf-8') as f:
        f.write(tex)
    print(f"\nWrote {os.path.join('.texbuild', tex_name)}")

    if not args.build:
        print(f'Run "python3 MakePDFS.py --build" to produce {args.out}.pdf, '
              f'or compile .texbuild/{tex_name} yourself (twice, for the TOC).')
        return 0

    output_pdf = os.path.join(ROOT_DIR, args.out + '.pdf')
    if not run_pdflatex(tex_name, output_pdf):
        return 1

    print(f'\nBuilt {args.out}.pdf')
    return 0


if __name__ == '__main__':
    sys.exit(main())
