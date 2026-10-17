#!/usr/bin/env bash
# C++ 调试与 RE 分析速查（根据提供的原对话正文整理）。
# 命令环境：Linux / WSL，Bash + GCC/libstdc++；源码 main.cpp。
# 按需复制命令，不要整份执行；C++ 示例与错误报告均放在注释中。

# ===== 1. 推荐配置：ASan + UBSan + STL Debug Mode =====
g++ -std=c++17 -O1 -g \
    -Wall -Wextra -Wshadow \
    -fsanitize=address,undefined \
    -fno-omit-frame-pointer \
    -fno-sanitize-recover=all \
    -D_GLIBCXX_DEBUG \
    main.cpp -o main.debug
./main.debug < input.txt
# 先确认编译成功，再运行，避免误跑旧程序。
# -fsanitize=address：非法内存访问；undefined：未定义行为。
# -D_GLIBCXX_DEBUG：libstdc++ 容器、迭代器检查，非 sanitizer。
# -g：保留源码行号等信息；-O1：兼顾速度与定位效果。
# -fno-omit-frame-pointer：改善调用栈的完整性。
# -fno-sanitize-recover=all：停止恢复，优先定位第一个错误。
# -Wall -Wextra -Wshadow：编译期警告，应先阅读并处理。
# 调试版时间、内存开销明显增加，不用它衡量正式 TLE/MLE。

# ===== 2. 检测器与选择性检查 =====
# ASan  -fsanitize=address：堆/栈/全局越界、非法释放、释放后访问。
# UBSan -fsanitize=undefined：有符号溢出、非法移位、整数除零、
#   空指针、未对齐访问等；不是所有逻辑错误都能被检测。
# LSan  -fsanitize=leak：内存泄漏，支持情况取决于平台。
# TSan  -fsanitize=thread：多线程数据竞争，单线程竞赛通常不用；
#   不可与 ASan 合用。
# MSan  -fsanitize=memory：未初始化内存读取，仅 Clang，
#   对工具链及所用库要求较高，不作为日常默认配置。
# GCC 可单独选择（替换或补充编译命令中的检查项）：
# -fsanitize=signed-integer-overflow
# -fsanitize=shift
# -fsanitize=bounds
# -fsanitize=alignment
# -fsanitize=integer 是 Clang 的扩展组合，不能直接套用到 GCC。
# 它也检查部分非 UB 但可疑的整数操作，见第 7 节。

# ===== 3. ASan：如何从输出找到问题 =====
# 越界示例（C++）：vector<int> a(5); int* p = a.data();
# cout << p[5] << '\n';  // 读第六个元素，合法下标只有 0..4。
# 典型报告结构（地址和行号为示意）：
# ==12345==ERROR: AddressSanitizer: heap-buffer-overflow
# READ of size 4 at 0x603000000054 thread T0
#     #0 0x... in main /home/user/main.cpp:8
#     #1 0x... in __libc_start_main ...
# 0x603000000054 is located 0 bytes after
# 20-byte region [0x603000000040, 0x603000000054)
# allocated by thread T0 here:
#     #0 0x... in operator new(unsigned long)
#     #1 0x... in std::vector<int>::vector(...)
#     #2 0x... in main /home/user/main.cpp:5
# SUMMARY: AddressSanitizer: heap-buffer-overflow
# ==12345==ABORTING
# 阅读顺序：类型 -> 读写/宽度 -> 自己的源码行 -> 边界 -> 分配栈。
# (1) heap-buffer-overflow：动态分配内存区域的边界被越过。
# (2) READ of size 4：读 4 字节，常见于 int；WRITE of size 8
#     是写 8 字节，可能是 long long 或指针，不能仅靠宽度定类型。
# (3) #0 通常是直接触发处；若在库函数中，向 #1、#2 等调用者
#     找第一处自己的源码。main.cpp:8 表示触发在第 8 行。
#     触发位置未必是根因：下标可能在之前的 DFS 中已算错。
# (4) 20-byte region 若存 5 个 4 字节 int，0 bytes after 表示
#     恰好在末尾外，即 p[5]；4 bytes before 常提示 p[-1]。
#     合法区域左闭右开，结合 sizeof(T) 与下标来源检查。
# (5) allocated by 指分配位置，可定位容器创建处与实际容量。
# heap-use-after-free 还要对照三组栈：当前访问、freed by、
# previously allocated by，追踪释放/扩容后仍被使用的旧地址。

