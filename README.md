### NJU-git Project

项目简介： ```NJU-git``` 简称 ```Nit```，是一个简单的版本控制系统，实现了简易版的 ```git``` 功能，包括 ```init```、```add```、```commit```、```status```、```checkout```、```log```、```rm``` 等功能

- ```init``` 用于初始化一个新的 ```Nit``` 仓库
- ```add``` 添加文件到暂存区，目前暂时仅支持添加文件，不支持添加目录
- ```commit``` 提交暂存区的文件到仓库(存储于```objects```目录下)
- ```status``` 查看当前暂存区文件以及将移出```commit```的文件
- ```checkout``` 切换到指定的 ```commit```，并将文件写入到工作区
- ```log``` 查看当前所有的 ```commit``` 的 ```hash``` 和 ```msg```
- ```rm``` 将文件从当前```commit```中移出
- 暂不支持 ```branch```、```merge```、```rebase``` 等功能
#### Build(By CMake in Linux)
```bash
mkdir build && cd build
cmake ..
make
```

#### Usage
```bash
Usage: nit <command> [arguments]

Commands:
  init                    Initialize a new Nit repository
  status                  Show the working tree status
  add <file>              Add file contents to the index
  checkout <commit id>    Checkout a commit
  log                     Show commit hash value and msg
  commit <msg>            Record changes to the repository
  rm <file>               Remove file from the working tree and index
```

#### File Structure
- ```.nit```
  - ```index``` 存储暂存区的文件
  - ```HEAD``` 存储当前最新的 ```commit``` 的 ```sha1```
  - ```log``` 存储当前所有的 ```commit``` 的 ```sha1``` 和 ```msg```
  - ```remove``` 存储移出当前 ```commit``` 的文件
  - ```objects```目录
    - 存储所有的普通文件 ```blob```、```tree```、```commit``` 对象

#### File Format
- ```index``` 文件格式
  - 一行一个暂存区文件，格式为:
    ```bash
    <sha1> <filename>
    ```
    其中```sha1 = hash(filename + "\n" + content) ```
- ```HEAD``` 文件格式
  - 仅一行，格式为: 
    ```bash
    <sha1>
    ```
    其中```sha1```为当前 ```commit``` 的 ```sha1```
- ```log``` 文件格式
  - 四行一组，包含```commit```对象的```sha1```和```msg```，格式为: 
    ```bash
    === 
    commit: <sha1>
    <msg>

    ```
    其中```sha1 = hash(commitContent)```，```msg```为```commit```的```msg```
- ```remove``` 文件格式
  - 一行一个移出的文件，格式为:
    ```bash
    <filename>
    ```
- ```objects``` 目录下的文件格式
  - 普通文件格式：
    ```bash
    <filename>
    <content>
    ```
  - ```tree``` 文件格式(与```index```格式相同)
    一行一个暂存区文件，格式为: 
    ```bash
    <sha1> <filename>
    ```
    其中```sha1 = hash(filename + "\n" + content)```
  - ```commit``` 文件格式
    ```bash
    tree: <treeSha1>
    parent: <parentSha1>
    <msg>
    ```
    其中```treeSha1```为```commit```对应的```tree```对象的```sha1```，```parentSha1```为```parent commit```的```sha1```，```msg```为```commit```的```msg```

#### Added Object Type
- 被执行 ```nit add <file>``` 的```file```对象属性：
    - ```UNCHECKED```        : 未检查的异常属性
    - ```SAME```             : 在当前```commit```中，并且与其中同名文件内容一致
    - ```ADD```              : 新添加到暂存区的文件(即没被当前```commit```追踪)
    - ```MODIFIED_ADDED```   : 已经在暂存区，但是修改过内容(但并不被当前```commit```追踪)
    - ```MODIFIED_COMMITED```: 已经被当前```commit```追踪，并且修改过内容
    - ```REMOVED```          : 当前被暂存区记录或被当前```commit```追踪，但文件已经被删除
    - ```NOTEXIST```         : 文件不存在

- ```ADD``` 状态需要写入 ```index``` 和 ```objects``` 目录下的 ```blob``` 文件
- ```MODIFIED_ADDED``` 状态需要写入(覆盖) ```index``` 和 ```objects``` 目录下的 ```blob``` 文件(删除原来的创建新的)
- ```MODIFIED_COMMITED``` 状态需要写入(如果有要覆盖) ```index``` 和 ```objects``` 目录下的 ```blob``` 文件(不删除原来的，直接创建新的)
- ```SAME``` 状态说明文件和当前 ```commit``` 其中对应文件一致，需要判定```index```是否有该文件，若有则删除该记录，不需要创建新的```blob```文件，因为```commit```会继承
- ```REMOVE```状态，需要删除```index```中的记录，不需要删除```objects```目录下的```blob```文件
- ```NOTEXIST```和```UNCHECKED```状态返回错误

#### Some Details
- ```init``` 创建空 ```tree``` 对象以及 ```commit``` 对象(```msg``` 为 ```initial commit```)
- 对于在暂存区的文件，若是改名，则对应```status```为 ```deleted```
- 暂存区至少要通过文件名、文件内容的 ```hash``` 来唯一标识文件

- ```index``` 可以看作是一个未完成的 ```tree```

- ```objects```目录下的文件名为```hash```的前两位字符，作为目录名，后38位字符作为文件名，如```objects/12/3456789abcdef0123456789abcdef01234567```

- ```commit```所追踪的文件会继承，此时需要做的是继承```parent```的```tree```，```index```之中只记录其修改或者新添加的文件.