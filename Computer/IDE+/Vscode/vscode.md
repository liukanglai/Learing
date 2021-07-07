fire code!!!!!!!!!!!!!!!!!!!!!
# windows

## 


- vim
- c/c++
- code runner


## debug

- creat a file .vscode
- vim launch.json tasks.json

- lanuch.json:
        {
    "version": "0.2.0",
    "configurations": [
        {
            "name": "C/C++",
            "type": "cppdbg",
            "request": "launch",
            "program": "${fileDirname}/${fileBasenameNoExtension}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "preLaunchTask": "compile",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
- tasks.json:
        {
    "version": "2.0.0",
    "tasks": [{
            "label": "compile",
            "command": "g++",
            "args": [
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}"
            ],
            "problemMatcher": {
                "owner": "cpp",
                "fileLocation": [
                    "relative",
                    "${workspaceRoot}"
                ],
                "pattern": {
                    "regexp": "^(.*):(\\d+):(\\d+):\\s+(warning|error):\\s+(.*)$",
                    "file": 1,
                    "line": 2,
                    "column": 3,
                    "severity": 4,
                    "message": 5
                }
            },
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}

## more

- firacode font
- editor.font Font Ligatures,
```
 "editor.fontFamily": "'Fira Code' ",
    "editor.fontLigatures": true,
```
- 16,500

## c/c++

- install extension c/c++,code runner
- download mingw-w64

    > 右键此电脑，属性-高级系统设置-环境变量-path-编辑-新建（将mingw中bin路径放入）
