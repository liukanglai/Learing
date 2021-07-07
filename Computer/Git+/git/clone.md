# command

- git clone ...

# 错误处理

    - 提示错误信息如下： sign_and_send_pubkey: signing failed: agent refused operation
    错误原因：在ssh账户中没有加入新生成的密钥
    解决：将密码加入ssh账户
    逐条运行如下命令
    eval "$(ssh-agent -s)"
    ssh-add
