### NJU-git Project

#### Tips
- ```init``` 也需要创建空 ```tree``` 对象以及 ```commit``` 对象(msg 为 "initial commit")
- ```add``` 时，如果文件已经在暂存区，则不需要重复添加
- ```add``` 其文件都存在于暂存区(暂存区存储在 ```.nit/index``` 文件中)
- ```commit``` 时，需要将暂存区的文件全部提交，创建新的 ```tree``` 对象，然后创建新的 ```commit``` 对象
- ```status``` 会比较 ```index``` 暂存区和工作区的文件差异
- 对于在暂存区的文件，若是改名，则对应```status```为 ```deleted```，改名后的文件未被追踪，说明暂存区至少要通过文件名、文件内容的 ```hash``` 来唯一标识文件