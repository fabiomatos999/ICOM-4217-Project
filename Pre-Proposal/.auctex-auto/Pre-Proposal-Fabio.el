(TeX-add-style-hook
 "Pre-Proposal-Fabio"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("report" "12pt")))
   (TeX-run-style-hooks
    "latex2e"
    "report"
    "rep12"
    "setspace"))
 :latex)

