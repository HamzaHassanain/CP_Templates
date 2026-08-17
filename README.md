# My Competitive Programming Templates

#### This is the templates I use for competitive programming, I have created this repository to keep track of my templates and to share them with others. I will keep updating this repository with new templates and algorithms as I learn through the competitive programming journey.

## Building the printable notebook

`MakePDFS.py` collects every `.cpp` in the topic folders into a two-column,
small-font reference notebook in the style of an ICPC team reference document.

```bash
python3 MakePDFS.py --build
```

That writes `Hamza.pdf` in the repository root. Everything intermediate goes
into `.texbuild/` (gitignored), so the source tree stays clean — delete that
folder any time.

### Options

| Flag | Effect |
| --- | --- |
| `--build` | run `pdflatex` twice (needed for the table of contents) and copy the PDF out |
| `--title` / `--author` | cover text and PDF metadata |
| `--out NAME` | output basename, default `Hamza` → `Hamza.pdf` |
| `--font scriptsize\|footnotesize\|tiny` | listing font size, default `scriptsize` |
| `--no-numbers` | drop line numbers for more horizontal room |

To squeeze the page count down for a printing limit:

```bash
python3 MakePDFS.py --build --font tiny --no-numbers
```

Without `--build` the script only generates `.texbuild/Hamza.tex`, which you can
compile yourself — run `pdflatex` **twice** so the table of contents resolves.

### Requirements

Python 3 and a LaTeX install. **`texlive-full` is not needed** (it is ~5.5 GB);
the document only uses `geometry`, `listings`, `xcolor`, `fancyhdr`, `hyperref`
and `lmodern`, so this is enough:

```bash
sudo apt-get install --no-install-recommends \
    texlive-latex-base texlive-latex-recommended texlive-fonts-recommended
```

### Adding cheat sheets

Any `.tex` file placed in a `Notes/` folder is `\input` at the front of the
notebook, before the code chapters. Write plain LaTeX fragments there (no
preamble, no `\begin{document}`) for formula sheets, game-theory tables,
complexity budgets, and similar reference material.

### Filenames

Source filenames are sanitized automatically when staged into `.texbuild/`, so
spaces, parentheses and other special characters are fine — the original names
are still what appear as headings in the PDF.
