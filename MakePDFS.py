import os
import re

# --- Configuration ---
ROOT_DIR = '.'  # Assumes the script is in the root with your topic folders
# Add any folder names to ignore
EXCLUDED_DIRS = {'.git', '.vscode', '__pycache__'}
OUTPUT_FILENAME = 'Hamza.tex'
BOOK_TITLE = 'Hamza'
AUTHOR_NAME = 'Hamza'
# ---------------------


def sanitize_for_latex(text):
    """
    Escapes special LaTeX characters in a given string.
    This is a crucial fix.
    """
    # First handle problematic characters that cause file path issues
    text = text.replace('–', '-')  # Replace em dash with regular dash
    text = text.replace('—', '-')  # Replace em dash with regular dash
    text = text.replace('&', 'and')  # Replace ampersand with 'and'
    text = text.replace('%', 'percent')  # Replace percent with 'percent'
    # Replace underscore with dash to avoid math mode
    text = text.replace('_', '-')

    # Fix double spaces
    while '  ' in text:
        text = text.replace('  ', ' ')

    # Then escape remaining LaTeX special characters (but not % and _ since we replaced them)
    text = re.sub(r'([#{}])', r'\\\1', text)
    text = text.replace('\\', r'\textbackslash')
    text = text.replace('^', r'\textasciicircum')
    text = text.replace('~', r'\textasciitilde')
    text = text.replace('$', r'\$')
    return text


def sanitize_filename(filename):
    """
    Sanitize filename for LaTeX file paths - simpler than full LaTeX escaping
    """
    # Replace problematic characters in filenames
    filename = filename.replace('–', '-')  # Replace em dash with regular dash
    filename = filename.replace('—', '-')  # Replace em dash with regular dash
    filename = filename.replace('&', 'and')  # Replace ampersand with 'and'
    # Replace percent with 'percent'
    filename = filename.replace('%', 'percent')
    filename = filename.replace('_', '-')  # Replace underscore with dash
    # Fix double spaces
    while '  ' in filename:
        filename = filename.replace('  ', ' ')
    return filename


def create_safe_symlinks():
    """
    Create symbolic links for files with problematic characters in their names
    """
    import os

    dirs = sorted([d for d in os.listdir(ROOT_DIR) if os.path.isdir(
        os.path.join(ROOT_DIR, d)) and d not in EXCLUDED_DIRS])

    for chapter_name in dirs:
        chapter_path = os.path.join(ROOT_DIR, chapter_name)
        files = sorted([f for f in os.listdir(chapter_path)
                       if f.endswith(('.cpp', '.h', '.hpp'))])

        for filename in files:
            safe_filename = sanitize_filename(filename)
            if filename != safe_filename:
                original_path = os.path.join(chapter_path, filename)
                safe_path = os.path.join(chapter_path, safe_filename)

                # Remove existing symlink if it exists
                if os.path.islink(safe_path):
                    os.unlink(safe_path)

                # Create symbolic link with safe name
                if not os.path.exists(safe_path):
                    try:
                        os.symlink(filename, safe_path)
                        print(f"Created symlink: {safe_path} -> {filename}")
                    except OSError as e:
                        print(f"Warning: Could not create symlink for {
                              filename}: {e}")


