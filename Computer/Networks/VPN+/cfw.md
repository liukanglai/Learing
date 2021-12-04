#

        mixin: # 注意下面缩进
          dns:
            enable: true
            enhanced-mode: redir-host
            nameserver:
              - 114.114.114.114 # 真实请求DNS，可多设置几个
          tun:
            enable: true
            stack: system # 或 gvisor
            dns-hijack: # DNS劫持设置为系统DNS
              - 1.0.0.1:53 # 请勿更改

