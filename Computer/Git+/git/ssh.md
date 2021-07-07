# ssh

$ git config –global user.name “xxxxname”

$ git config –global user.email “171xxxx887@qq.com”

$ git config --global credential.helper cache
# Set git to use the credential memory cache
$ git config --global credential.helper 'cache --timeout=3600'
# Set the cache to timeout after 1 hour (setting is in seconds)

# owner:

- picture-settings-ssh and gpg keys.new ssh key-paste your ssh
- your ssh: rm -r .ssh
- creat:ssh-keygen -t rsa -C "liukanglai" (or: email) 
- cat ~/.ssh/id_rsa.pub (copy it to github's ssh)
- ssh -T git@github.com (check)

本地提交代码免密码提交（ssh方式）
使用如下命令：git remote -v，会出现如下内容：

$ git remote -v
origin https://github.com/lony2016/myLeetcode.git (fetch)
origin https://github.com/lony2016/myLeetcode.git (push)

出现上述内容说明还是通过https方式访问的。

下面把访问方式改为ssh方式，输入如下命令：
$ git remote set-url origin git@github.com:liukanglai/Learning-diary.git

## staff/employee

- give ssh
