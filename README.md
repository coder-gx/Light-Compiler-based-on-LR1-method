做了如下更新：


\begin{itemize}
    \item 将文法读取与语法分析分开。考虑到文法文法文件改动不是很频繁，而代码文件每次编译进本都要改变，之前我们将每次编译过程都加入了文法的读取，但是很耗时间，Debug状态下编译一次大概10s左右。但是，经过测试发现，99\%的时间耗费在文法分析的过程，即生成语法DFA和ACTION——GOTO表的过程，我们将文法分析脱离了编译过程，只放在初始化的构造函数、重置过程、以及新文法的导入过程。这样可以大大节省整体的编译时间，最后编译一次大致只需要10ms，提升了进1000倍的速度。
    \item 添加了中间代码生成的相关接口。包括可视化接口、错误日志接口和四元式结果记录。丰富了程序的整体逻辑。
    \item 改进了交互模式，增加了编译时间记录，写入编译日志，便于回溯编译过程，同时观测编译效率，更好的进行针对性的算法设计。
\end{itemize}