# ===== 4. UBSan：runtime error 的含义与修复 =====
# 格式：文件名:行号:列号: runtime error: 具体错误。
# 定位该表达式后，检查操作数类型、值及其来源。
# 4.1 signed integer overflow：有符号整数溢出。
# int a = 1000000000; int b = a * 3;
# 报告：1000000000 * 3 cannot be represented in type 'int'
# 即 3000000000 超出 32 位 int；检查乘法、累加、距离更新等。
# int a = 1000000000, b = 3;
# long long c = a * b;       // 错：乘法仍先在 int 中进行。
# long long c = 1LL * a * b; // 在乘法前提升类型。
# 也可直接让参与运算的变量为 long long；仍须保证结果不超范围。
# 4.2 shift exponent：移位量为负或不小于提升后左操作数的位宽。
# int x = 1; int y = x << 32;
# 报告：shift exponent 32 is too large for 32-bit type 'int'
# 需要 64 位可写 1LL << 32；1LL << k 仍须保证位移量有效。
# 检查状压 DP、二进制 Trie 中 1 << k 的类型和 k 的范围。
# 4.3 division by zero：整数 / 或 % 的除数为 0。
# int x = 10, y = 0; cout << x / y;
# 检查分母、取模操作；即使没用浮点数也可能收到 SIGFPE
# （Floating point exception），不能按信号名称断言浮点错误。
# 4.4 index ... out of bounds：固定大小数组的下标越界。
# int a[5] = {}; cout << a[5];
# 报告：index 5 out of bounds for type 'int[5]'
# 核对每一维容量、0/1 起始及循环边界；UBSan 不保证覆盖
# vector 的所有非法下标，还需要 ASan / STL Debug Mode。
# 4.5 load of null pointer / reference binding to null pointer：
# int* p = nullptr; cout << *p; // 空指针解引用。
# 追踪指针/引用的来源；查初始化、查找结果及空容器操作。
# 4.6 load of misaligned address：地址不满足类型的对齐要求。
# 例如报告 long long requires 8 byte alignment。
# 优先检查指针损坏、非法类型转换、此前的越界写入；
# 若指向 STL 头文件，要沿调用栈回到自己的容器操作。

# ===== 5. STL Debug Mode：补足容器边界检查 =====
# C++ 示例：
# vector<int> a;
# a.reserve(10);
# a[0] = 42; // 错：reserve 只增加 capacity，size 仍为 0。
# 已分配的内存范围内也可能存在逻辑越界，普通 ASan 未必报错；
# 部分库有容器边界标注，但不能一概依赖。
# -D_GLIBCXX_DEBUG 的典型输出：
# Error: attempt to subscript container
# with out-of-bounds index 0,
# but container only holds 0 elements.
# 应改为 resize(10) 后按下标写入，或使用 push_back。
# 还能检查 end() 解引用、失效迭代器及部分算法前置条件。
# erase 后、vector 扩容后，检查此前保存的迭代器/引用/指针。
# 这是 libstdc++ 调试模式，会改变部分 STL 类型的大小和行为；
# 跨编译单元传递 STL 容器时，各单元统一启用并重新编译。
# 较低开销可用 -D_GLIBCXX_ASSERTIONS，但检查范围更少。

