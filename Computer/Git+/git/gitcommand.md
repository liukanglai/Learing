# command

    init    add .     config --global    commit -m ""     remote add origin    push



- git init:在当前目录生成

- git status:查看状态
- git add (file): . 代表所有,  准备提交
- git diff: 查看改动
- git reset: 返回

- git config --global user.name/email ""
- git commit -m "description": if no -m, it will a vi,then you can write it
- git commit -a : add+commit,but 未追踪file no
- git config --global core.editor vim

- vim .gitignore:write a file name --还没有追踪的file
- git rm --cached "....": 停止追踪

- git branch ...
- git checkout ...: 切换branch
- git merge ...: 合并branch
- git branch -d ...: del  -D 强制

- git remote add origin https://

- git push --set-upstream origin master
- git push -u origin master

- git config credential.helper store
- git log --diff-filter=D --summary: 查看commit id。
- git checkout $id~1 file: 恢复文件。
- git config credential.helper store:  remember user


- git clone http
- git pull : 下载更新 
- git pull --rebase

- git blame 查看历史(人)
