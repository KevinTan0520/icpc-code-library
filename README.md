# ICPC Code Library

这个仓库用于维护 ICPC 纸质代码模板。原始代码模板目录保留在根目录下，LaTeX 文档会从这些算法文件夹中扫描源码并生成双栏版式。

## 生成纸质模板

在仓库根目录运行：

```powershell
.\build-template.ps1
```

脚本会扫描算法文件夹中的源码，重新生成 `template/generated/code-listings.tex`，并在本机有 `latexmk` 或 `xelatex` 时构建 `template/build/main.pdf`。

只刷新生成的 TeX，不编译 PDF：

```powershell
.\build-template.ps1 -NoCompile
```

## 文件布局

- `template/main.tex`：紧凑双栏 LaTeX 主模板。
- `template/generated/`：自动生成的 LaTeX include 文件。
- `template/build/`：PDF 和 LaTeX 编译中间产物。
- `template/tmp/`：渲染检查、预览图片等临时产物。
- `template/tools/generate_template_tex.py`：扫描源码并写出生成文件。
- `build-template.ps1`：根目录入口脚本，负责调用生成器并编译 PDF。

生成器会忽略编辑器目录、构建目录、`template/` 和编译产物。新增 `.cpp`、`.hpp`、`.java`、`.py` 等源码文件后，重新运行脚本即可更新文档。