# ===== 6. 常见 RE 错误速查：含义 -> 优先检查 =====
# heap-buffer-overflow   堆越界 -> vector/new[]/动态数组下标。
# stack-buffer-overflow  栈内对象越界 -> 局部数组、DFS 内数组。
# global-buffer-overflow 全局越界 -> 全局容量、循环边界。
# heap-use-after-free    释放后访问 -> delete、erase、容器重分配。
# stack-use-after-return 返回后访问局部对象 -> 返回局部引用/指针。
# stack-overflow         栈耗尽 -> 递归深度、局部大数组。
# attempting double-free 重复释放 -> delete/free 的生命周期。
# SEGV on unknown address 非法访问 -> 空/坏指针、非法下标。
# signed integer overflow 有符号溢出 -> int 乘法、累加、距离更新。
# shift exponent         无效位移量 -> 1 << k 的类型与 k。
# division by zero       整数除零 -> 分母、取模除数。
# misaligned address     非法对齐 -> 指针运算、类型转换、此前越界。
# LeakSanitizer: detected memory leaks -> new/malloc 是否被释放。
# stack-overflow 与 stack-buffer-overflow 是两类不同的问题。
# DEADLYSIGNAL 只说明严重信号，继续看后续类型和调用栈。
# 调用栈大量重复同一函数：查递归出口、visited、无向图回父边；
# 即使逻辑正确，过深 DFS 也可能耗尽栈空间。

# ===== 7. 控制输出、恢复行为与专项检查 =====
# 让 UBSan 打印调用栈（依赖可用的符号化工具/运行环境）：
UBSAN_OPTIONS=print_stacktrace=1 ./main.debug < input.txt
# 只保存诊断，stdout 仍显示在终端：
./main.debug < input.txt 2> error.log
# 同时分开保存正常输出与诊断；sanitizer 报告通常写到 stderr：
./main.debug < input.txt > output.txt 2> error.log
# UBSan 默认许多检查报错后继续，ASan 通常首次内存错误即退出。
# 优先使用 -fno-sanitize-recover=all 修复首个错误后重跑。
# 若要 UBSan 尽量继续，将其替换为 -fsanitize-recover=undefined；
# 某些错误不能恢复，且 UB 后状态可能不可靠，后续报告谨慎分析。
# Clang 专项整数检查（不能直接换成 g++）：
clang++ -std=c++17 -O1 -g \
    -fsanitize=address,undefined,integer \
    -fno-sanitize-recover=all \
    main.cpp -o main.integer
# 可查某些窄化/符号转换和无符号回绕，如 int x=300; char y=x;
# 但哈希等算法可能有意无符号回绕，报告并不必然代表 bug。
# 怀疑未初始化变量时，可另用 GCC 编译期诊断（并非静态链接）：
g++ -std=c++17 -O2 \
    -Wall -Wextra -Wshadow \
    -Wuninitialized -Wmaybe-uninitialized \
    main.cpp -o main.warnings
# 告警也不能覆盖所有路径；ASan 不是未初始化读取检测器，
# 需要运行时专项检测时考虑 Clang MSan，见第 2 节。

# ===== 8. 三种配置与比赛排错流程 =====
# Debug：使用第 1 节命令，定位越界、UB 和 STL 误用。
# Stress：大量随机对拍，保留部分低开销 STL 断言。
g++ -std=c++17 -O2 -D_GLIBCXX_ASSERTIONS main.cpp -o main.stress
# 观察性能时仍需考虑断言开销，并与最终 Release 配置对照。
# Release：最终提交，标准与优化选项以评测环境为准。
g++ -std=c++17 -O2 main.cpp -o main
# Debug 报错：读类型 -> 找自己的源码行 -> 追踪下标/指针/数值。
# Debug 无报错仍 WA：用小数据暴力对拍，保存失败输入与种子。
# 对拍还要检查退出码；仅比较答案可能把 RE 当作 WA。
# 只有 -O2 出问题：优先检查 UB、未初始化变量及优化相关问题。
# 核心组合：-fsanitize=address,undefined -g -D_GLIBCXX_DEBUG
