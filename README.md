# lisp-interpreter

====

[Code on Github](https://github.com/liuyongvs/lisp-interpreter)

[在线阅读](http://xianbai.me/learn-md/)

文档目录
----

* [关于 Markdown](article/about/readme.md)
  - [第一个 Markdown 文档](article/about/helloworld.md)
  - [Hello.md](article/about/hello.md)
* [语法](article/syntax/readme.md)
  - [段落与换行](article/syntax/paragraphs-and-line-breaks.md)
  - [标题](article/syntax/headers.md)
  - [引用](article/syntax/blockquotes.md)
  - [列表](article/syntax/lists.md)
  - [代码](article/syntax/code.md)
  - [分隔线](article/syntax/horizontal-rule.md)
  - [超链接](article/syntax/links.md)
  - [图片](article/syntax/images.md)
  - [强调](article/syntax/emphasis.md)
  - [字符转义](article/syntax/blackslash-escapes.md)
* [扩展语法](article/extension/readme.md)
  - [删除线](article/extension/strikethrougn.md)
  - [代码块和语法高亮](article/extension/code-blocks-and-highlighting.md)
  - [表格](article/extension/table.md)
  - [Task List](article/extension/task-list.md)
* [编辑器与扩展](article/tools/readme.md)
  - [MarkdownPad](article/tools/markdownpad.md)
  - [Texts](article/tools/texts.md)
  - [MarkPad](article/tools/markpad.md)
  - [MdCharm](article/tools/mdcharm.md)
  - [Markdown Edit](article/tools/markdown-edit.md)
  - [CuteMarkEd](article/tools/cutemarked.md)
  - [Haroopad](article/tools/haroopad.md)
  - [Mou](article/tools/mou.md)
  - [MacDown](article/tools/macdown.md)
  - [Markdown Pro](article/tools/markdown-pro.md)
  - [ReText](article/tools/retext.md)
  - [sublime-markdown-extended](article/tools/sublime-markdown-extended.md)
  - [Atom Markdown Preview](article/tools/atom-markdown-preview.md)
  - [IDEA Markdown](article/tools/idea-markdown.md)
  - [Cmd Markdown](article/tools/cmd-markdown.md)
  - [StackEdit](article/tools/stackedit.md)
  - [Dillinger](article/tools/dillinger.md)
* [格式转换](article/convert/readme.md)
  - [HTML](article/convert/html.md)
  - [PDF](article/convert/pdf.md)
  - [Word](article/convert/word.md)

参考资料
----

1. [Project Markdown][project-markdown]
2. [Markdown Syntax zh_TW][syntex-tw]
3. [Markdown Syntax CN][syntex-cn]
4. [Wiki Markdown][wiki-markdown]
5. [XBeta Wiki Markdown][xbeta-markdown]

[project-markdown]: http://daringfireball.net/projects/markdown/ "Project Markdown"
[syntex-tw]: https://github.com/othree/markdown-syntax-zhtw/blob/master/syntax.md "Markdown Syntax zh_TW"
[syntex-cn]: http://wowubuntu.com/markdown/ "Markdown Syntax CN"
[wiki-markdown]: http://zh.wikipedia.org/zh-cn/Markdown "Wiki Markdown"
[xbeta-markdown]: http://xbeta.org/wiki/show/Markdown "XBeta Wiki Markdown"

共享协议
----

本文由 [LearnShare][learnshare] 整理并在 [CC BY-SA 3.0][CC] 协议下发布，主要为了给自己和各位朋友作为学习 Markdown 的入门及参考资料。

请各位遵循 [Markdown: License][license] 及其它参考文献的共享协议来使用、修改和发布。

[learnshare]: https://github.com/learnshare "LearnShare"
[CC]: http://zh.wikipedia.org/wiki/Wikipedia:CC "Wiki: CC"
[license]: http://daringfireball.net/projects/markdown/license "Markdown: License"




implement a lisp interpreter with low level language in C.
1. design the data struct to represent the atom type, 
    ATOM_NIL,
		ATOM_SYMBOL,
		ATOM_INTEGER,
		ATOM_PAIR,
		ATOM_FUNCTION,
		ATOM_CLOSURE

2. lexer and parser

3. support basic functional programming operate such as car cdr cons nil 

4. extend to arithmetic (+, - , * ,/)

5. support boolean and if(which is a short-circuit evaluation), such as(< 1 2) (= (+ 1 2) (- 21 12))

6. expand to lambda expressions and closures(lambda (x) (* x x)). and simple syntatic sugar(such as ('(1 2)), which is a list)

7. use the mark sweep algorithm proposed by John McCarthy to implement a simle garbage collector .

more works deserve to be done, and i will do it later
1. support other type, such as float, big number, complex and so on

2. support variadic functions, such as(+ 1 2 3 4)

3. tail recursive optimization

4. environment use the hash table to speed up

5. alternative GC

#!/usr/bin/env python                                                                                                                     
# gpu_stat.py [DELAY [COUNT]]                                                                                                             
# dump some gpu stats as a line of json                                                                                                   
# {"util":{"PCIe":"0", "memory":"11", "video":"0", "graphics":"13"}, "used_mem":"161"}                                                    

import json, socket, subprocess, sys, time
try:
    delay = int(sys.argv[1])
except:
    delay = 1

try:
    count = int(sys.argv[2])
except:
    count = None

i = 0
while True:
    GPU = socket.gethostname() + ":0[gpu:0]"
    cmd = ['nvidia-settings', '-t', '-q', GPU + '/GPUUtilization']
    gpu_util = subprocess.check_output(cmd).strip().split(",")
    gpu_util = dict([f.strip().split("=") for f in gpu_util])
    cmd[-1] = GPU + '/UsedDedicatedGPUMemory'
    gpu_used_mem = subprocess.check_output(cmd).strip()
    print json.dumps({"used_mem": gpu_used_mem, "util": gpu_util, "time": int(time.time())})
    if count != None:
        i += 1
        if i == count:
            exit(0)
    time.sleep(delay)
