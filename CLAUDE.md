# CLAUDE.md

> **非工程项目标识**：本仓库是算法学习项目（C++ 算法 / 数据结构片段集合，含 heap / kmp / segment_tree / quick_sort / radix_sort / merge_sort / str_hash / zfunction / least_common_ancestor 等子目录），**不走** `~/.claude/CLAUDE.md` §新项目工程地基 + agent-deck 应用 SDK 注入的 `resources/claude-config/CLAUDE.md` §新项目工程地基 通用工程约定。

## 不适用的通用约定

Agent 在本仓库工作时**不要主动套用**以下通用约定（这些约定面向长期维护的工程项目，对学习项目是噪音）：

- **目录骨架**：本项目按算法主题分子目录（`heap/` `kmp/` `quick_sort/` 等），不需要 `src/` `build/` `dist/` `ref/changelogs/` `ref/reviews/` `ref/conventions/` 等工程根目录
- **README / CLAUDE.md 三问**：纯学习代码不需要"用户视角功能总览"维护
- **改动后必做 changelog / review**：学习提交直接走 git commit message，不需要 `ref/changelogs/CHANGELOG_X.md` / `ref/reviews/REVIEW_X.md` 双轨归档
- **反复反馈升级约定 / tally**：不维护 `ref/conventions/tally.md` 候选状态机
- **complex plan workflow + worktree 隔离**：学习代码不开 plan 文件、不进 git worktree，不走 §Step 0 RFC / §Step 0.5 spike / §Step 1.5 Deep-Review 流程
- **deep-review / 决策对抗 / 异构 reviewer 双对抗**：学习代码不引入多轮 review

## 适用的通用约定（最小集）

仍遵守的通用约定：

- **中文回复**（user CLAUDE.md §通用约定 §输出）
- **不主动创建无关 .md 文件**（本 CLAUDE.md 是用户明示要求的例外）
- **macOS 没有 `timeout` / `gtimeout`**：Bash 命令体不写 `timeout 5m ...`，超时只走 Bash 工具自身 `timeout` 参数

## Agent 工作姿势

- **加新算法**：建新子目录（如 `union_find/`），里面放对应 `.cpp` / `.h` / `README.md`（子目录级别 README 描述算法本身）
- **修 bug / 优化**：直接改对应子目录文件 + git commit
- **回答算法题**：可以 explain 代码 / 画图 / 给复杂度分析，但**不要**主动建 `ref/` 子目录归档
