### NJU-git Project

#### Tips
- ```init``` 也需要创建空 ```tree``` 对象以及 ```commit``` 对象(msg 为 "initial commit")
- ```add``` 时，如果文件已经在暂存区，则不需要重复添加
- ```add``` 其文件都存在于暂存区(暂存区存储在 ```.nit/index``` 文件中)
- ```commit``` 时，需要将暂存区的文件全部提交，创建新的 ```tree``` 对象，然后创建新的 ```commit``` 对象
- ```status``` 会比较 ```index``` 暂存区和工作区的文件差异
- 对于在暂存区的文件，若是改名，则对应```status```为 ```deleted```，改名后的文件未被追踪，说明暂存区至少要通过文件名、文件内容的 ```hash``` 来唯一标识文件


- 根据第三点，```add``` 时不仅要修改 ```index``` 文件，还要创建```/objects```目录下的对应文件的 ```blob```(若修改过或是新文件)
- 根据第四点，```commit``` 时要读取```index```，并且根据此文件来创建```/objects```目录下的对应文件的 ```blob``` 和 ```tree```，并且要修改 ```HEAD``` 文件来表示```current newest commit```，还需要更新 ```log``` 文件来记录```commit``` 的 ```hash``` 和 ```msg```
- 如此看 ```index``` 就是一个未完成的 ```tree```

- ```index```文件格式：
    - 一行一个文件，格式为```hash filename```，其中```hash```为文件内容的```hash```，```filename```为文件名
- ```HEAD```文件格式：
    - 仅一行，格式为```hash```，其中```hash```为```commit```对象的```hash```
- ```log```文件格式：
    - 两行一个```commit```对象的```hash```和```msg```，格式为```hash\nmsg\n```，其中```hash```为```commit```对象的```hash```，```msg```为```commit```的```msg```
- ```objects```目录下的文件格式：
    - ```blob```文件格式为```content```，其中```content```为文件内容
    - ```tree```文件格式为```hash filename```，其中```hash```为文件内容的```hash```，```filename```为文件名
    - ```commit```文件格式为```hash\nparent\nmsg```，其中```hash```为对应的```tree```对象的```hash```，```parent```为```commit```对象的```hash```，```msg```为```commit```的```msg```
- 上述文件名为```hash```的前两位字符，作为目录名，后38位字符作为文件名，如```objects/12/3456789abcdef0123456789abcdef01234567```

- 当 ```tree``` 为空时，若没有内容，则可能与空的```blob``` 冲突，所以需要作出区分

- ```commit```会继承，此时需要做的是继承```parent```的```tree```，```index```之中只记录其修改或者新添加的文件.
- 即当commit的时候，才开始构建 tree，将 parent 的 tree 读取出来，然后将 index 之中的文件附加进去
- ```checkout``` 时，需要将```commit```的```tree```中的文件写入到工作区，同时更新```HEAD```文件


- ```ADD``` 状态需要写入 ```index``` 和 ```objects``` 目录下的 ```blob``` 文件
- ```MODIFIED_ADDED``` 状态需要写入(覆盖) ```index``` 和 ```objects``` 目录下的 ```blob``` 文件(删除原来的创建新的)
- ```MODIFIED_COMMITED``` 状态需要写入(如果有要覆盖) ```index``` 和 ```objects``` 目录下的 ```blob``` 文件(不删除原来的，直接创建新的)
- ```SAME``` 状态说明文件和当前 ```commit``` 其中对应文件一致，需要判定```index```是否有该文件，若有则删除该记录，不需要创建新的```blob```文件，因为```commit```会继承
- ```REMOVE```状态，需要删除```index```中的记录，不需要删除```objects```目录下的```blob```文件

### ```map<std::string filename, std::string fileSha1>```
- ```filename```为文件名，```fileSha1```为文件内容的```hash```
- 