def generate_latex_content():
    """Scans directories and generates the corrected LaTeX source code."""

    # LaTeX preamble: Document setup, packages, and code style.
    # Added \usepackage[utf8]{inputenc} for better character support.
    preamble = r"""
\documentclass[11pt]{book}
\usepackage[utf8]{inputenc}
\usepackage{geometry}
\usepackage{listings}
\usepackage{hyperref}
\usepackage{xcolor}
\usepackage{lmodern} % Use a modern font that has more glyphs

% Page geometry
\geometry{a4paper, margin=1in}

% PDF metadata and clickable links
\hypersetup{
    colorlinks=true,
    linkcolor=blue,
    filecolor=magenta,      
    urlcolor=cyan,
    pdftitle={""" + sanitize_for_latex(BOOK_TITLE) + r"""},
    pdfauthor={""" + sanitize_for_latex(AUTHOR_NAME) + r"""},
}

% C++ syntax highlighting style for the 'listings' package
\definecolor{codegreen}{rgb}{0,0.6,0}
\definecolor{codegray}{rgb}{0.5,0.5,0.5}
\definecolor{codepurple}{rgb}{0.58,0,0.82}
\definecolor{backcolour}{rgb}{0.98,0.98,0.98}

\lstdefinestyle{mystyle}{
    backgroundcolor=\color{backcolour},   
    commentstyle=\color{codegreen},
    keywordstyle=\color{magenta},
    numberstyle=\tiny\color{codegray},
    stringstyle=\color{codepurple},
    basicstyle=\small\ttfamily, % Using \small for better fit
    breakatwhitespace=false,         
    breaklines=true,                 
    captionpos=b,                    
    keepspaces=true,                 
    numbers=left,                    
    numbersep=5pt,                  
    showspaces=false,                
    showstringspaces=false,
    showtabs=false,                  
    tabsize=2,
    inputencoding=utf8, % Assume C++ files are UTF-8 encoded
    literate={á}{{\'a}}1 {é}{{\'e}}1 {í}{{\'i}}1 {ó}{{\'o}}1 {ú}{{\'u}}1 % Handle accents
}
\lstset{style=mystyle, language=C++}

"""

    # Start of the document
    doc_start = r"""
\begin{document}

\title{""" + sanitize_for_latex(BOOK_TITLE) + r"""}
\author{""" + sanitize_for_latex(AUTHOR_NAME) + r"""}
\date{\today}
\maketitle

\frontmatter
\tableofcontents

\mainmatter
"""

    # End of the document
    doc_end = r"""
\end{document}
"""

    # --- File Scanning and Content Generation ---
    body_content = []
    dirs = sorted([d for d in os.listdir(ROOT_DIR) if os.path.isdir(
        os.path.join(ROOT_DIR, d)) and d not in EXCLUDED_DIRS])

    for chapter_name in dirs:
        chapter_path = os.path.join(ROOT_DIR, chapter_name)
        files = sorted([f for f in os.listdir(chapter_path)
                       if f.endswith(('.cpp', '.h', '.hpp'))])

        if not files:
            continue

        print(f"Processing Chapter: {chapter_name}")
        body_content.append(
            f"\\chapter{{{sanitize_for_latex(chapter_name.replace('_', ' '))}}}\n")

        for filename in files:
            # Sanitize filename for safe file path usage
            safe_filename = sanitize_filename(filename)

            # --- FIX #1: Ensure forward slashes for LaTeX paths ---
            # This creates a path like "Data Structures/My_File.cpp"
            latex_path = f"{chapter_name}/{safe_filename}"

            # --- FIX #2: Sanitize the caption text for LaTeX ---
            caption_text = sanitize_for_latex(filename)

            print(f"  -> Adding file: {filename}")

            body_content.append(f"\\section{{{caption_text}}}\n")
            body_content.append(
                f"\\lstinputlisting[caption={{{caption_text}}}]{{{latex_path}}}\n\n")

    return preamble + doc_start + "".join(body_content) + doc_end


if __name__ == '__main__':
    print("Creating safe symbolic links for problematic filenames...")
    create_safe_symlinks()

    print("\nGenerating LaTeX content...")
    latex_code = generate_latex_content()
    with open(OUTPUT_FILENAME, 'w', encoding='utf-8') as f:
        f.write(latex_code)
    print(f"\n✅ Successfully generated '{OUTPUT_FILENAME}'!")
    print("\nNext steps:")
    print(f"1. Open your terminal and run the LaTeX compiler: pdflatex {
          OUTPUT_FILENAME}")
    print(f"2. Run it a second time to build the table of contents: pdflatex {
          OUTPUT_FILENAME}")
    print("3. Your PDF book will be ready!")
