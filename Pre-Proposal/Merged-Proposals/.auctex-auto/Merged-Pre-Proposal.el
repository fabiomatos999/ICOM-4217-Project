(TeX-add-style-hook
 "Merged-Pre-Proposal"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "12pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("fontenc" "T1")))
   (TeX-run-style-hooks
    "latex2e"
    "../Fabio-Proposal/Pre-Proposal-Fabio"
    "../Guillermo-Proposal/Pre-Proposal-Guillermo"
    "../Ruben-Proposal/Pre-Proposal-Ruben"
    "../Enrique-Proposal/Pre-Proposal-Enrique"
    "article"
    "art12"
    "unicode"
    "setspace"
    "fontenc"
    "helvet"))
 :latex)

