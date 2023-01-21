(TeX-add-style-hook
 "Pre-Proposal-Fabio"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("fontenc" "T1")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art12"
    "unicode"
    "setspace"
    "fontenc"
    "helvet"))
 :latex)

