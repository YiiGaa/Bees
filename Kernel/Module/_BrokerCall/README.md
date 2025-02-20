# _BrokerCall-调用Task函数

主要功能包括：
- call-调用函数
- end-停止循环

模块参数的值说明：

- 如果希望设置的值为固定值，则直接设置对应类型的值即可
- 如果希望从`passParam`中动态获取值作为设置的值，则请以`get##key`作为模块参数的设置值
- 如果想要获取的值在passParam的里层（嵌套json），则请以`>>`定位，如`get##key_1>>key_2`

# ※ call-调用函数
- 组入版本：1.0=2025.01.12
- 一般用于跨Task的函数调用

```json
{
	"name":"_BrokerCall",
	"param":{
    "_action":"call",
    "_call":"",
    "_data":""
  }
}
```

| key | 说明 | 是否必要 | 默认值 | 类型 | 组入/更新版本 |
| --- | --- | --- | --- | --- | --- |
| _action | 固定为"call" | 是 |  | String | 1.0=2025.01.12 |
| _identity | 识别key，为了方便叫停这个调用 | 否 | "" | String | 1.0=2025.01.12 |
| _call | 调用的函数名称 | 是 |  | String | 1.0=2025.01.12 |
| _param | 调用函数时，传递的数据池 | 否 | 整个passParam | Object | 1.0=2025.01.12 |
| _loopCount | 循环次数，-1为无限次 | 否 | 1 | Int | 1.0=2025.01.12 |
| _sleepTime | 每次循环间隔时间(微秒)，1000000=1秒 | 否 | 1000000（1秒） | Int64 | 1.0=2025.01.12 |
| _isErrorStop | 当调用返回错误，是否继续循环，true：中断，false：继续调用 | 否 | false | Int | 1.0=2025.01.12 |
| _isAdoptFunRet | 是否以调用函数的返回值作为此模块的返回，true：是，false：否 | 否 | true | String | 1.0=2025.01.12 |
| _errorKey | 将函数调用返回的errorCode写入passParam的key | 否 | 不写入passParam | String | 1.0=2025.01.12 |
| _messageKey | 将函数调用返回的errorCode对应的message写入passParam的key | 否 | 不写入passParam | String | 1.0=2025.01.12 |

模块参数补充说明：

### > _identity

- 识别key，方便叫停这个调用

- 若缺省时，默认为空字符串`""`

- 识别key不需要唯一

### > _call

- 调用的函数名称
- 调用的函数名称为Task的函数，命名格式为`Task名::函数名`，例如`OrderGet::OrderListen`
- 编译整个工程时，会自动登记所有Task中除去`Start`以外的函数，并以`Task名::函数名`的格式登记函数，记录在`Task/Task.hpp`文件中
- 在`sample.cpp`测试代码中，调用的函数需要先通过`TOOLS_CallFunctionMark()`注册

### > _param

- 调用函数时，传递的数据池
- 在省略设置时，会传递整个passParam，且passParam的修改会被保留；不省略设置时，修改都不会被保留

# ※ end-停止循环

- 组入版本：1.0=2025.01.12
- 停止循环仅提前结束循环调用，不会中断调用中的函数，不会唤醒sleep等待

```json
{
  "name":"_BrokerCall",
  "param":{
    	"_action":"end",
    	"_identity":""
  }
}
```

| key | 说明 | 是否必要 | 默认值 | 类型 | 组入/更新版本 |
| --- | --- | --- | --- | --- | --- |
| _action | 固定为"end" | 是 |  | String | 1.0=2025.01.12 |
| _identity | 识别key，停止对应的调用循环 | 否 | "" | String | 1.0=2025.01.12 |

模块参数补充说明：

### > identity

- 识别key，停止对应的调用循环
- 若多个调用循环采用同一个`identity`，会同时停止这些调用循环

# ◎ 模块单独测试

在工程根目录，运行shell指令

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerCall
```

也可以通过`Christmas插件`运行`ShellExcute>>Build#Module`，并在插件打开的终端中输入`_BrokerCall`

`sample.cpp`是专门用于单独测试的代码

模块编译后会自动运行，编译的可执行文件生成在`/Build/sample`

# ◎ 更新列表

**1.0=2025.01.12**

- 模块建立