(TeX-add-style-hook
 "Merged-Pre-Proposal"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("fontenc" "T1")))
   (TeX-run-style-hooks
    "latex2e"
    "../Guillermo-Proposal/Pre-Proposal-Guillermo"
    "../Ruben-Proposal/Pre-Proposal-Ruben"
    "article"
    "art12"
    "unicode"
    "setspace"
    "fontenc"
    "helvet"))
 :latex)

