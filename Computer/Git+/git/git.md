# command

    init    add .     config --global    commit -m ""     remote add origin    push

- git init:在当前目录生成

- git status:查看状态
- git add (file): . 代表所有, 准备提交
- git diff: 查看改动
- git reset: 返回

- git config --global user.name/email ""
- git commit -m "description": if no -m, it will a vi,then you can write it
- git commit -a : add+commit,but 未追踪 file no
- git config --global core.editor vim

- vim .gitignore:write a file name --还没有追踪的 file
- git rm --cached "....": 停止追踪

- git branch ...
- git checkout ...: 切换 branch
- git merge ...: 合并 branch
- git branch -d ...: del -D 强制

- git remote add origin https://

- git push --set-upstream origin master
- git push -u origin master

- git config credential.helper store
- git log --diff-filter=D --summary: 查看 commit id。
- git checkout $id~1 file: 恢复文件。
- git config credential.helper store: remember user

- git clone http
- git pull : 下载更新
- git pull --rebase

- git blame 查看历史(人)
  整理自 https://www.worldhello.net/

# git

"歌易特"

> 2005 年 4 月 3 日，开始开发 Git
>
> 2005 年 4 月 6 日，项目发布
>
> 2005 年 4 月 7 日，Git 就可以作为自身的版本控制工具了
>
> 2005 年 4 月 18 日，发生第一个多分支合并
>
> 2005 年 4 月 29 日，Git 的性能就已经达到了 Linus 的预期
>
> 2005 年 6 月 16 日，Linux 核心 2.6.12 发布，那时 Git 已经在维护 Linux 核心的源代码了

- 备份
- 重提交
- 差异比较
- 下载

# 工作区

# 暂存区

- git add
- 撤回：git checkout --filename

# 仓库区
