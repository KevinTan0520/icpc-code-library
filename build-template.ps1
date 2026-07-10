param(
    [switch]$NoCompile
)

$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $Root
try {
    python template/tools/generate_template_tex.py

    if (-not $NoCompile) {
        New-Item -ItemType Directory -Force -Path template/build | Out-Null

        if (Get-Command latexmk -ErrorAction SilentlyContinue) {
            latexmk -xelatex -interaction=nonstopmode -halt-on-error -outdir=template/build template/main.tex
        } elseif (Get-Command xelatex -ErrorAction SilentlyContinue) {
            xelatex -interaction=nonstopmode -halt-on-error -output-directory=template/build template/main.tex
            xelatex -interaction=nonstopmode -halt-on-error -output-directory=template/build template/main.tex
        } else {
            Write-Warning 'No latexmk or xelatex found. Generated TeX only; install TeX Live or MiKTeX to build the PDF.'
        }
    }
} finally {
    Pop-Location
}
